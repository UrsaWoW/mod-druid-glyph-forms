# mod-druid-glyph-forms

Repurposes the Druid minor glyph **Glyph of Unburdened Rebirth** (spell
58161, item 43331): instead of making Rebirth free of its mana cost, having
the glyph slotted makes a **Tauren** or **Night Elf** druid's Bear Form, Dire
Bear Form, and Cat Form use the original **classic (pre-WotLK)** models
instead of the newer per-hair-color (Night Elf) / per-skin-color (Tauren)
models introduced in Wrath of the Lich King.

The item itself is renamed to **Glyph of Ancestral Forms**, with its
description updated to "Brings back the original Druid forms.", via
`data/sql/db-world/base/item_template.sql`. Only the `name` and `description`
fields differ from the stock item 43331 row - everything else (quality,
displayid, spellid_1, etc.) is left untouched.

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

To apply the classic model:

- `spell_dru_bear_form_glyph_forms`, `spell_dru_dire_bear_form_glyph_forms`,
  and `spell_dru_cat_form_glyph_forms` are `AuraScript`s bound (as additional
  `spell_script_names` rows, alongside core's own `spell_dru_feral_swiftness`
  binding on 5487/9634) to each form's `SPELL_AURA_MOD_SHAPESHIFT` effect.
  `AfterEffectApply` runs after core's own shapeshift handling has already set
  the WotLK model, then overwrites it via `Unit::SetDisplayId` with the
  classic model for the target's race - but only when the target
  `HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH)` (58161) and the module is
  enabled. Other races are left untouched.
- `spell_dru_glyph_of_unburdened_rebirth`, an `AuraScript` bound to spell
  58161, zeroes out `EFFECT_0`'s amount via `DoEffectCalcAmount` so the
  glyph's real 100% Rebirth mana-cost reduction no longer applies - the glyph
  slot is now dedicated entirely to the classic-forms behavior above.
- `data/sql/db-world/base/spell_script_names.sql` adds the four
  `spell_script_names` rows above.

## Configuration

See `conf/mod_druid_glyph_forms.conf.dist`:

- `GlyphForms.Enable` - when enabled (default), Glyph of Unburdened Rebirth's
  mana-cost reduction is replaced by the classic-forms behavior described
  above. When disabled, both scripts fall back to stock behavior: the glyph's
  real 100% Rebirth mana-cost reduction applies, and Bear/Dire Bear/Cat Form
  keep whichever WotLK per-race model core would normally assign.

## Known limitations

- Only affects Bear Form, Dire Bear Form, and Cat Form - Travel Form, Aquatic
  Form, Tree of Life, and Moonkin Form have no WotLK per-race model variant
  in `player_shapeshift_model` to begin with, so there is nothing to revert
  for them.
- Only Tauren and Night Elf druids are affected, matching which races
  actually received new per-customization models in WotLK.
- The spell's in-game tooltip text (the flavor line describing what the
  glyph effect does) is sourced from the client's `Spell.dbc`/locale data,
  which this module cannot change from the server side - it will still read
  the stock "reduces Rebirth's mana cost" wording even though the glyph no
  longer does that. Only the item's name and description (both server-side
  `item_template` fields) were updated.
