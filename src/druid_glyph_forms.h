/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODULE_DRUID_GLYPH_FORMS_H
#define MODULE_DRUID_GLYPH_FORMS_H

#include "Config.h"

#define MODULE_STRING "mod-druid-glyph-forms"

enum DruidGlyphFormsSpells
{
    SPELL_DRUID_BEAR_FORM                   = 5487,
    SPELL_DRUID_DIRE_BEAR_FORM               = 9634,
    SPELL_DRUID_CAT_FORM                     = 768,

    // Item 43331's spellid_1 (58161) is only the momentary "apply glyph" cast
    // (SPELL_EFFECT_APPLY_GLYPH) fired when socketing the glyph - it never
    // becomes a persistent aura. GlyphProperties.dbc entry 434 (its misc
    // value) points to 57857 as the actual glyph-effect spell that gets cast
    // as a lasting aura and shows in the Glyphs UI - that's the one to check
    // with HasAura() and to bind the reagent-waiver-neutralizing script to.
    // Its EFFECT_0 is SPELL_AURA_NO_REAGENT_USE (waives Rebirth's reagent),
    // not a mana-cost reduction - the tooltip/module name is a misnomer.
    SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH = 57857
};

// Custom CreatureDisplayInfo rows (creaturedisplayinfo_dbc_druid_glyph_forms.sql,
// merged into the client's CreatureDisplayInfo.dbc by the dbtodbc patch
// pipeline), not any of Blizzard's own IDs. Confirmed by direct testing: the
// WotLK 3.2+ client silently overrides/re-resolves a server-set DisplayID
// whenever it recognizes it as part of the known per-race skin-tone family
// for a shapeshift form (e.g. 8571/29409-29412 for Tauren Cat Form all share
// that recognition, including the CustomizationID=255 fallback row) - so
// pointing at any of Blizzard's own classic-fallback IDs (8571, 2289, 2281,
// 892) gets silently corrected back to the WotLK model, even though the
// server-side field is set correctly. An ID the client has never seen is not
// part of that recognized family and is rendered as sent. Each row here
// reuses the real shared mesh (ModelID, matching the WotLK-family sibling)
// with the true legacy pre-3.2 texture (verified via direct MPQ extraction -
// low resolution, distinct DXT encoding, and for Bear Form specifically,
// orphaned by no stock DBC row referencing it at all).
enum DruidGlyphFormsDisplayIds
{
    DISPLAY_CAT_FORM_NIGHTELF  = 900104,
    DISPLAY_CAT_FORM_TAUREN    = 900101,
    DISPLAY_BEAR_FORM_NIGHTELF = 900103,
    DISPLAY_BEAR_FORM_TAUREN   = 900102
};

struct DruidGlyphFormsConfig
{
    bool enabled = true;

    static DruidGlyphFormsConfig* instance()
    {
        static DruidGlyphFormsConfig instance;
        return &instance;
    }

    void Load()
    {
        enabled = sConfigMgr->GetOption<bool>("GlyphForms.Enable", true);
    }
};

void AddSC_druid_glyph_forms();

#endif // MODULE_DRUID_GLYPH_FORMS_H
