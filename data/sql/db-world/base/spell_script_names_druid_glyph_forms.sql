DELETE FROM `spell_script_names` WHERE `spell_id` IN (5487, 9634, 768, 58161) AND `ScriptName` IN
    ('spell_dru_bear_form_glyph_forms', 'spell_dru_dire_bear_form_glyph_forms',
     'spell_dru_cat_form_glyph_forms', 'spell_dru_glyph_of_unburdened_rebirth');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(5487, 'spell_dru_bear_form_glyph_forms'),
(9634, 'spell_dru_dire_bear_form_glyph_forms'),
(768, 'spell_dru_cat_form_glyph_forms'),
(58161, 'spell_dru_glyph_of_unburdened_rebirth');
