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
    SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH = 58161
};

enum DruidGlyphFormsDisplayIds
{
    DISPLAY_CAT_FORM_NIGHTELF  = 892,
    DISPLAY_CAT_FORM_TAUREN    = 8571,
    DISPLAY_BEAR_FORM_NIGHTELF = 2281,
    DISPLAY_BEAR_FORM_TAUREN   = 2289
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
