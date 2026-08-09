DELETE FROM `creaturedisplayinfo_dbc` WHERE `ID` IN (900101, 900102, 900103, 900104);
INSERT INTO `creaturedisplayinfo_dbc`
    (`ID`, `ModelID`, `SoundID`, `ExtendedDisplayInfoID`, `CreatureModelScale`, `CreatureModelAlpha`,
     `TextureVariation_1`, `TextureVariation_2`, `TextureVariation_3`, `PortraitTextureName`,
     `BloodLevel`, `BloodID`, `NPCSoundID`, `ParticleColorID`, `CreatureGeosetData`, `ObjectEffectPackageID`)
VALUES
    -- ModelID here points at each form's genuine legacy mesh (see
    -- 2026_08_09_01_add_legacy_model_data.sql), not the shared WotLK model -
    -- 900201/900202/900203 are new custom CreatureModelData rows, 3143 is the
    -- stock client's own (previously unused) Night Elf Cat legacy mesh entry.
    (900101, 900201, 0, 0, 1.0, 255, 'DruidCatTaurenSkinLegacy', 'DruidCatTaurenSkinLegacy', '', '', -1, 0, 0, 0, 0, 0),
    (900102, 900202, 3022, 0, 1.2, 255, 'DruidTaurenBearSkin', '', '', '', 1, 0, 0, 0, 0, 0),
    (900103, 900203, 3022, 0, 1.0, 255, 'DruidBearSkin', '', '', '', 1, 0, 0, 0, 0, 0),
    (900104, 3143, 0, 0, 0.9, 255, 'DruidCatSkin_Legacy', '', '', '', 1, 1, 0, 0, 0, 0);
