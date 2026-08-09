# mod-druid-glyph-forms

Repurposes the Druid minor glyph **Glyph of Unburdened Rebirth** (spell
58161, item 43331): instead of waiving Rebirth's reagent requirement, having
the glyph slotted makes a **Tauren** or **Night Elf** druid's Bear Form, Dire
Bear Form, and Cat Form use the original **classic (pre-WotLK)** models
instead of the newer per-hair-color (Night Elf) / per-skin-color (Tauren)
models introduced in Wrath of the Lich King.

The item itself is renamed to **Glyph of Ancestral Forms** via
`data/sql/db-world/base/item_template.sql` (only the `name` field differs
from the stock item 43331 row - everything else, including `description`, is
left untouched there). Its description is updated separately, by
`data/sql/db-world/updates/2026_08_08_00_fix_glyph_spell_id_and_text.sql`
(see "Spell name/description" below) to
"Bear Form, Dire Bear Form, and Cat Form use their original forms."

## Design notes

WotLK gave Night Elf and Tauren druids race-and-customization-specific
shapeshift models, replacing the single classic model each race previously
used. AzerothCore stores these in the `player_shapeshift_model` table, with
the classic model kept as the `CustomizationID = 255` catch-all fallback row:

| Form                        | Night Elf classic model | Tauren classic model |
|------------------------------|:-----------------------:|:---------------------:|
| Cat Form (768)               | 892                      | 8571                   |
| Bear Form (5487) / Dire Bear Form (9634) | 2281        | 2289                   |

Travel Form, Aquatic Form, Tree of Life, and Moonkin Form have no per-race
variants in this table to begin with (WotLK never gave them race-specific
models), so this module does not touch them - there is nothing to revert.

### 58161 vs 57857

Item 43331's `spellid_1` is 58161, which is what gets cast when the item is
*used* to socket the glyph. But 58161 only has a momentary
`SPELL_EFFECT_APPLY_GLYPH` effect (no aura) - per `Spell::EffectApplyGlyph`,
it looks up `GlyphProperties.dbc` entry 434 (58161's effect misc value) and
casts *that* entry's `SpellId`, which is a different spell: **57857**. 57857
is the one that becomes a persistent aura and is what the Glyphs UI shows
once slotted. Blizzard gives both IDs the same Name/Description text, which
made this easy to miss - but only 57857 ever shows up in `HasAura()` checks,
and only 57857 carries the real `SPELL_AURA_NO_REAGENT_USE`/`EFFECT_0`
reagent-waiver effect. The module targets 57857 throughout.

To apply the classic model:

- `spell_dru_bear_form_glyph_forms`, `spell_dru_dire_bear_form_glyph_forms`,
  and `spell_dru_cat_form_glyph_forms` are `AuraScript`s bound (as additional
  `spell_script_names` rows, alongside core's own `spell_dru_feral_swiftness`
  binding on 5487/9634) to each form's `SPELL_AURA_MOD_SHAPESHIFT` effect.
  `AfterEffectApply` (registered on `AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK`,
  matching core's own `HandleAuraModShapeshift`) runs after core's own
  shapeshift handling has already set the WotLK model, then overwrites it via
  `Unit::SetDisplayId` with the classic model for the target's race - but
  only when the target `HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH)`
  (57857) and the module is enabled. Other races are left untouched.
- `spell_dru_glyph_of_unburdened_rebirth`, an `AuraScript` bound to spell
  57857. Despite the name, 57857's `EFFECT_0` is `SPELL_AURA_NO_REAGENT_USE`
  (waives Rebirth's reagent), not a mana-cost reduction - core's
  `AuraEffect::HandleNoReagentUseAura` recomputes the player's
  `PLAYER_NO_REAGENT_COST` fields from every active `SPELL_AURA_NO_REAGENT_USE`
  effect on them each time one applies or is removed. `AfterEffectApply`/
  `AfterEffectRemove` (registered on `AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK`)
  run after core's own recompute and redo it themselves, excluding this
  effect's own `SpellClassMask` - so the reagent waiver never actually takes
  effect, without disturbing any unrelated `SPELL_AURA_NO_REAGENT_USE` effects
  the player might have. The glyph slot is now dedicated entirely to the
  classic-forms behavior above.
- `data/sql/db-world/base/spell_script_names_druid_glyph_forms.sql` adds the
  base `spell_script_names` bindings (originally all four scripts bound to
  58161 - a bug also fixed by the file below).
- `data/sql/db-world/updates/2026_08_08_00_fix_glyph_spell_id_and_text.sql`
  rebinds `spell_dru_glyph_of_unburdened_rebirth` from 58161 to 57857, and
  renames both 58161 and 57857 to "Glyph of Ancestral Forms" with a
  description matching the classic-forms behavior via full-row overrides in
  the `spell_dbc` table (see "Spell name/description" below) - both IDs need
  it since 58161's text is what the item's own "Use:" tooltip line pulls
  from, while 57857's is what the Glyphs UI panel shows once slotted.

## Configuration

See `conf/mod_druid_glyph_forms.conf.dist`:

- `GlyphForms.Enable` - when enabled (default), Glyph of Unburdened Rebirth's
  reagent waiver is replaced by the classic-forms behavior described above.
  When disabled, both scripts fall back to stock behavior: the glyph's real
  reagent waiver for Rebirth applies, and Bear/Dire Bear/Cat Form keep
  whichever WotLK per-race model core would normally assign.

## Known limitations

- Only affects Bear Form, Dire Bear Form, and Cat Form - Travel Form, Aquatic
  Form, Tree of Life, and Moonkin Form have no WotLK per-race model variant
  in `player_shapeshift_model` to begin with, so there is nothing to revert
  for them.
- Only Tauren and Night Elf druids are affected, matching which races
  actually received new per-customization models in WotLK.

## Spell name/description

Spell tooltip text (Name/Description, used by the item's "Use:" line and by
the Glyphs UI panel) is normally baked into the client's `Spell.dbc` and out
of server reach. This setup has a `dbtodbc` pipeline (a separate tool, not
part of this module) that merges rows from the `acore_world.spell_dbc` table
on top of the compiled `Spell.dbc` before it's written into the client's
patch MPQ - a non-`NULL` column in a `spell_dbc` row overrides that field for
the matching spell `ID`, `NULL` passes the original DBC value through.
`data/sql/db-world/updates/2026_08_08_00_fix_glyph_spell_id_and_text.sql` uses
this to rename both 58161 and 57857's Name/Description. Because `spell_dbc`'s non-text
columns are `NOT NULL DEFAULT '0'` (a sparse insert would zero out every
other field of the spell), each row is a full-row snapshot of that spell's
current data with only the text columns changed, not a sparse override.

**For the renamed text to reach players**, after applying this SQL: re-run
`dbtodbc.exe` to regenerate `Spell.dbc` from the DB, and reload/relaunch the
client so it picks up the updated patch MPQ. This module has no dependency
on that pipeline being present - without it, the spell keeps its stock
Blizzard name/description (only the item's own tooltip, via `item_template`,
would reflect the rename).
