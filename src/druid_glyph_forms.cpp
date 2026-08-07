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

#include "druid_glyph_forms.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Unit.h"

// Sets the target's display id to the classic (pre-WotLK) Night Elf/Tauren
// model for a shapeshift form, when Glyph of Unburdened Rebirth is slotted.
// Other races keep whatever model core's own shapeshift handling already set.
static void ApplyClassicFormModel(Unit* target, uint32 nightElfDisplayId, uint32 taurenDisplayId)
{
    if (!DruidGlyphFormsConfig::instance()->enabled)
        return;

    if (!target->HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH))
        return;

    switch (target->getRace())
    {
        case RACE_NIGHTELF:
            target->SetDisplayId(nightElfDisplayId);
            break;
        case RACE_TAUREN:
            target->SetDisplayId(taurenDisplayId);
            break;
        default:
            break;
    }
}

// 5487 - Bear Form
class spell_dru_bear_form_glyph_forms : public AuraScript
{
    PrepareAuraScript(spell_dru_bear_form_glyph_forms);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_BEAR_FORM, SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        ApplyClassicFormModel(GetTarget(), DISPLAY_BEAR_FORM_NIGHTELF, DISPLAY_BEAR_FORM_TAUREN);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_bear_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

// 9634 - Dire Bear Form
class spell_dru_dire_bear_form_glyph_forms : public AuraScript
{
    PrepareAuraScript(spell_dru_dire_bear_form_glyph_forms);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_DIRE_BEAR_FORM, SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        ApplyClassicFormModel(GetTarget(), DISPLAY_BEAR_FORM_NIGHTELF, DISPLAY_BEAR_FORM_TAUREN);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_dire_bear_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

// 768 - Cat Form
class spell_dru_cat_form_glyph_forms : public AuraScript
{
    PrepareAuraScript(spell_dru_cat_form_glyph_forms);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_CAT_FORM, SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        ApplyClassicFormModel(GetTarget(), DISPLAY_CAT_FORM_NIGHTELF, DISPLAY_CAT_FORM_TAUREN);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_cat_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

// 58161 - Glyph of Unburdened Rebirth
// Stock effect: EFFECT_0 reduces Rebirth's mana cost by 100%. This module
// repurposes the glyph entirely into the classic-forms behavior above, so
// the mana-cost reduction is zeroed out here.
class spell_dru_glyph_of_unburdened_rebirth : public AuraScript
{
    PrepareAuraScript(spell_dru_glyph_of_unburdened_rebirth);

    void CalcCostAmount(AuraEffect const* /*aurEff*/, int32& amount)
    {
        if (DruidGlyphFormsConfig::instance()->enabled)
            amount = 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_glyph_of_unburdened_rebirth::CalcCostAmount, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
    }
};

void AddSC_druid_glyph_forms()
{
    DruidGlyphFormsConfig::instance()->Load();
    RegisterSpellScript(spell_dru_bear_form_glyph_forms);
    RegisterSpellScript(spell_dru_dire_bear_form_glyph_forms);
    RegisterSpellScript(spell_dru_cat_form_glyph_forms);
    RegisterSpellScript(spell_dru_glyph_of_unburdened_rebirth);
}
