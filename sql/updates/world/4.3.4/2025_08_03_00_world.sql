-- Stormwind Gryphons
DELETE FROM `creature_static_flags_override` WHERE `SpawnId` IN (314127, 314133, 314137, 314183, 314134, 314135, 314132);
INSERT INTO `creature_static_flags_override` (`SpawnId`, `DifficultyId`, `StaticFlags1`) VALUES
(314127, 0, 0x1 | 0x4),
(314133, 0, 0x1 | 0x4),
(314137, 0, 0x1 | 0x4),
(314183, 0, 0x1 | 0x4),
(314134, 0, 0x1 | 0x4),
(314135, 0, 0x1 | 0x4),
(314132, 0, 0x1 | 0x4);

-- Orgrimmar Wind Riders
DELETE FROM `creature_static_flags_override` WHERE `SpawnId` IN (310988, 310989, 310981, 310971, 310968, 310966, 310967);
INSERT INTO `creature_static_flags_override` (`SpawnId`, `DifficultyId`, `StaticFlags1`) VALUES
(310988, 0, 0x1 | 0x4),
(310989, 0, 0x1 | 0x4),
(310981, 0, 0x1 | 0x4),
(310971, 0, 0x1 | 0x4),
(310968, 0, 0x1 | 0x4),
(310966, 0, 0x1 | 0x4),
(310967, 0, 0x1 | 0x4);
