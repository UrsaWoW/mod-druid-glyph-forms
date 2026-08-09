-- Genuine pre-3.2 Druid form meshes (Creature\DruidCatTauren\DruidCatTauren_Legacy.mdx,
-- Creature\DruidBear\DruidBearTauren_Legacy.mdx, Creature\DruidBear\DruidBear_Legacy.mdx)
-- confirmed present in the client MPQs (own .skin/.anim files, not just a texture
-- variant) but never wired into CreatureModelData.dbc - unlike the Night Elf Cat
-- legacy mesh, which the stock client already registers as ModelID 3143. Field
-- values otherwise cloned from each mesh's WotLK sibling row (213/214/1232).
DELETE FROM `creaturemodeldata_dbc` WHERE `ID` IN (900201, 900202, 900203);
INSERT INTO `creaturemodeldata_dbc`
    (`ID`, `Flags`, `ModelName`, `SizeClass`, `ModelScale`, `BloodID`, `FootprintTextureID`,
     `FootprintTextureLength`, `FootprintTextureWidth`, `FootprintParticleScale`, `FoleyMaterialID`,
     `FootstepShakeSize`, `DeathThudShakeSize`, `SoundID`, `CollisionWidth`, `CollisionHeight`, `MountHeight`,
     `GeoBoxMinX`, `GeoBoxMinY`, `GeoBoxMinZ`, `GeoBoxMaxX`, `GeoBoxMaxY`, `GeoBoxMaxZ`,
     `WorldEffectScale`, `AttachedEffectScale`, `MissileCollisionRadius`, `MissileCollisionPush`, `MissileCollisionRaise`)
VALUES
    -- Tauren Cat Form legacy mesh (clone of 1232)
    (900201, 0, 'Creature\\DruidCatTauren\\DruidCatTauren_Legacy.mdx', 1, 1.0, 1, 7, 18.0, 12.0, 1.0, 0, 0, 0, 1089,
     0.6111000180244446, 2.0309998989105225, 1.241371989250183,
     -2.7952399253845215, -0.9328140020370483, -0.04487299919128418, 1.0340839624404907, 0.6177219748497009, 1.471182942390442,
     1.0, 1.0, 0.0, 0.0, 0.0),
    -- Tauren Bear Form legacy mesh (clone of 214)
    (900202, 16, 'Creature\\DruidBear\\DruidBearTauren_Legacy.mdx', 1, 1.0, 1, 7, 18.0, 12.0, 1.0, 0, 0, 0, 3022,
     0.6111000180244446, 2.0309998989105225, 0.0,
     -1.5319019556045532, -0.9212859869003296, -0.07204300165176392, 1.7752900123596191, 0.8599590063095093, 1.8594030141830444,
     1.0, 1.0, 0.0, 0.0, 0.0),
    -- Night Elf Bear Form legacy mesh (clone of 213)
    (900203, 16, 'Creature\\DruidBear\\DruidBear_Legacy.mdx', 1, 1.0, 1, 7, 18.0, 12.0, 1.0, 0, 0, 0, 3022,
     0.6111000180244446, 2.0309998989105225, 0.0,
     -1.5319019556045532, -1.0700260400772095, -0.07204300165176392, 1.678473949432373, 1.0412260293960571, 1.8594030141830444,
     1.0, 1.0, 0.0, 0.0, 0.0);

-- Blizzard's own Night Elf Cat Form legacy mesh (ModelID 3143, stock/existing
-- row - not one of our custom IDs above) is missing CreatureModelDataFlags'
-- NoAttachedWeapons (0x10). Both Bear Form meshes (213/214) already carry
-- this flag, which is why weapons never render in Bear Form; the current-gen
-- Cat mesh (1231/1232) apparently has no active weapon-attachment bone at
-- all so it never needed the flag, but this older mesh does have one and
-- shows the equipped weapon during autoattack without it. All other fields
-- are an exact snapshot of the stock row - only Flags changes, 0 -> 16.
DELETE FROM `creaturemodeldata_dbc` WHERE `ID` = 3143;
INSERT INTO `creaturemodeldata_dbc`
    (`ID`, `Flags`, `ModelName`, `SizeClass`, `ModelScale`, `BloodID`, `FootprintTextureID`,
     `FootprintTextureLength`, `FootprintTextureWidth`, `FootprintParticleScale`, `FoleyMaterialID`,
     `FootstepShakeSize`, `DeathThudShakeSize`, `SoundID`, `CollisionWidth`, `CollisionHeight`, `MountHeight`,
     `GeoBoxMinX`, `GeoBoxMinY`, `GeoBoxMinZ`, `GeoBoxMaxX`, `GeoBoxMaxY`, `GeoBoxMaxZ`,
     `WorldEffectScale`, `AttachedEffectScale`, `MissileCollisionRadius`, `MissileCollisionPush`, `MissileCollisionRaise`)
VALUES
    (3143, 16, 'Creature\\DRUIDCAT\\DruidCat_Legacy.mdx', 0, 1.0, 3, 4, 18.0, 12.0, 1.0, 0, 0, 0, 3004,
     0.6111000180244446, 2.0309998989105225, 1.241371989250183,
     -2.5970590114593506, -0.7774230241775513, -0.006202999968081713, 1.0575159788131714, 0.53370201587677, 1.536152958869934,
     1.0, 1.0, 0.0, 0.0, 0.0);
