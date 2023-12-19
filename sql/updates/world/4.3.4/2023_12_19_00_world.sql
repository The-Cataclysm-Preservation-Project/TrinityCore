SET @CGUID := 254934;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+26;
DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+26;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 44797, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -353.178985595703125, -700.6669921875, 888.1903076171875, 5.532693862915039062, 7200, 0, 0, 4149700, 0, 0, 0, 0, 0, 15595), -- Time Warden (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86003 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+1, 44652, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -279.110992431640625, -680.593994140625, 888.1912841796875, 3.106686115264892578, 7200, 0, 0, 4149700, 0, 0, 0, 0, 0, 15595), -- Slate Dragon (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86003 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+2, 44650, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -318.704986572265625, -722.68798828125, 888.1912841796875, 3.193952560424804687, 7200, 0, 0, 4149700, 0, 0, 0, 0, 0, 15595), -- Storm Rider (Area: 0 - Difficulty: 1) CreateObject2 - !!! might be temporary spawn !!!
(@CGUID+3, 44645, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -280.118011474609375, -659.385009765625, 888.18829345703125, 1.675516128540039062, 7200, 0, 0, 4149700, 0, 0, 0, 0, 0, 15595), -- Nether Scion (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86003 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+4, 44687, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -265.8909912109375, -740.02301025390625, 907.36334228515625, 2.338741064071655273, 7200, 0, 0, 26143110, 0, 0, 0, 0, 0, 15595), -- Proto-Behemoth (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 83412 - Force Flying) - !!! might be temporary spawn !!!
(@CGUID+5, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -347.079986572265625, -728.12799072265625, 891.76434326171875, 1.117010712623596191, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+6, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -347.5780029296875, -717.302001953125, 891.7633056640625, 1.047197580337524414, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+7, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -340.75, -722.47601318359375, 891.76434326171875, 1.169370532035827636, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+8, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -350.563995361328125, -721.66302490234375, 891.7633056640625, 1.01229095458984375, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+9, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -346.433990478515625, -724.04901123046875, 891.76434326171875, 1.099557399749755859, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+10, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -350.571014404296875, -725.4169921875, 891.7633056640625, 1.064650893211364746, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+11, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -342.00201416015625, -726.52801513671875, 891.76434326171875, 1.169370532035827636, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+12, 44641, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -344.8389892578125, -720.16302490234375, 891.76434326171875, 1.082104086875915527, 7200, 0, 0, 414970, 0, 0, 0, 0, 0, 15595), -- Orphaned Emerald Whelp (Area: 0 - Difficulty: 1) CreateObject2 (Auras: 86022 - Unresponsive) - !!! might be temporary spawn !!!
(@CGUID+13, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -358.9639892578125, -702.14801025390625, 888.1903076171875, 2.30383467674255371, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+14, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -348.537994384765625, -700.24700927734375, 888.1903076171875, 5.532693862915039062, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+15, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -352.803985595703125, -695.06402587890625, 888.1903076171875, 5.358160972595214843, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+16, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -353.764007568359375, -706.36102294921875, 888.1903076171875, 2.30383467674255371, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+17, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -314.985992431640625, -717.39599609375, 888.1903076171875, 0.261799395084381103, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+18, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -277.954986572265625, -687.78802490234375, 888.19232177734375, 3.159045934677124023, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+19, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -273.010009765625, -684.6939697265625, 888.1912841796875, 3.159045934677124023, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+20, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -321.9739990234375, -718.42401123046875, 888.1912841796875, 0.383972436189651489, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1
(@CGUID+21, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -284.868011474609375, -661.1510009765625, 888.1903076171875, 1.710422635078430175, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+22, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -273.9219970703125, -675.947998046875, 888.1893310546875, 6.195918560028076171, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+23, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -283.141998291015625, -685.32501220703125, 888.1912841796875, 3.159045934677124023, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+24, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -280.7239990234375, -674.46002197265625, 888.1903076171875, 0.087266460061073303, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)
(@CGUID+25, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -320.157989501953125, -727.9949951171875, 888.1912841796875, 3.246312379837036132, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326), -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1
(@CGUID+26, 44765, 671, 5334, 5334,  0x1 | 0x2 | 0x4 | 0x8, 169, 0, 0, 0, -274.890625, -656.8350830078125, 888.1893310546875, 4.956735134124755859, 7200, 0, 0, 4679, 0, 0, 0, 0, 0, 27326); -- Spike (Area: The Bastion of Twilight - Difficulty: 5) CreateObject1 (Auras: 83480 - Spike Visual)

SET @OGUID := 211121;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+1;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 205088, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, -346.6719970703125, -723.260009765625, 888.10601806640625, 5.532694816589355468, 0, 0, -0.3665008544921875, 0.93041771650314331, 7200, 255, 1, 15595), -- Whelp Cage Base (Area: 0 - Difficulty: 1) CreateObject2
(@OGUID+1, 205087, 671, 5334, 0, 0x1 | 0x2 | 0x4 | 0x8, 169, 0, -346.6719970703125, -723.260009765625, 888.10601806640625, 5.532694816589355468, 0, 0, -0.3665008544921875, 0.93041771650314331, 7200, 255, 1, 15595); -- Whelp Cage (Area: 0 - Difficulty: 1) CreateObject2

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+1;

DELETE FROM `spawn_group_template` WHERE `groupId`= 462;
INSERT INTO `spawn_group_template` (`groupId`, `groupName`, `groupFlags`) VALUES
(462, 'Bastion of Twilight - Halfus Wyrmbreaker Encounter Setup', 4);

DELETE FROM `spawn_group` WHERE `groupId`= 462;
INSERT INTO `spawn_group` (`groupId`, `spawnType`, `spawnId`) VALUES
(462, 0, @CGUID+0),
(462, 0, @CGUID+1),
(462, 0, @CGUID+2),
(462, 0, @CGUID+3),
(462, 0, @CGUID+4),
(462, 0, @CGUID+5),
(462, 0, @CGUID+6),
(462, 0, @CGUID+7),
(462, 0, @CGUID+8),
(462, 0, @CGUID+9),
(462, 0, @CGUID+10),
(462, 0, @CGUID+11),
(462, 0, @CGUID+12),
(462, 0, @CGUID+13),
(462, 0, @CGUID+14),
(462, 0, @CGUID+15),
(462, 0, @CGUID+16),
(462, 0, @CGUID+17),
(462, 0, @CGUID+18),
(462, 0, @CGUID+19),
(462, 0, @CGUID+20),
(462, 0, @CGUID+21),
(462, 0, @CGUID+22),
(462, 0, @CGUID+23),
(462, 0, @CGUID+24),
(462, 0, @CGUID+25),
(462, 0, @CGUID+26),
(462, 0, 253861),
(462, 0, 253863),
(462, 0, 253864),
(462, 0, 253866),
(462, 0, 253868),
(462, 0, 253869),
(462, 0, 253871),
(462, 0, 253874),
(462, 0, 253876),
(462, 0, 253878),
(462, 1, @OGUID+0),
(462, 1, @OGUID+1);

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_halfus_unresponsive', 'spell_halfus_fireball', 'spell_halfus_fireball_barrage');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(86003, 'spell_halfus_unresponsive'),
(86022, 'spell_halfus_unresponsive'),
(86058, 'spell_halfus_fireball'),
(83862, 'spell_halfus_fireball'),
(83719, 'spell_halfus_fireball_barrage');

UPDATE `creature_template` SET `npcflag`= 0x1000000 WHERE `entry` IN (44653, 46221, 46222, 46223, 44826, 46218, 46219, 46220, 44828, 46212, 46213, 46214, 44829, 46215, 46216, 46217);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (44653, 46221, 46222, 46223, 44826, 46218, 46219, 46220, 44828, 46212, 46213, 46214, 44829, 46215, 46216, 46217);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(44653, 83447, 1, 1),
(46221, 83447, 1, 1),
(46222, 83447, 1, 1),
(46223, 83447, 1, 1),
(44826, 83591, 1, 1),
(46218, 83591, 1, 1),
(46219, 83591, 1, 1),
(46220, 83591, 1, 1),
(44828, 83589, 1, 1),
(46212, 83589, 1, 1),
(46213, 83589, 1, 1),
(46214, 83589, 1, 1),
(44829, 83590, 1, 1),
(46215, 83590, 1, 1),
(46216, 83590, 1, 1),
(46217, 83590, 1, 1);
