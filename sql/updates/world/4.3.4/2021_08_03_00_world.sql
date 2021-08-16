DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=6944 AND `SourceEntry`=1 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=26998 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 6944, 1, 0, 0, 47, 0, 26998, 8, 0, 0, 0, 0, '', 'Bat Handler Maggotbreath - Show gossip if quest 26998 is active');
 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=44894 AND `SourceEntry`=83756 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27045 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 44894, 83756, 0, 0, 47, 0, 27045, 8, 0, 0, 0, 0, '', 'Allow spellclick if quest 27045 is active');
 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83464 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44737 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 83464, 0, 2, 31, 0, 3, 44737, 0, 0, 0, 0, '', 'Chain Visual Left - Target Subdued Forest Ettin');
 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83467 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44737 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 83467, 0, 2, 31, 0, 3, 44737, 0, 0, 0, 0, '', 'Chain Visual Right - Target Subdued Forest Ettin');
 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83763 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44883 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 83763, 0, 0, 31, 0, 3, 44883, 0, 0, 0, 0, '', 'Armoire Camera - Target Crowley');
 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=83764 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44884 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 83764, 0, 0, 31, 0, 3, 44884, 0, 0, 0, 0, '', 'Armoire Camera - Target Ivar');

DELETE FROM `conditions` WHERE `SourceEntry`= 83838 AND `SourceTypeOrReferenceId`= 13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ScriptName`, `Comment`) VALUES
(13, 1, 83838, 0, 0, 31, 0, 3, 44914, 0, 0, 0, '', 'Pick Up Orc Crate - Target Orc Sea Pup');

DELETE FROM `conditions` WHERE `SourceEntry`= 83902 AND `SourceTypeOrReferenceId`= 13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ScriptName`, `Comment`) VALUES
(13, 1, 83902, 0, 0, 31, 0, 3, 44367, 0, 0, 0, '', 'Release Diseased Mutant Bush Chicken - Target Forest Ettin');

DELETE FROM `conditions` WHERE `SourceEntry`= 83827 AND `SourceTypeOrReferenceId`= 13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ScriptName`, `Comment`) VALUES
(13, 1, 83827, 0, 0, 31, 0, 3, 44908, 0, 0, 0, '', 'Skitterweb Web - Target Skitterweb Stalker');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=11901;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(15, 11901, 0, 0, 1, 9, 0, 27069, 0, 0, 0, 0, 0, '', 'Admiral Hatchet - Show gossip option if player has quest 27069');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=84104 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=44989 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 84104, 0, 0, 31, 0, 3, 44989, 0, 0, 0, 0, '', 'Fenris Camera - Target Crowley');

 -- Fenris Isle phases
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=235 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27099 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 170, 235, 0, 0, 47, 0, 27099, 8, 0, 0, 0, 0, '', 'Fenris Keep - Add phase 170 - 27099 taken');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=172 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27099 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 170, 172, 0, 0, 47, 0, 27099, 8, 0, 0, 0, 0, '', 'Fenris Isle - Add phase 170 - 27099 taken');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=172 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27099 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 172, 0, 0, 47, 0, 27099, 8, 0, 1, 0, 0, '', 'Fenris Isle - Remove phase 169 - 27099 taken');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=235 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27099 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 235, 0, 0, 47, 0, 27099, 8, 0, 1, 0, 0, '', 'Fenris Keep - Remove phase 169 - 27099 taken');

DELETE FROM `phase_area` WHERE `AreaId`=172 AND `PhaseId`=169;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES 
(172, 169, 'Silverpine Forest - Fenris Isle (Base)');

DELETE FROM `phase_area` WHERE `AreaId`=172 AND `PhaseId`=170;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES 
(172, 170, 'Silverpine Forest - Fenris Isle - No Escape quest');

DELETE FROM `phase_area` WHERE `AreaId`=235 AND `PhaseId`=169;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES 
(235, 169, 'Silverpine Forest - Fenris Keep (Base)');

DELETE FROM `phase_area` WHERE `AreaId`=235 AND `PhaseId`=170;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES 
(235, 170, 'Silverpine Forest - Fenris Keep - No Escape quest');

 -- Dark Ranger
SET @CGUID := 399000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID AND @CGUID+1;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1360.47, 1063.27, 52.9999, 3.92421, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(@CGUID+1, 44632, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1428.24, 1032.64, 53.0474, 4.06429, 300, 0, 0, 1398, 0, 2, 0, 0, 0, '', 0);
 
 -- High Warlord Cromush
DELETE FROM `creature` WHERE `guid` = @CGUID+2;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+2, 44640, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1357.21, 1046.36, 52.7554, 4.46892, 300, 0, 0, 387450, 0, 2, 0, 0, 0, 'npc_high_warlord_cromush_44640', 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+2;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+2, 446400, 0, 0, 0, 1, 0, 0, 0, 0, 3, '80697');

 -- Orc Sea Dog
DELETE FROM `creature` WHERE `guid`= 321872;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(321872, 44913, 0, 130, 5386, 1, 0, 1, 169, 0, -1, 0, 0, 1045.19, 1567.43, 28.1601, 1.40073, 300, 0, 0, 1, 0, 0, 0, 0, 0, '', 0);

 -- Forest Ettin
DELETE FROM `creature` WHERE `guid`= @CGUID+6;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+6, 44367, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1189.52, 1573.09, 28.3514, 1.55021, 300, 0, 0, 819, 0, 2, 16777216, 0, 0, NULL, 0);

 -- Lady Sylvanas Windrunner
DELETE FROM `creature` WHERE `guid`= @CGUID+7;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+7, 44365, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1382.11, 1040.91, 54.3174, 3.73064, 300, 0, 0, 34356800, 94700, 0, 0, 134251328, 0, 'npc_silverpine_sylvanas_fhc', 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+7;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+7, 0, 0, 29257, 0, 2, 0, 0, 0, 0, 3, '83231');

 -- Agatha
DELETE FROM `creature_template_addon` WHERE `entry`=44951;
INSERT INTO `creature_template_addon` (`entry`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(44951, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, '85451');

UPDATE `creature_template` SET `unit_class`= 2, `VehicleId`= 1109, `HoverHeight`= 2.4, `ScriptName`= 'npc_silverpine_agatha_fenris' WHERE `entry` = 44951;

DELETE FROM `creature_text` WHERE `CreatureID`=44951;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44951, 0, 0, 'Death is only the beginning.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 1, 'Different master, same insatiable thirst for power.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 2, 'Do you feel it, $n? The darkness surrounds us.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 3, 'I long for the frozen wastes of Northrend.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 4, 'I sense doom in your future, $n.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 5, 'Life is meaningless. It is in the afterlife that we are truly tested.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 6, 'The warmth of this place sickens me.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 7, 'Through me the Banshee Queen sees all...', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 0, 8, 'Waste no time, $n. We mustn\'t keep the Banshee Queen waiting.', 12, 0, 100, 1, 0, 0, 0, 0, 0, ''),
(44951, 1, 0, 'Bow before your new master!', 14, 0, 100, 0, 0, 0, 0, 0, 0, ''),
(44951, 1, 1, 'Rise and destroy our enemies!', 14, 0, 100, 0, 0, 0, 0, 0, 0, ''),
(44951, 1, 2, 'Rise! Become Forsaken!', 14, 0, 100, 0, 0, 0, 0, 0, 0, ''),
(44951, 2, 0, 'This way, $n. We will take them by surprise.', 12, 0, 100, 0, 0, 0, 0, 0, 0, ''),
(44951, 3, 0, 'Run...', 12, 0, 100, 0, 0, 0, 0, 0, 0, ''),
(44951, 4, 0, 'RUN!', 12, 0, 100, 0, 0, 0, 0, 0, 0, '');

 -- Hillsbrad Refugee
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_hillsbrad_refugee' WHERE `entry` = 44954;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_hillsbrad_refugee' WHERE `entry` = 44966;
  
 -- Innkeepers
UPDATE `creature_template` SET `ScriptName`= 'npc_innkeeper' WHERE `subname` = 'Innkeeper';

 -- Fenris Keep stuff
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_fenris_stalker' WHERE `entry` = 45032;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_fenris_camera' WHERE `entry` = 45003;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_crowley_fenris' WHERE `entry` = 44989;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_odelic_fenris' WHERE `entry` = 44993;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_bartolo_fenris' WHERE `entry` = 44994;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_loremaster_fenris' WHERE `entry` = 44995;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_henry_fenris' WHERE `entry` = 44996;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_caretaker_fenris' WHERE `entry` = 44997;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_sophia_fenris' WHERE `entry` = 45002;

DELETE FROM `creature_addon` WHERE `guid` IN (321875, 321874, 321872, 321869, 321925, 321930, 321930, 321927, 321933, 321911, 321871, 321915);
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(321875, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321874, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321872, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321869, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321925, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321930, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321927, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321933, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321911, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186'),
(321871, 449132, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83832'),
(321915, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '84186');

DELETE FROM `creature_addon` WHERE `guid` IN (321878, 321870, 321934, 321924, 321916, 321474, @CGUID+6);
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(321878, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83829'),
(321870, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83829 42871'),
(321934, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83829'),
(321924, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83829'),
(321916, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '83829'),
(321474, 443670, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(@CGUID+6, 443671, 0, 0, 0, 1, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_addon` WHERE `guid`= 322043;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(322043, 449117, 0, 0, 0, 257, 0, 0, 0, 0, 3, '18950');

 -- Bat Handler Maggotbreath
DELETE FROM `creature` WHERE `guid`=@CGUID+3;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+3, 44825, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1423.08, 1016.46, 52.544, 2.90902, 300, 0, 0, 77490, 0, 0, 0, 0, 0, NULL, 0);
 
 -- Deathstalker Commander Belmont
DELETE FROM `creature` WHERE `guid`=@CGUID+4;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+4, 44789, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1415.07, 1010.24, 52.7315, 2.09687, 300, 0, 0, 2085, 0, 0, 0, 0, 0, NULL, 0);

 -- High Apothecary Shana T'veen
DELETE FROM `creature` WHERE `guid`=@CGUID+5;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+5, 44784, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1431.55, 1086.89, 60.4768, 3.81387, 300, 0, 0, 276, 590, 0, 0, 0, 0, NULL, 0);

 -- Worgen Renegade
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 44793;
UPDATE `creature` SET `spawndist`=5 WHERE `id` = 44793;

 -- Worg
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1765;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1765;

 -- Ferocious Grizzled Bear
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1778;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1778;

 -- Giant Rabid Bear
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1797;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1797;
 
 -- Rabid Dog
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1766;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1766;

 -- Vile Fin Oracle
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1908;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1908;

 -- Vile Fin Tidehunter
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1768;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1768;

  -- Skitterweb Lurker
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 1781;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 1781;

 -- Bloodfang Scavenger
UPDATE `creature` SET `MovementType`=1 WHERE `id` = 44547;
UPDATE `creature` SET `spawndist`=10 WHERE `id` = 44547;

UPDATE `creature` SET `spawndist`= 0 WHERE `guid` = 321893;
DELETE FROM `creature_addon` WHERE `guid`= 321893;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(321893, 445470, 0, 0, 0, 1, 0, 0, 0, 0, 0, '');

UPDATE `creature` SET `spawndist`= 0 WHERE `guid` = 321729;
DELETE FROM `creature_addon` WHERE `guid`= 321729;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(321729, 445471, 0, 0, 0, 1, 0, 0, 0, 0, 0, '');

UPDATE `creature` SET `spawndist`= 0 WHERE `guid` = 321895;
DELETE FROM `creature_addon` WHERE `guid`= 321895;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(321895, 445472, 0, 0, 0, 1, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_formations` WHERE `MemberGUID`=322043;
DELETE FROM `creature_formations` WHERE `MemberGUID`=322042;
INSERT INTO `creature_formations` (`LeaderGUID`, `MemberGUID`, `FollowDistance`, `FollowAngle`, `GroupAI`, `InversionPoint1`, `InversionPoint2`) VALUES
(322043, 322043, 0, 0, 515, -1, -1),
(322043, 322042, 3.5, 270, 515, -1, -1);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=44365 AND `ID`=1;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(44365, 1, 2179, 0, 42775, 15595);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=44640 AND `ID`=1;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(44640, 1, 47031, 0, 0, 18019);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=44791 AND `ID` BETWEEN 1 AND 4;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(44791, 1, 1481, 0, 0, 0),
(44791, 2, 11121, 9661, 0, 0),
(44791, 3, 15273, 0, 0, 0),
(44791, 4, 36502, 0, 0, 0);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=44792 AND `ID` BETWEEN 1 AND 4;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(44792, 1, 1481, 0, 0, 0),
(44792, 2, 11121, 9661, 0, 0),
(44792, 3, 15273, 0, 0, 0),
(44792, 4, 36502, 0, 0, 0);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=44632 AND `ID`=1;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(44632, 1, 0, 0, 34263, 18019);

UPDATE creature_template SET `npcflag` = 16777216, `ScriptName` = 'npc_silverpine_armoire_click' WHERE `entry` = 44894;
UPDATE creature_template SET `unit_flags` = 33554440, `VehicleId` = 1055, `ScriptName` = 'npc_silverpine_armoire' WHERE `entry` = 44893;
UPDATE creature_template SET `unit_flags` = 2, `unit_flags2` = 16384, `VehicleId` = 1059, `ScriptName` = 'npc_silverpine_packleader_ivar_bloodfang_exhanguinate' WHERE `entry` = 44884;
UPDATE creature_template SET `unit_flags` = 2, `unit_flags2` = 16384, `ScriptName` = 'npc_silverpine_lord_darius_crowley_exhanguinate' WHERE `entry` = 44883;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_deathstalker_rane_yorick' WHERE `entry` = 44882;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_bat_handler_maggotbreath'  WHERE `entry` = 44825;
UPDATE creature_template SET `spell1` = 83573, `VehicleId` = 1051, `ScriptName` = 'npc_silverpine_forsaken_bat'  WHERE `entry` = 44821;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_worgen_renegade'  WHERE `entry` = 44793;
UPDATE creature_template SET `spell1` = 19983, `spell2` = 8078, `ScriptName` = 'npc_silverpine_forsaken_trooper'  WHERE `entry` = 44792;
UPDATE creature_template SET `spell1` = 19983, `spell2` = 8078, `ScriptName` = 'npc_silverpine_forsaken_trooper'  WHERE `entry` = 44791;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_horde_coffin_hauler'  WHERE `entry` = 44764;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_horde_hauler'  WHERE `entry` = 44731;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_grand_executor_mortuus'  WHERE `entry` = 44615;
UPDATE creature_template SET `unit_flags2` = 2107392, `VehicleId` = 1109  WHERE `entry` = 44610;
UPDATE creature_template SET `unit_flags2` = 2099200, `HoverHeight` = 2.8  WHERE `entry` = 44609;
UPDATE creature_template SET `unit_flags` = 768, `dynamicflags` = 32, `ScriptName` = 'npc_silverpine_fallen_human'  WHERE `entry` = 44593;
UPDATE creature_template SET `unit_flags` = 768, `dynamicflags` = 32, `ScriptName` = 'npc_silverpine_fallen_human'  WHERE `entry` = 44592;
UPDATE creature_template SET `npcflag`= 16777216 WHERE `entry` = 44915;
UPDATE creature_template SET `unit_flags`= 33288 WHERE `entry` = 44914;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_mutant_bush_chicken'  WHERE `entry` = 44935;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_forest_ettin'  WHERE `entry` = 44367;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_salty_rocka'  WHERE `entry` = 45498;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_apothecary_wormcrud'  WHERE `entry` = 44912;

UPDATE creature_template SET `ScriptName` = 'npc_silverpine_orc_sea_pup' WHERE `entry` = 44914;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_admiral_hatchet' WHERE `entry` = 44916;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_orc_sea_dog' WHERE `entry` = 44942;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_webbed_victim_skitterweb' WHERE `entry` = 44941;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_skitterweb_matriarch', `AIName` = '', `flags_extra` = 512 WHERE `entry` = 44906;

DELETE FROM `creature_template_addon` WHERE `entry` IN (44608, 44609, 44610, 44615, 44592, 44593, 44825, 44821, 44882, 44893, 44894, 44884, 44883, 44632, 44764, 44731);
INSERT INTO `creature_template_addon` (`entry`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(44608, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, ''),
(44609, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, ''),
(44610, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, ''),
(44615, 0, 0, 0, 0, 257, 0, 0, 0, 0, 3, '83231'),
(44592, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, '80636'),
(44593, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, '80636'),
(44825, 0, 0, 0, 0, 257, 0, 0, 0, 0, 0, ''),
(44821, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44882, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44893, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44894, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44884, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44883, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, ''),
(44632, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, ''),
(44764, 0, 0, 0, 0, 1, 0, 0, 0, 0, 4, ''),
(44731, 0, 0, 0, 0, 1, 0, 0, 0, 0, 4, '');

DELETE FROM `creature_template_movement` WHERE `CreatureId` IN (44610, 44608, 44609);
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`, `InteractionPauseTimer`) VALUES
(44610, 1, 0, 1, 0, 0, NULL),
(44608, 1, 0, 1, 0, 0, NULL),
(44609, 1, 0, 1, 0, 0, NULL);

DELETE FROM `creature_template_movement` WHERE `CreatureId`= 44908;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`, `InteractionPauseTimer`) VALUES 
(44908, 1, 0, 1, 0, 0, NULL);

DELETE FROM `creature_text` WHERE `CreatureID`=44365 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 10;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44365, 0, 0, 'Where is that ogre-headed buffoon?', 12, 0, 100, 6, 0, 20459, 0, 44695, 0, 'VO_QE_SP_Sylvanas_SPEvent01'),
(44365, 1, 0, 'Ah, speak of the devil...', 12, 0, 100, 1, 0, 20460, 0, 44696, 0, 'VO_QE_SP_Sylvanas_SPEvent02'),
(44365, 2, 0, 'Warchief, so glad you could make it.', 12, 0, 100, 1, 0, 20461, 0, 44701, 0, 'VO_QE_SP_Sylvanas_SPEvent03'),
(44365, 3, 0, 'With the death of the Lich King, many of the more intelligent Scourge became... unemployed. Those \'fiends,\' as you so delicately put it, are called val\'kyr. They are under my command now...', 12, 0, 100, 1, 0, 20462, 0, 44702, 0, 'VO_QE_SP_Sylvanas_SPEvent04'),
(44365, 4, 0, '...and they are part of the reason that I asked to see you.', 12, 0, 100, 1, 0, 20463, 0, 44703, 0, 'VO_QE_SP_Sylvanas_SPEvent05'),
(44365, 5, 0, 'Very well, Warchief. I have solved the plight of the Forsaken!', 12, 0, 100, 5, 0, 20464, 0, 44705, 0, 'VO_QE_SP_Sylvanas_SPEvent06'),
(44365, 6, 0, 'As a race, we Forsaken are unable to procreate.', 12, 0, 100, 274, 0, 20465, 0, 44706, 0, 'VO_QE_SP_Sylvanas_SPEvent07'),
(44365, 7, 0, 'With the aid of the val\'kyr, we are now able to take the corpses of the fallen and create new Forsaken.', 12, 0, 100, 1, 0, 20466, 0, 44707, 0, 'VO_QE_SP_Sylvanas_SPEvent08'),
(44365, 8, 0, 'Agatha, show the Warchief!', 12, 0, 100, 5, 0, 20467, 0, 44709, 0, 'VO_QE_SP_Sylvanas_SPEvent09'),
(44365, 9, 0, 'Warchief, without these new Forsaken my people would die out... Our hold upon Gilneas and northern Lordaeron would crumble.', 12, 0, 100, 1, 0, 20468, 0, 44715, 0, 'VO_QE_SP_Sylvanas_SPEvent10'),
(44365, 10, 0, 'Isn\'t it obvious, Warchief? I serve the Horde.', 12, 0, 100, 66, 0, 20469, 0, 44718, 0, 'VO_QE_SP_Sylvanas_SPEvent11');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44615 AND `GroupID`=0 AND `ID`=0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44615, 0, 0, 'Move, Forsaken!', 12, 0, 100, 25, 2, 0, 0, 0, 0, '');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44640 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44640, 0, 0, 'ABBERATION!', 12, 0, 100, 5, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01'),
(44640, 1, 0, 'As you command, Warchief.', 12, 0, 100, 1, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44629 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 8;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44629, 0, 0, 'This better be important, Sylvanas. You know how I detest this place and its foul stench. Why have you called for me?', 12, 0, 100, 1, 0, 20496, 0, 0, 0, 'VO_QE_Garrosh_SPEvent01'),
(44629, 1, 0, 'And more importantly, what are those Scourge fiends doing here?', 12, 0, 100, 25, 0, 20497, 0, 0, 0, 'VO_QE_Garrosh_SPEvent02'),
(44629, 2, 0, 'Get on with it, Sylvanas.', 12, 0, 100, 1, 0, 20498, 0, 0, 0, 'VO_QE_Garrosh_SPEvent03'),
(44629, 3, 0, 'What you have done here, Sylvanas... it goes against the laws of nature. Disgusting is the only word I have to describe it.', 12, 0, 100, 1, 0, 20499, 0, 0, 0, 'VO_QE_Garrosh_SPEvent04'),
(44629, 4, 0, 'Have you given any thought to what this means, Sylvanas?', 12, 0, 100, 1, 0, 20500, 0, 0, 0, 'VO_QE_Garrosh_SPEvent05'),
(44629, 5, 0, 'What difference is there between you and the Lich King now?', 12, 0, 100, 1, 0, 20501, 0, 0, 0, 'VO_QE_Garrosh_SPEvent06'),
(44629, 6, 0, 'Watch your clever mouth, bitch.', 12, 0, 100, 25, 0, 20502, 0, 0, 0, 'VO_QE_Garrosh_SPEvent07'),
(44629, 7, 0, 'Cromush, you stay behind and make sure the Banshee Queen is well "guarded." I will be expecting a full report when next we meet.', 12, 0, 100, 1, 0, 20503, 0, 0, 0, 'VO_QE_Garrosh_SPEvent08'),
(44629, 8, 0, 'Remember, Sylvanas, eventually we all have to stand before our maker and face judgment. Your day may come sooner than others...', 12, 0, 100, 1, 0, 20504, 0, 0, 0, 'VO_QE_Garrosh_SPEvent09');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44791 AND `GroupID`=0 AND `ID` BETWEEN 0 AND 4;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44791, 0, 0, 'Thank you, hero!', 12, 1, 100, 71, 0, 0, 0, 44839, 0, ''),
(44791, 0, 1, 'Thanks. That beast was a handful!', 12, 1, 100, 1, 0, 0, 0, 44835, 0, ''),
(44791, 0, 2, 'That beast came from nowhere!', 12, 1, 100, 5, 0, 0, 0, 44836, 0, ''),
(44791, 0, 3, 'They won\'t take our land without a fight!', 12, 1, 100, 15, 0, 0, 0, 44838, 0, ''),
(44791, 0, 4, 'Worgen filth! I spit on your corpse!', 12, 1, 100, 14, 0, 0, 0, 44837, 0, '');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44821 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44821, 0, 0, 'Eliminate all murlocs by using the Blight Concoction!$B$B|r|TInterface\\\\Icons\\\\INV_ALCHEMY_POTION_05.BLP:24|t', 42, 1, 100, 0, 0, 0, 0, 44902, 0, 'Forsaken Bat to Player'),
(44821, 1, 0, 'Returning to Forsaken High Command!$B$B|r|TInterface\\\\Icons\\\\ACHIEVEMENT_ZONE_SILVERPINE_01.BLP:24|t', 42, 1, 100, 0, 0, 0, 0, 44903, 0, 'Forsaken Bat to Player');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44882 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 2;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44882, 0, 0, 'We haven\'t got much time. Crowly will be arriving shortly.', 12, 0, 100, 5, 0, 0, 0, 44951, 0, NULL),
(44882, 1, 0, 'THERE! Hide in the armoire! I\'ll hide in the shadow next to you.', 12, 0, 100, 25, 0, 0, 0, 44955, 0, NULL),
(44882, 2, 0, 'I live... and die... for the Banshee Queen.', 12, 0, 100, 0, 0, 0, 0, 44971, 0, NULL);
 
DELETE FROM `creature_text` WHERE `CreatureID`=44825 AND `GroupID`=0 AND `ID`=0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44825, 0, 0, 'You better bring that bat back in one piece, $n!', 12, 0, 100, 25, 0, 0, 0, 44906, 0, '');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44883 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 3;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44883, 0, 0, 'Have you given any more thought to my proposal, Ivar?', 12, 0, 100, 6, 0, 0, 0, 44957, 0, NULL),
(44883, 1, 0, 'If we\'re to win this war we will need your help. Our packs must unite! The Forsaken will destroy us otherwise.', 12, 0, 100, 396, 0, 0, 0, 44960, 0, NULL),
(44883, 2, 0, 'You are the alpha male, Ivar. The rest of the ferals in Silverpine will do as you command.', 12, 0, 100, 397, 0, 0, 0, 44961, 0, NULL),
(44883, 3, 0, 'So will you help?', 12, 0, 100, 6, 0, 0, 0, 44965, 0, NULL);
 
DELETE FROM `creature_text` WHERE `CreatureID`=44884 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 10;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44884, 0, 0, 'Why trust you now, Crowley? You abandoned us. Left us to die.', 12, 0, 100, 6, 0, 0, 0, 44959, 0, NULL),
(44884, 1, 0, 'I don\'t care about your war, Crowley.', 12, 0, 100, 274, 0, 0, 0, 44962, 0, NULL),
(44884, 2, 0, 'But...', 12, 0, 100, 1, 0, 0, 0, 0, 0, NULL),
(44884, 3, 0, 'I have seen firsthand what the Forsaken are capable of doing. It is true. They hunt us... slaughter the defenseless.', 12, 0, 100, 1, 0, 0, 0, 44964, 0, NULL),
(44884, 4, 0, 'Aye, I will gather my pack... we...', 12, 0, 100, 1, 0, 0, 0, 44966, 0, NULL),
(44884, 5, 0, 'Packleader Ivar Bloodfang sniffs the air.', 16, 0, 100, 479, 0, 0, 0, 44967, 0, NULL),
(44884, 6, 0, 'It would appear that we are being watched, Crowley. You have grown soft... Likely Greymane\'s fault.', 12, 0, 100, 1, 0, 0, 0, 44968, 0, NULL),
(44884, 7, 0, 'Treacherous little pup!', 12, 0, 100, 15, 0, 0, 0, 44969, 0, NULL),
(44884, 8, 0, 'What say you now, spy?', 12, 0, 100, 0, 0, 0, 0, 44970, 0, ''),
(44884, 9, 0, 'I was hoping you\'d say that...', 12, 0, 100, 0, 0, 0, 0, 44972, 0, ''),
(44884, 10, 0, 'I will prepare the pack. It will take some time to gather them all, but we will join... for now.', 12, 0, 100, 1, 0, 0, 0, 44973, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44942;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44942, 0, 0, 'We gotta find the matriarch of this operation and end her!', 12, 1, 100, 5, 0, 0, 0, 45079, 0, 'Orc Sea Dog to Player'),
(44942, 0, 1, 'Thank you, friend. Let\'s kill us some overgrown... spiders. Sounded better in my head.', 12, 1, 100, 5, 0, 0, 0, 45078, 0, 'Orc Sea Dog to Player'),
(44942, 0, 2, 'I need a drink!', 12, 1, 100, 5, 0, 0, 0, 45080, 0, 'Orc Sea Dog to Player'),
(44942, 0, 3, 'Welcome to the party, $g pal:lady;!', 12, 1, 100, 5, 0, 0, 0, 45077, 0, 'Orc Sea Dog to Player');

DELETE FROM `creature_text` WHERE `CreatureID`=45498;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(45498, 0, 0, 'Hey over there! Hey! Hey! Over there! How\'sh everything going Gororgararar?', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 00'),
(45498, 1, 0, 'HA! Shows how much you know. I left the critter back on the third ship!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 01'),
(45498, 2, 0, 'Yoo hoo! "Shmaltzy!" Hey, why come your shupplies... Whe... Oh... Um... You got lots of boxes to keep track of...', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 10'),
(45498, 3, 0, 'I didn\'t like that pet anyways. Beshides... people stop by all the time and give me their pets. If I lost one, it doesn\'t matter. I\'ve got dozens more to lose!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 11'),
(45498, 4, 0, 'Hey over there! Hey! Hey! Over there! How\'sh everything going Gororgararar?', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 20'),
(45498, 5, 0, 'My pet! Oh no! Uh... I am shure that pet is around here somewhere...', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 21'),
(45498, 6, 0, 'Hey "Salty" when are you going to get a new name? Thish town ain\'t big \'nough for two of ussh...', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 30'),
(45498, 7, 0, 'Wait... whee is my pet? It\'s gone! No need to panic... Just need a quick drink and I can figure out where he went to.', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 31'),
(45498, 8, 0, '"Malty!" Hey, "Malty!" I thought you were going to get us shome more booze! What\'sh the hold up?', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 40'),
(45498, 9, 0, 'I didn\'t like that pet anyways. Beshides... people stop by all the time and give me their pets. If I lost one, it doesn\'t matter. I\'ve got dozens more to lose!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Rocka 41');

DELETE FROM `creature_text` WHERE `CreatureID`=45497;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(45497, 0, 0, 'Hey, shtop bothering me. You\'re a dishgrace to us all. You can\'t even hold onto your own petsh!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Gorgar 00'),
(45497, 1, 0, 'Maybe you should worry about your pet and not me! I\'m taking care of my bishinesh jusht fine!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Gorgar 10'),
(45497, 2, 0, 'Hey, shtop bothering me. You\'re a dishgrace to us all. You can\'t even hold onto your own petsh!', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Gorgar 20'), 
(45497, 3, 0, 'It\'s "Salty!" I got here firsht! I got off the boat and onto land firsht! I called the name "Salty." Hey... Where\'s your pet at?', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Gorgar 30'),
(45497, 4, 0, '"Shalty." That\'s my name! Your name should be "I can\'t hold onto petsh." I can\'t believe anyone would trusht you with a beasht.', 14, 1, 100, 5, 0, 0, 0, 0, 0, 'Gorgar 40');

DELETE FROM `creature_text` WHERE `CreatureID`=44913;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44913, 0, 0, 'What\'sh wit the huuuuge bush chick-a-doodle?', 12, 1, 100, 1, 0, 0, 0, 0, 0, ''),
(44913, 1, 0, 'It\'sh makin\' me hungry... why\'s it shooo big? You shome kind of idiot or shomething? Makin\' a chicken so big... Who doesh that?', 12, 1, 100, 1, 0, 0, 0, 0, 0, ''),
(44913, 2, 0, 'Forshaken do... that\'sh who... It\'sh sho big... Look at it\'sh big stupid neck. I jusht want to choke it to death!', 12, 1, 100, 1, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44912;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44912, 0, 0, 'Touch my chicken and I will kill you in your sleep. Am I understood?', 12, 1, 100, 1, 0, 0, 0, 0, 0, NULL);

DELETE FROM `creature_text` WHERE `CreatureID`=44914;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44914, 0, 0, 'Where to going, captain?', 12, 1, 100, 66, 0, 0, 0, 44990, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 1, 0, 'OOF! Maybe trow little softer, captain?', 12, 1, 100, 0, 0, 0, 0, 44992, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 2, 0, 'Heavy... no can see notin.', 12, 1, 100, 0, 0, 0, 0, 44993, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 3, 0, 'Captain, pup need help!', 12, 1, 100, 0, 0, 0, 0, 44996, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 4, 0, 'Please, captain, I can put box down?', 12, 1, 100, 0, 0, 0, 0, 44999, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 5, 0, 'NO MORE! DIS IS MAXI... MAXIMO... ME... DIS TOO MUCH!', 12, 1, 100, 0, 0, 0, 0, 45003, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 6, 0, 'Where to put? Dey falling! HELP!', 12, 1, 100, 0, 0, 0, 0, 45025, 0, 'Orc Sea Pup to Orc Crate'),
(44914, 7, 0, 'OOF!', 12, 1, 100, 0, 0, 0, 0, 0, 0, 'Orc Sea Pup to Orc Crate');

DELETE FROM `creature_sparring_template` WHERE `CreatureID` IN (44793, 44791, 44792);
INSERT INTO `creature_sparring_template` (`CreatureID`, `HealthLimitPct`) VALUES
(44793, 85),
(44791, 85),
(44792, 85);

UPDATE `gameobject_template` SET `ScriptName`='go_silverpine_abandoned_outhouse' WHERE `entry`=205143;

DELETE FROM `gossip_menu_option` WHERE `MenuId`=6944 AND `OptionIndex`=1;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) VALUES
(6944, 1, 2, 'I need to take a bat to the Dawning Isles.', 0, 1, 1, 15595);

UPDATE `gossip_menu_option` SET `OptionNpcflag`= 2 WHERE `MenuId` = 11901;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=44894 AND `spell_id`=83756;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(44894, 83756, 1, 0);

DELETE FROM `creature_questender` WHERE `id`=44615 AND `quest` IN (26964, 26965, 26989);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44615, 26964),
(44615, 26965),
(44615, 26989);

DELETE FROM `creature_questender` WHERE `id`=44778 AND `quest`=26992;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44778, 26992);

DELETE FROM `creature_questender` WHERE `id`=44784 AND `quest` IN (26995, 26998);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44784, 26995),
(44784, 26998);

DELETE FROM `creature_questender` WHERE `id`=44789 AND `quest`=27045;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44789, 27045);

DELETE FROM `creature_questender` WHERE `id`=44365 AND `quest`=27056;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44365, 27056);

DELETE FROM `creature_questender` WHERE `id`=44916 AND `quest`=27065;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(44916, 27065);

DELETE FROM `creature_queststarter` WHERE `id`=1515 AND `quest`=26964;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(1515, 26964);

DELETE FROM `creature_queststarter` WHERE `id`=44615 AND `quest` IN (26965, 26989);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44615, 26965),
(44615, 26989);

DELETE FROM `creature_queststarter` WHERE `id`=44778 AND `quest`=26992;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44778, 26992);

DELETE FROM `creature_queststarter` WHERE `id`=44784 AND `quest` IN (26995, 26998);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44784, 26995),
(44784, 26998);

DELETE FROM `creature_queststarter` WHERE `id`=44789 AND `quest`=27056;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44789, 27056);

DELETE FROM `creature_queststarter` WHERE `id`=44365 AND `quest`=27065;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44365, 27065);

UPDATE gossip_menu_option SET `OptionNpcflag` = '65536', `OptionType` = '8' WHERE `MenuId` = 12025 AND `OptionIndex` = 0;
UPDATE gossip_menu_option SET `OptionNpcflag` = '128', `OptionType` = '3' WHERE `MenuId` = 12025 AND `OptionIndex` = 1;

DELETE FROM `npc_vendor` WHERE `entry`=45496;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`, `VerifiedBuild`) VALUES 
(45496, 1, 159, 0, 0, 0, 1, 0, 0),
(45496, 2, 1179, 0, 0, 0, 1, 0, 0),
(45496, 3, 4540, 0, 0, 0, 1, 0, 0),
(45496, 4, 4541, 0, 0, 0, 1, 0, 0),
(45496, 5, 4496, 0, 0, 0, 1, 0, 0),
(45496, 6, 4498, 0, 0, 0, 1, 0, 0),
(45496, 7, 4470, 0, 0, 0, 1, 0, 0),
(45496, 8, 5042, 0, 0, 0, 1, 0, 0),
(45496, 9, 1205, 0, 0, 0, 1, 0, 0), 
(45496, 10, 4542, 0, 0, 0, 1, 0, 0),
(45496, 11, 5048, 0, 0, 0, 1, 0, 0);

DELETE FROM `npc_vendor` WHERE `entry`=45497;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`, `VerifiedBuild`) VALUES 
(45497, 1, 159, 0, 0, 0, 1, 0, 0),
(45497, 2, 1179, 0, 0, 0, 1, 0, 0),
(45497, 3, 4540, 0, 0, 0, 1, 0, 0),
(45497, 4, 4541, 0, 0, 0, 1, 0, 0),
(45497, 5, 4496, 0, 0, 0, 1, 0, 0),
(45497, 6, 4498, 0, 0, 0, 1, 0, 0),
(45497, 7, 4470, 0, 0, 0, 1, 0, 0),
(45497, 8, 5042, 0, 0, 0, 1, 0, 0),
(45497, 9, 7005, 0, 0, 0, 1, 0, 0), 
(45497, 10, 2901, 0, 0, 0, 1, 0, 0),
(45497, 11, 5956, 0, 0, 0, 1, 0, 0),
(45497, 12, 39505, 0, 0, 0, 1, 0, 0),
(45497, 13, 6256, 0, 0, 0, 1, 0, 0),
(45497, 14, 6217, 0, 0, 0, 1, 0, 0),
(45497, 15, 3371, 0, 0, 0, 1, 0, 0),
(45497, 16, 2320, 0, 0, 0, 1, 0, 0),
(45497, 17, 2321, 0, 0, 0, 1, 0, 0),
(45497, 18, 2678, 0, 0, 0, 1, 0, 0),
(45497, 19, 2880, 0, 0, 0, 1, 0, 0),
(45497, 20, 4289, 0, 0, 0, 1, 0, 0),
(45497, 21, 6529, 0, 0, 0, 1, 0, 0),
(45497, 22, 6530, 0, 0, 0, 1, 0, 0),
(45497, 23, 2605, 0, 0, 0, 1, 0, 0),
(45497, 24, 6260, 0, 0, 0, 1, 0, 0),
(45497, 25, 2604, 0, 0, 0, 1, 0, 0),
(45497, 26, 2324, 0, 0, 0, 1, 0, 0),
(45497, 27, 39354, 0, 0, 0, 1, 0, 0),
(45497, 28, 20815, 0, 0, 0, 1, 0, 0);

UPDATE quest_template_addon SET `PrevQuestID` = '27098', `NextQuestID` = '27232' WHERE `ID` = 27231;
UPDATE quest_template_addon SET `PrevQuestID` = '27098' WHERE `ID` = 27226;
UPDATE quest_template_addon SET `PrevQuestID` = '27094' WHERE `ID` = 27096;
UPDATE quest_template_addon SET `PrevQuestID` = '27195' WHERE `ID` = 27290;

 -- Bloodfang Scavenger
SET @ENTRY := 44547;
DELETE FROM `smart_scripts` WHERE `entryOrGuid` = @ENTRY AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = "SmartAI", `ScriptName` = "" WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 0, 0, 0, 100, 0, 4000, 6000, 25000, 30000, 11, 84308, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Every 25 - 30 seconds (4 - 6s initially) - Self: Cast spell Furious Howl (84308) on Self"),
(@ENTRY, 0, 1, 0, 9, 0, 100, 0, 0, 30, 14000, 15000, 11, 78509, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "When victim in range 0 - 30 yards (check every 14000 - 15000 ms) - Self: Cast spell Torch Toss (78509) on Victim"),
(@ENTRY, 0, 2, 0, 1, 0, 100, 0, 4000, 25000, 40000, 70000, 5, 15, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Every 40 - 70 seconds (4 - 25s initially) - Self: Play emote ONESHOT_ROAR(DNR) (15)"),
(@ENTRY, 0, 3, 0, 1, 0, 50, 0, 4000, 14000, 25000, 55000, 11, 83860, 0, 0, 1, 0, 0, 11, 44920, 20, 0, 0, 0, 0, 0, "Every 25 - 55 seconds (4 - 14s initially) - Self: Cast spell Toss Torch (83860) on random 1 Creature North Tide's Invisible Stalker (44920) in 20 yd"),
(@ENTRY, 0, 4, 2, 0, 0, 100, 0, 3, 0, 0, 0, 4, 9036, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On action[3] - Self: Play direct sound WolfHowl (9036) to every player in visibility range of Self");

 -- North Tide's Invisible Stalker
SET @ENTRY := 44920;
DELETE FROM `smart_scripts` WHERE `entryOrGuid` = @ENTRY AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = "SmartAI", `ScriptName` = "" WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 0, 8, 0, 100, 0, 83860, 0, 0, 0, 11, 83859, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On spell Toss Torch (83860) hit - Self: Cast spell North Tide's Fire (83859) on random 1 Self");

 -- Detect: Quest Invis Zone 3
DELETE FROM `spell_area` WHERE `spell`=83989 AND `area`=5369 AND `quest_start`=26964 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES
(83989, 5369, 26964, 27097, 0, 0, 2, 3, 74, 11);

 -- Detect: Quest Invis Zone 1
DELETE FROM `spell_area` WHERE `spell`=83232 AND `area`=5369 AND `quest_start`=26964 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES
(83232, 5369, 26964, 27098, 0, 0, 2, 3, 66, 1);

 -- Detect: Quest Invis Zone 5
DELETE FROM `spell_area` WHERE `spell`=84241 AND `area`=5386 AND `quest_start`=27065 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
(84241, 5386, 27065, 27098, 0, 0, 2, 3, 74, 1);

DELETE FROM `spell_script_names` WHERE `spell_id`=83173 AND `ScriptName`='spell_silverpine_raise_forsaken_83173';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83173, 'spell_silverpine_raise_forsaken_83173');

DELETE FROM `spell_script_names` WHERE `spell_id`=83149 AND `ScriptName`='spell_silverpine_forsaken_trooper_masterscript';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83149, 'spell_silverpine_forsaken_trooper_masterscript');

DELETE FROM `spell_script_names` WHERE `spell_id`=80365 AND `ScriptName`='spell_silverpine_flurry_of_claws';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80365, 'spell_silverpine_flurry_of_claws');

DELETE FROM `spell_script_names` WHERE `spell_id`=83865 AND `ScriptName`='spell_silverpine_sea_pup_trigger';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83865, 'spell_silverpine_sea_pup_trigger');

DELETE FROM `spell_script_names` WHERE `spell_id`=83838 AND `ScriptName`='spell_silverpine_pick_up_orc_crate';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83838, 'spell_silverpine_pick_up_orc_crate');

DELETE FROM `spell_script_names` WHERE `spell_id`=83978 AND `ScriptName`='spell_silverpine_agatha_broadcast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83978, 'spell_silverpine_agatha_broadcast');

DELETE FROM `vehicle_template_accessory` WHERE `entry`=44764 AND `seat_id` BETWEEN 0 AND 5;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(44764, 44734, 0, 1, 'Sitting on top - Horde Engineer', 8, 30000),
(44764, 44737, 1, 1, 'Subdued Forest Ettin', 8, 30000),
(44764, 0, 2, 1, 'Sitting on floor (top left) - Player side', 8, 30000),
(44764, 44766, 3, 1, 'On the bottom - Coffin', 8, 30000),
(44764, 44766, 4, 1, 'On the top - Coffin', 8, 30000),
(44764, 44766, 5, 1, 'On the middle - Coffin', 8, 30000);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=44731 AND `seat_id` BETWEEN 0 AND 7;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(44731, 44734, 0, 1, 'Sitting on top - Horde Engineer', 8, 30000),
(44731, 44737, 1, 1, 'Subdued Forest Ettin', 8, 30000),
(44731, 0, 2, 1, 'Sitting on floor (top left) - Player side', 8, 30000),
(44731, 44733, 3, 1, 'Sitting on floor (top right) - Female Trooper', 8, 30000),
(44731, 44732, 4, 1, 'Sitting on floor (bottom left) - Male Trooper', 8, 30000),
(44731, 44733, 5, 1, 'Sitting on floor (bottom right) - Female Trooper', 8, 30000),
(44731, 44733, 6, 1, 'Sitting on shelf (bottom left) - Female trooper', 8, 30000),
(44731, 44732, 7, 1, 'Sitting on shelf (bottom right) - Male trooper', 8, 30000);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=45120;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(45120, 44913, 0, 0, 'Orc Demolisher - Orc Sea Dog', 6, 30000);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=45198;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(45198, 46483, 1, 1, 'Forsaken Catapult', 8, 30000);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=45282;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(45282, 45374, 0, 0, 'Forsaken Catapult', 6, 30000);

DELETE FROM `vehicle_seat_addon` WHERE `SeatEntry`=8420;
INSERT INTO `vehicle_seat_addon` (`SeatEntry`, `SeatOffsetX`, `SeatOffsetY`, `SeatOffsetZ`, `SeatOffsetO`, `ExitParamX`, `ExitParamY`, `ExitParamZ`, `ExitParamO`, `ExitParamValue`) VALUES 
(8420, 0, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448820 AND `point` BETWEEN 1 AND 16;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448820, 1, 1299.87, 1189.73, 52.1171, 2.45043, 3, 0, 1, 0, 0, 0, 0),
(448820, 2, 1290.53, 1196.47, 52.4176, 2.49362, 3, 0, 1, 0, 0, 0, 0),
(448820, 3, 1289.97, 1200.79, 52.6583, 1.70037, 3, 0, 1, 0, 0, 0, 0),
(448820, 4, 1290.12, 1205.71, 52.7164, 1.53936, 3, 0, 1, 0, 0, 0, 0),
(448820, 5, 1293.6, 1207.07, 53.5343, 0.373047, 3, 0, 1, 0, 0, 0, 0),
(448820, 6, 1299.12, 1207.07, 53.7466, 0.325923, 3, 0, 1, 0, 0, 0, 0),
(448820, 7, 1303.74, 1210.4, 53.7466, 0.93068, 3, 0, 1, 0, 0, 0, 0),
(448820, 8, 1304.21, 1212.67, 53.7466, 1.36265, 3, 0, 1, 0, 0, 0, 0),
(448820, 9, 1303.62, 1219.87, 53.7466, 1.65325, 3, 0, 1, 0, 0, 0, 0),
(448820, 10, 1300.44, 1222.61, 53.7466, 2.43864, 3, 0, 1, 0, 0, 0, 0),
(448820, 11, 1297.95, 1220.37, 53.7466, 3.94268, 3, 0, 1, 0, 0, 0, 0),
(448820, 12, 1297.62, 1212.44, 58.4808, 4.06835, 3, 0, 1, 0, 0, 0, 0),
(448820, 13, 1297.64, 1210.12, 58.4751, 4.72023, 3, 0, 1, 0, 0, 0, 0),
(448820, 14, 1301.66, 1206.27, 58.4941, 5.53704, 3, 0, 1, 0, 0, 0, 0),
(448820, 15, 1311.44, 1206.49, 58.5103, 6.2596, 3, 0, 1, 0, 0, 0, 0),
(448820, 16, 1312.64, 1208.44, 58.5121, 1.01707, 3, 0, 1, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448830 AND `point` BETWEEN 0 AND 4;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448830, 4, 1313.48, 1206.09, 58.5119, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 3, 1305.32, 1206.43, 58.5126, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 2, 1302.33, 1206.44, 58.499, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 1, 1300.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 0, 1299.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448840 AND `point` BETWEEN 0 AND 3;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448840, 3, 1308.4, 1206.28, 58.5109, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 2, 1302.31, 1206.61, 58.4984, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 1, 1297.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 0, 1296.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448210 AND `point` BETWEEN 0 AND 6;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448210, 0, 1379.67, 1021.7, 59.9887, 0, 0, 0, 1, 3, 0, 0, 0),
(448210, 1, 1249.74, 1001.91, 66.9924, 0, 0, 0, 1, 1, 0, 0, 0),
(448210, 2, 1212.13, 981.977, 65.3913, 0, 0, 0, 1, 1, 0, 0, 0),
(448210, 3, 1180.25, 927.461, 60.3111, 0, 0, 0, 1, 1, 0, 0, 0),
(448210, 4, 1149.33, 766.078, 57.3167, 0, 0, 0, 1, 1, 0, 0, 0),
(448210, 5, 1192.43, 475.569, 62.4255, 0, 0, 0, 1, 1, 0, 0, 0),
(448210, 6, 1204.81, 329.134, 50.3452, 0, 0, 0, 1, 1, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448211 AND `point` BETWEEN 0 AND 17;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448211, 0, 1197.2, 295.636, 47.8965, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 1, 1119.13, 234.839, 46.6181, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 2, 989.85, 180.368, 52.3217, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 3, 904.214, 168.247, 53.1545, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 4, 736.523, 178.213, 52.474, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 5, 721.224, 213.371, 53.8464, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 6, 720.123, 256.447, 55.9538, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 7, 790.195, 289.27, 52.1988, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 8, 1058.38, 312.523, 51.4779, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 9, 1078.93, 313.845, 51.2958, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 10, 1174.25, 330.96, 51.4075, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 11, 1186.85, 345.036, 50.4988, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 12, 1209.86, 406.546, 54.787, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 13, 1300.05, 659.114, 70.2942, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 14, 1361.85, 853.952, 70.2904, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 15, 1408.33, 1010.9, 60.1401, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 16, 1419.27, 1020.78, 54.8617, 0, 0, 0, 1, 1, 0, 0, 0),
(448211, 17, 1009.7, 353.118, 57.484, 0, 0, 0, 1, 1, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448212 AND `point` BETWEEN 0 AND 5;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448212, 0, 994.671, 465.943, 80.597, 0, 0, 0, 1, 1, 0, 0, 0),
(448212, 1, 972.181, 656.085, 119.203, 0, 0, 0, 1, 1, 0, 0, 0),
(448212, 2, 986.189, 699.04, 122.876, 0, 0, 0, 1, 1, 0, 0, 0),
(448212, 3, 1097.17, 790.282, 103.229, 0, 0, 0, 1, 1, 0, 0, 0),
(448212, 4, 1338.65, 979.091, 68.574, 0, 0, 0, 1, 1, 0, 0, 0),
(448212, 5, 1418.54, 1019.48, 53.918, 0, 0, 0, 1, 1, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=446320 AND `point` BETWEEN 1 AND 12;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(446320, 1, 1347.58, 1055.34, 52.4303, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 2, 1346.27, 1031.6, 52.4005, 0, 0, 8000, 1, 0, 0, 100, 0),
(446320, 3, 1347.58, 1055.34, 52.4303, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 4, 1359.32, 1061.86, 52.9373, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 5, 1383.98, 1085.92, 53.1041, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 6, 1394.97, 1093.72, 57.4961, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 7, 1406.86, 1093.08, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 8, 1413.5, 1086.69, 60.477, 0, 0, 8000, 1, 0, 0, 100, 0),
(446320, 9, 1406.86, 1093.08, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 10, 1394.97, 1093.72, 57.4961, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 11, 1383.98, 1085.92, 53.1041, 0, 0, 0, 1, 0, 0, 100, 0),
(446320, 12, 1370.33, 1072.82, 53.2393, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=446321 AND `point` BETWEEN 1 AND 15;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(446321, 1, 1433.18, 1070.35, 60.4769, 0, 0, 8000, 1, 0, 0, 100, 0),
(446321, 2, 1441.05, 1065.37, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 3, 1442.17, 1052.31, 59.632, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 4, 1425.12, 1028.87, 52.6475, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 5, 1410.54, 1022.41, 53.0542, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 6, 1400.68, 1018.82, 53.1551, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 7, 1387, 1008.78, 52.9742, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 8, 1374.11, 1008.62, 52.4603, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 9, 1356.44, 1018, 52.273, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 10, 1370.67, 1009.98, 52.3691, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 11, 1385.57, 1008.11, 52.8996, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 12, 1395.1, 1015.81, 53.1705, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 13, 1440.66, 1048.09, 58.4715, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 14, 1442.34, 1055.47, 60.1114, 0, 0, 0, 1, 0, 0, 100, 0),
(446321, 15, 1442.73, 1063.33, 60.4767, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=445920;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(445920, 0, 1379.38, 1026.29, 52.7724, 0, 0, 0, 1, 0, 0, 100, 0),
(445920, 1, 1378.12, 1008.51, 52.6451, 0, 0, 0, 1, 0, 0, 100, 0),
(445920, 2, 1365.61, 1012.32, 52.2462, 0, 0, 0, 1, 0, 0, 100, 0),
(445920, 3, 1351.37, 1019.94, 52.5687, 0, 0, 0, 1, 0, 0, 100, 0),
(445920, 4, 1344.32, 1013.14, 54.2249, 0, 0, 0, 1, 0, 0, 100, 0),
(445920, 5, 1335.48, 1010.45, 54.5999, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=445921;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(445921, 0, 1346.33, 1050.31, 52.0797, 0, 0, 0, 1, 0, 0, 100, 0),
(445921, 1, 1346.14, 1032.64, 52.3299, 0, 0, 0, 1, 0, 0, 100, 0),
(445921, 2, 1348.5, 1022.23, 52.8049, 0, 0, 0, 1, 0, 0, 100, 0),
(445921, 3, 1343.69, 1018.34, 54.0274, 0, 0, 0, 1, 0, 0, 100, 0),
(445921, 4, 1334.64, 1014.69, 54.5999, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=446402;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(446402, 0, 1412.05, 1089.42, 60.4771, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 1, 1405.38, 1095.16, 60.4774, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 2, 1392.62, 1093.5, 56.4067, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 3, 1380.65, 1083.27, 52.6221, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 4, 1372.86, 1062, 53.0398, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 5, 1376.69, 1048.61, 53.3362, 0, 0, 0, 1, 1, 0, 100, 0),
(446402, 6, 1375.32, 1046.38, 53.2336, 0, 0, 0, 1, 1, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=449132;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(449132, 1, 1042.53, 1576.98, 27.8422, 0, 1.5, 0, 1, 0, 0, 100, 0),
(449132, 2, 1039.96, 1575.8, 27.8389, 0, 1.5, 0, 1, 0, 0, 100, 0),
(449132, 3, 1041.15, 1573.23, 27.8719, 0, 1.5, 0, 1, 0, 0, 100, 0),
(449132, 4, 1043.72, 1574.41, 27.8719, 0, 1.5, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=443670;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(443670, 1, 825.89, 1718.34, 22.2519, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 2, 818.119, 1748.72, 20.3455, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 3, 799.656, 1783.5, 15.0734, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 4, 782.212, 1800.32, 10.3869, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 5, 799.656, 1783.5, 15.0734, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 6, 818.119, 1748.72, 20.3455, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 7, 825.89, 1718.34, 22.2519, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 8, 820.537, 1699.94, 25.5024, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 9, 814.499, 1671.15, 25.0936, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 10, 790.365, 1643.21, 28.6474, 0, 0, 0, 1, 0, 0, 100, 0),
(443670, 11, 814.499, 1671.15, 25.0936, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=443671;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(443671, 1, 1175.62, 1646.04, 24.1475, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 2, 1181.72, 1685.52, 18.2732, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 3, 1168.04, 1727.76, 14.6648, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 4, 1177.37, 1775.86, 15.7438, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 5, 1215.09, 1809.68, 14.6487, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 6, 1225.77, 1870.26, 11.1122, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 7, 1200.27, 1929.76, 9.33947, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 8, 1156.34, 1950.75, 10.7929, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 9, 1109.37, 1923.76, 16.3755, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 10, 1073.83, 1906.91, 11.4691, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 11, 1020.82, 1883.71, 7.93567, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 12, 918.272, 1891.01, 2.30209, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 13, 879.323, 1870.27, 3.01408, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 14, 909.113, 1806.19, 9.54606, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 15, 877.328, 1797.94, 10.5616, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 16, 861.72, 1769.17, 12.5129, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 17, 849.565, 1737.1, 15.6483, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 18, 834.012, 1690.72, 23.1909, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 19, 844.08, 1647.87, 24.3174, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 20, 859.078, 1610.91, 30.0227, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 21, 860.066, 1559.86, 32.5314, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 22, 860.204, 1513.52, 37.2294, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 23, 882.742, 1501.47, 36.9006, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 24, 921.172, 1492.41, 37.668, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 25, 948.871, 1478.94, 41.0227, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 26, 947.87, 1450.26, 43.9835, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 27, 943.933, 1403.02, 43.3574, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 28, 944.467, 1372.42, 45.3726, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 29, 974.8, 1382.46, 46.4532, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 30, 1004.57, 1395, 44.3585, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 31, 1039.49, 1392.53, 40.9365, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 32, 1075.56, 1400.47, 38.4846, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 33, 1101.22, 1430.84, 37.6798, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 34, 1131.34, 1452.96, 36.3552, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 35, 1168.08, 1487.45, 35.1006, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 36, 1185.62, 1547.62, 31.6081, 0, 0, 0, 1, 0, 0, 100, 0),
(443671, 37, 1189.52, 1573.09, 28.3514, 0, 0, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=449117;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(449117, 1, 1003.85, 1525.62, 33.6755, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 2, 1010.8, 1524.84, 33.7151, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 3, 1022.07, 1524.02, 33.1232, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 4, 1038.73, 1522.56, 32.2662, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 5, 1047.17, 1512.18, 32.6264, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 6, 1058.32, 1507.93, 32.1418, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 7, 1075.05, 1512.04, 29.5329, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 8, 1089.86, 1512.07, 29.4874, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 9, 1115.21, 1510.73, 34.1252, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 10, 1124.52, 1514.68, 35.3186, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 11, 1125.05, 1526.36, 32.6298, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 12, 1125.4, 1561.43, 30.5572, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 13, 1125.59, 1602.19, 29.4256, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 14, 1122.79, 1612.25, 29.205, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 15, 1119.13, 1615.9, 28.7379, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 16, 1097.08, 1617.75, 27.3545, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 17, 1060.91, 1618.93, 27.954, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 18, 1035.23, 1619.32, 25.12, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 19, 1011.71, 1619.25, 23.9885, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 20, 1009.35, 1604.06, 24.8816, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 21, 1007.62, 1587.91, 27.0853, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 22, 999.101, 1564.94, 28.7607, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 23, 1003.75, 1546.04, 29.0175, 0, 0, 0, 1, 0, 0, 100, 0),
(449117, 24, 1004.17, 1535.15, 32.0005, 0, 0, 0, 1, 0, 0, 100, 0);

