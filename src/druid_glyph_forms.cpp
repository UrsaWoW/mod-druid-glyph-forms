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
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Log.h"

// Sets the target's display id to the classic (pre-WotLK) Night Elf/Tauren
// model for a shapeshift form, when Glyph of Unburdened Rebirth is slotted.
// Other races keep whatever model core's own shapeshift handling already set.
static void ApplyClassicFormModel(Unit* target, uint32 nightElfDisplayId, uint32 taurenDisplayId)
{
    if (!DruidGlyphFormsConfig::instance()->enabled)
    {
        LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: module disabled, skipping");
        return;
    }

    if (!target->HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH))
    {
        LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: {} lacks aura {}, skipping",
            target->GetGUID().ToString(), SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH);
        return;
    }

    LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: {} race={} currentDisplayId={} NE={} Tauren={}",
        target->GetGUID().ToString(), target->getRace(), target->GetDisplayId(), nightElfDisplayId, taurenDisplayId);

    switch (target->getRace())
    {
        case RACE_NIGHTELF:
            target->SetDisplayId(nightElfDisplayId);
            LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: {} set NE display {} -> now {}",
                target->GetGUID().ToString(), nightElfDisplayId, target->GetDisplayId());
            break;
        case RACE_TAUREN:
            target->SetDisplayId(taurenDisplayId);
            LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: {} set Tauren display {} -> now {}",
                target->GetGUID().ToString(), taurenDisplayId, target->GetDisplayId());
            break;
        default:
            LOG_INFO("module.druidglyphforms", "ApplyClassicFormModel: {} race {} not NE/Tauren, skipping",
                target->GetGUID().ToString(), target->getRace());
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
        AfterEffectApply += AuraEffectApplyFn(spell_dru_bear_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
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
        AfterEffectApply += AuraEffectApplyFn(spell_dru_dire_bear_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
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
        AfterEffectApply += AuraEffectApplyFn(spell_dru_cat_form_glyph_forms::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

// 57857 - Glyph of Unburdened Rebirth (the actual glyph-effect aura cast by
// GlyphProperties.dbc entry 434; item 43331/spell 58161 only fires the
// momentary SPELL_EFFECT_APPLY_GLYPH cast that triggers it, see header).
// Stock effect: EFFECT_0 is SPELL_AURA_NO_REAGENT_USE, which waives Rebirth's
// reagent requirement by OR-ing this effect's SpellClassMask into the
// player's PLAYER_NO_REAGENT_COST fields (core: AuraEffect::HandleNoReagentUseAura,
// recomputed from every currently active SPELL_AURA_NO_REAGENT_USE effect on
// the player, not just this one). This module repurposes the glyph entirely
// into the classic-forms behavior above, so after core recomputes those
// fields we recompute them again ourselves, excluding this effect's own
// SpellClassMask, to cancel out just the reagent waiver without disturbing
// any unrelated SPELL_AURA_NO_REAGENT_USE effects the player might have.
class spell_dru_glyph_of_unburdened_rebirth : public AuraScript
{
    PrepareAuraScript(spell_dru_glyph_of_unburdened_rebirth);

    void HandleReagentUse(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!DruidGlyphFormsConfig::instance()->enabled)
            return;

        Player* player = GetTarget()->ToPlayer();
        if (!player)
            return;

        flag96 mask;
        for (AuraEffect const* effect : player->GetAuraEffectsByType(SPELL_AURA_NO_REAGENT_USE))
            if (effect->GetId() != SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH)
                mask |= effect->GetSpellInfo()->Effects[effect->GetEffIndex()].SpellClassMask;

        player->SetUInt32Value(PLAYER_NO_REAGENT_COST_1, mask[0]);
        player->SetUInt32Value(PLAYER_NO_REAGENT_COST_1 + 1, mask[1]);
        player->SetUInt32Value(PLAYER_NO_REAGENT_COST_1 + 2, mask[2]);
    }

    // The spell_dru_*_form_glyph_forms scripts above only run on a real
    // shapeshift cast (AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK on the
    // shapeshift aura itself) - slotting or removing this glyph while
    // already shapeshifted doesn't touch that aura at all, so the model
    // would otherwise only update on the next form change. Force a refresh
    // here: restore the vanilla WotLK model first (glyph-unaware,
    // reflects the true current state if the glyph was just removed), then
    // reapply the classic override on top if the glyph is still present.
    void HandleFormRefresh(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!DruidGlyphFormsConfig::instance()->enabled)
            return;

        Unit* target = GetTarget();
        switch (target->GetShapeshiftForm())
        {
            case FORM_BEAR:
            case FORM_DIREBEAR:
                target->RestoreDisplayId();
                if (target->HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH))
                    ApplyClassicFormModel(target, DISPLAY_BEAR_FORM_NIGHTELF, DISPLAY_BEAR_FORM_TAUREN);
                break;
            case FORM_CAT:
                target->RestoreDisplayId();
                if (target->HasAura(SPELL_DRUID_GLYPH_OF_UNBURDENED_REBIRTH))
                    ApplyClassicFormModel(target, DISPLAY_CAT_FORM_NIGHTELF, DISPLAY_CAT_FORM_TAUREN);
                break;
            default:
                break;
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_unburdened_rebirth::HandleReagentUse, EFFECT_0, SPELL_AURA_NO_REAGENT_USE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectApplyFn(spell_dru_glyph_of_unburdened_rebirth::HandleReagentUse, EFFECT_0, SPELL_AURA_NO_REAGENT_USE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_unburdened_rebirth::HandleFormRefresh, EFFECT_0, SPELL_AURA_NO_REAGENT_USE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectApplyFn(spell_dru_glyph_of_unburdened_rebirth::HandleFormRefresh, EFFECT_0, SPELL_AURA_NO_REAGENT_USE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

// A player who logs in already shapeshifted (Bear/Dire Bear/Cat Form aura
// restored from character_aura) never goes through the AfterEffectApply
// hooks above - core sets the model directly while spawning the player, not
// via a real/reapply aura-effect event, so the classic override is lost on
// every relog until the next real shapeshift. Reapply it here once login
// finishes and the restored aura/shapeshift state is available.
class player_druid_glyph_forms_login : public PlayerScript
{
public:
    player_druid_glyph_forms_login() : PlayerScript("player_druid_glyph_forms_login") {}

    void OnPlayerLogin(Player* player) override
    {
        switch (player->GetShapeshiftForm())
        {
            case FORM_BEAR:
            case FORM_DIREBEAR:
                ApplyClassicFormModel(player, DISPLAY_BEAR_FORM_NIGHTELF, DISPLAY_BEAR_FORM_TAUREN);
                break;
            case FORM_CAT:
                ApplyClassicFormModel(player, DISPLAY_CAT_FORM_NIGHTELF, DISPLAY_CAT_FORM_TAUREN);
                break;
            default:
                break;
        }
    }
};

void AddSC_druid_glyph_forms()
{
    DruidGlyphFormsConfig::instance()->Load();
    RegisterSpellScript(spell_dru_bear_form_glyph_forms);
    RegisterSpellScript(spell_dru_dire_bear_form_glyph_forms);
    RegisterSpellScript(spell_dru_cat_form_glyph_forms);
    RegisterSpellScript(spell_dru_glyph_of_unburdened_rebirth);
    new player_druid_glyph_forms_login();
}
