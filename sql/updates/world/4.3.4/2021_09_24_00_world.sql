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

 -- Forsaken Apothecary
SET @ENTRY := 46483;
DELETE FROM `smart_scripts` WHERE `entryOrGuid` = @ENTRY AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = "SmartAI", `ScriptName` = "" WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 0, 1, 0, 100, 0, 500, 1000, 20000, 35000, 10, 16, 432, 467, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Every 20 - 35 seconds (500ms - 1s initially) - Self: Play random emote: ONESHOT_KNEEL (16), ONESHOT_USESTANDING (432), ONESHOT_WORK_MINING (467),");

 -- Dark Ranger
SET @CGUID := 399000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID AND @CGUID+1;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID, 44632, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1360.47, 1063.27, 52.9999, 3.92421, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(@CGUID+1, 44632, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 1, 1435.85, 1068.76, 60.4794, 5.5292, 300, 0, 0, 1398, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID AND @CGUID+1;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID, 446321, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+1, 446320, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL);

 -- High Warlord Cromush
DELETE FROM `creature` WHERE `guid` = @CGUID+2;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+2, 44640, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1357.21, 1046.36, 52.7554, 4.46892, 300, 0, 0, 387450, 0, 2, 0, 0, 0, 'npc_high_warlord_cromush_44640', 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+2;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+2, 446400, 0, 0, 0, 1, 0, 0, 0, 0, 3, '83231');

 -- Bat Handler Maggotbreath
DELETE FROM `creature` WHERE `guid`= @CGUID+3;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+3, 44825, 0, 0, 0, 1, 0, 1, 264, 0, -1, 0, 1, 1423.08, 1016.46, 52.544, 2.90902, 300, 0, 0, 77490, 0, 0, 0, 0, 0, NULL, 0);
 
 -- Deathstalker Commander Belmont
DELETE FROM `creature` WHERE `guid`= @CGUID+4;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+4, 44789, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1415.07, 1010.24, 52.7315, 2.09687, 300, 0, 0, 2085, 0, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+4;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID+4, 0, 0, 0, 8, 0, 0, 0, 0, 0, 3, '83231');

 -- High Apothecary Shana T'veen
DELETE FROM `creature` WHERE `guid`= @CGUID+5;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+5, 44784, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1431.48, 1086.86, 60.5617, 3.75246, 300, 0, 0, 276, 590, 0, 0, 0, 0, NULL, 0);

 -- Forest Ettin
DELETE FROM `creature` WHERE `guid`= @CGUID+6;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+6, 44367, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1189.52, 1573.09, 28.3514, 1.55021, 300, 0, 0, 819, 0, 2, 16777216, 0, 0, NULL, 0);

 -- Lady Sylvanas Windrunner
DELETE FROM `creature` WHERE `guid`= @CGUID+7;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+7, 44365, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 1, 1382.11, 1040.91, 54.3174, 3.73064, 300, 0, 0, 34356800, 94700, 0, 0, 134251328, 0, 'npc_silverpine_sylvanas_fhc', 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+7;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, '83231');

 -- Dark Ranger
DELETE FROM `creature` WHERE `guid`= @CGUID+8;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+8, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1360.47, 1063.27, 52.9999, 3.92421, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature` WHERE `guid`= @CGUID+9;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+9, 44632, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1435.85, 1068.76, 60.4794, 5.5292, 300, 0, 0, 1398, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+8 AND @CGUID+9;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+8, 446321, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+9, 446320, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL);

 -- Apothecary Witherbloom
DELETE FROM `creature` WHERE `guid`= @CGUID+10;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+10, 44778, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1409.33, 1065.86, 60.5617, 0.226893, 300, 0, 0, 1398, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+10;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+10, 0, 0, 0, 0, 1, 69, 0, 0, 0, 0, NULL);

 -- Apothecary Initiate
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+11 AND @CGUID+23;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+11, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1426.3, 1085.29, 60.4768, 0.715472, 300, 0, 0, 186, 382, 0, 0, 0, 0, NULL, 0),
(@CGUID+12, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1427.61, 1083.78, 60.4768, 0.715472, 300, 0, 0, 186, 382, 0, 0, 0, 0, NULL, 0),
(@CGUID+13, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1428.92, 1082.27, 60.4768, 0.715472, 300, 0, 0, 166, 356, 0, 0, 0, 0, NULL, 0),
(@CGUID+14, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1443.61, 1079.14, 60.4768, 0.687984, 300, 0, 0, 186, 382, 0, 0, 0, 0, NULL, 0),
(@CGUID+15, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1423.87, 1085.03, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+16, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1426.82, 1081.64, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+17, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1421.24, 1065.4, 60.4784, 5.10886, 300, 0, 0, 186, 382, 0, 0, 0, 0, NULL, 0),
(@CGUID+18, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1423.28, 1082.67, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+19, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1424.59, 1081.16, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+20, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1425.9, 1079.65, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+21, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1425.38, 1083.3, 60.4768, 0.715472, 300, 0, 0, 208, 410, 0, 0, 0, 0, NULL, 0),
(@CGUID+22, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1428.33, 1079.9, 60.4768, 0.715472, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0),
(@CGUID+23, 44786, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1410.63, 1102.33, 60.5617, 1.50098, 300, 0, 0, 148, 330, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+14;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+14, 0, 0, 0, 0, 1, 69, 0, 0, 0, 0, NULL);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+17;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+17, 0, 0, 0, 0, 1, 69, 0, 0, 0, 0, NULL);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+23;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+23, 0, 0, 0, 0, 1, 69, 0, 0, 0, 0, NULL);

 -- Apothecary Marry
DELETE FROM `creature` WHERE `guid`= @CGUID+24;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+24, 45490, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1422.57, 1097.31, 60.5617, 4.4855, 300, 0, 0, 230, 498, 0, 0, 0, 0, NULL, 0);

 -- These were extracted from https://cata-twinhead.twinstar.cz/?npc=45490, which a Cataclysm database on version 15595
DELETE FROM `npc_vendor` WHERE `entry`= 45490;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`, `VerifiedBuild`) VALUES 
(45490, 1, 7005, 0, 0, 0, 1, 0, 15595), 
(45490, 2, 2901, 0, 0, 0, 1, 0, 15595),
(45490, 3, 5956, 0, 0, 0, 1, 0, 15595),
(45490, 4, 39505, 0, 0, 0, 1, 0, 15595),
(45490, 5, 6256, 0, 0, 0, 1, 0, 15595),
(45490, 6, 6217, 0, 0, 0, 1, 0, 15595),
(45490, 7, 3371, 0, 0, 0, 1, 0, 15595),
(45490, 8, 2320, 0, 0, 0, 1, 0, 15595),
(45490, 9, 2321, 0, 0, 0, 1, 0, 15595),
(45490, 10, 2678, 0, 0, 0, 1, 0, 15595),
(45490, 11, 2880, 0, 0, 0, 1, 0, 15595),
(45490, 12, 4289, 0, 0, 0, 1, 0, 15595),
(45490, 13, 6529, 0, 0, 0, 1, 0, 15595),
(45490, 14, 6530, 0, 0, 0, 1, 0, 15595),
(45490, 15, 2605, 0, 0, 0, 1, 0, 15595),
(45490, 16, 6260, 0, 0, 0, 1, 0, 15595),
(45490, 17, 2604, 0, 0, 0, 1, 0, 15595),
(45490, 18, 2324, 0, 0, 0, 1, 0, 15595),
(45490, 19, 39354, 0, 0, 0, 1, 0, 15595),
(45490, 20, 20815, 0, 0, 0, 1, 0, 15595);

 -- Forsaken Apothecary
DELETE FROM `creature` WHERE `guid` = @CGUID+25;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+25, 46483, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1367.75, 995.832, 51.5314, 5.34071, 300, 0, 0, 208, 410, 0, 0, 0, 0, NULL, 0);

 -- Arthura
DELETE FROM `creature` WHERE `guid` = @CGUID+26;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+26, 44610, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1360.81, 1030.57, 55.874, 0.645772, 300, 0, 0, 30, 6520, 0, 16777218, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+26;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+26, 0, 0, 0, 50397184, 0, 0, 0, 0, 0, 3, '83231');

 -- Daschla
DELETE FROM `creature` WHERE `guid` = @CGUID+27;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+27, 44609, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1365.73, 1024.98, 56.0244, 1.02974, 300, 0, 0, 9030, 6520, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+27;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+27, 0, 0, 0, 50397184, 0, 0, 0, 0, 0, 3, '83231');

 -- Agatha
DELETE FROM `creature` WHERE `guid` = @CGUID+28;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+28, 44608, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1364.02, 1028.54, 55.9914, 0.855211, 300, 0, 0, 9030, 6520, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+28;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+28, 0, 0, 0, 50397184, 0, 0, 0, 0, 0, 3, '83988');

 -- Lady Sylvanas Windrunner
DELETE FROM `creature` WHERE `guid` = @CGUID+29;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+29, 44365, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1382.11, 1040.91, 54.3174, 3.73064, 300, 0, 0, 34356800, 94700, 0, 0, 134251328, 0, 'npc_silverpine_sylvanas_fhc', 0);

DELETE FROM `creature_addon` WHERE `guid` = @CGUID+29;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+29, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, '83231');

 -- Bat Handler Maggotbreath
DELETE FROM `creature` WHERE `guid` = @CGUID+30;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+30, 44825, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1423.08, 1016.46, 52.544, 2.90902, 300, 0, 0, 77490, 0, 0, 0, 0, 0, NULL, 0);

 -- Dark Ranger
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+31 AND @CGUID+38;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+31, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1341.66, 1020.96, 54.3438, 0.488692, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+32, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1337.14, 1018.39, 54.7498, 3.59538, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+33, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1345.66, 1010.64, 54.3924, 0.349066, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+34, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1341.2, 1009.05, 54.6825, 3.19395, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+35, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1367.17, 1082.13, 52.508, 4.97419, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+36, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1429.77, 1021.99, 52.3983, 3.34491, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+37, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1400.45, 1003, 52.9364, 2.26499, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+38, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1389.62, 1047.75, 52.9542, 3.90648, 300, 0, 0, 1398, 0, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+31 AND @CGUID+38;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+31, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+33, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+34, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+35, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+36, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+37, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL),
(@CGUID+38, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL);

 -- Fallen Human
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+39 AND @CGUID+68;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+39, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1369.57, 1036.9, 50.998, 0.20944, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+40, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1372.08, 1034.98, 50.8489, 0.890118, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+41, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1375.2, 1038.58, 51.2451, 4.55531, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+42, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1374.14, 1036.35, 50.9217, 5.53269, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+43, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1370.66, 1030.82, 51.134, 0.890118, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+44, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1373.35, 1031.58, 50.6189, 0.959931, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+45, 44593, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1376.98, 1030.98, 51.1026, 6.07026, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+46, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1370.66, 1030.82, 51.0504, 0.890118, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+47, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1372.08, 1034.98, 50.7657, 0.890118, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+48, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1374.14, 1036.35, 50.8385, 5.53269, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+49, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1375.2, 1038.58, 51.2451, 4.55531, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+50, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1369.57, 1036.9, 50.9154, 0.20944, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+51, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1376.98, 1030.98, 51.1026, 6.07026, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+52, 44593, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1373.35, 1031.58, 50.5358, 0.959931, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),

(@CGUID+53, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1369.71, 1040.19, 51.0212, 5.58505, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+54, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1367.33, 1035.38, 51.304, 0.890118, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+55, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1372.98, 1041.06, 51.381, 0.226893, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+56, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1372.58, 1038.07, 50.9666, 0.890118, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+57, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1369.65, 1033.67, 50.9668, 0.698132, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+58, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1376.43, 1034.25, 50.7297, 3.06611, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+59, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1373.36, 1033.41, 50.5956, 0.607811, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+60, 44592, 0, 130, 5369, 1, 0, 1, 264, 0, -1, 0, 0, 1374.17, 1028.72, 50.9252, 1.03193, 42, 0, 0, 273, 0, 0, 0, 775, 0, NULL, 0),
(@CGUID+61, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1372.98, 1041.06, 51.2977, 0.226893, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+62, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1369.71, 1040.19, 50.9381, 5.58505, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+63, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1372.58, 1038.07, 50.883, 0.890118, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+64, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1367.33, 1035.38, 51.2203, 0.890118, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+65, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1369.65, 1033.67, 50.8834, 0.698132, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+66, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1374.17, 1028.72, 50.9252, 1.03193, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+67, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1376.43, 1034.25, 50.7297, 3.06611, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+68, 44592, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1373.36, 1033.41, 50.5956, 0.607811, 300, 0, 0, 273, 0, 0, 0, 0, 0, NULL, 0);

 -- Grand Executor Mortuus
DELETE FROM `creature` WHERE `guid` = @CGUID+69;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+69, 44615, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1383, 1038.63, 54.4005, 3.90954, 300, 0, 0, 387450, 66810, 0, 0, 0, 0, NULL, 0);
 
 -- Apothecary Harrington
DELETE FROM `creature` WHERE `guid` = @CGUID+70;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+70, 45491, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1445.27, 1072.9, 60.5436, 1.20428, 300, 0, 0, 230, 498, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+70;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+70, 0, 0, 0, 0, 4097, 69, 0, 0, 0, 0, NULL);

 -- Deathstalker
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+71 AND @CGUID+74;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+71, 44790, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1412.63, 1011.4, 52.9539, 5.77704, 300, 0, 0, 156, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+72, 44790, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1415.05, 1008.17, 52.7525, 1.6009, 300, 0, 0, 176, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+73, 44790, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1415.5, 1012.99, 52.8803, 4.46804, 300, 0, 0, 198, 0, 0, 0, 0, 0, NULL, 0),
(@CGUID+74, 44790, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 1, 1417.19, 1009.78, 52.7009, 2.26893, 300, 0, 0, 156, 0, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+71 AND @CGUID+74;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+71, 0, 0, 0, 8, 0, 0, 0, 0, 0, 3, '83231'),
(@CGUID+72, 0, 0, 0, 8, 0, 0, 0, 0, 0, 3, '83231'),
(@CGUID+73, 0, 0, 0, 8, 0, 0, 0, 0, 0, 3, '83231'),
(@CGUID+74, 0, 0, 0, 8, 0, 0, 0, 0, 0, 3, '83231');

 -- Forsaken Warhorse (ported from MoP)
DELETE FROM `creature_template` WHERE `entry`= 73595;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES 
(73595, 0, 0, 0, 0, 0, 29257, 0, 0, 0, 'Forsaken Warhorse', '', 'Sylvanas\' Pet', NULL, 0, 85, 85, 0, 0, 35, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1077, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 17614);

DELETE FROM `creature_template_addon` WHERE `entry`= 73595;
INSERT INTO `creature_template_addon` (`entry`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(73595, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, '');

 -- Lady Sylvanas Windrunner
DELETE FROM `creature` WHERE `guid`= @CGUID+75;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+75, 44365, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 1, 501.426, 1565.8, 128.309, 4.40509, 300, 0, 0, 34356800, 94700, 0, 0, 134251328, 0, 'npc_silverpine_sylvanas_fhc', 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+75;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+75, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, '84183');

 -- Arthura
DELETE FROM `creature` WHERE `guid`= @CGUID+76;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+76, 44610, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 0, 501.788, 1575.48, 131.741, 4.76637, 300, 0, 0, 9030, 6520, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+76;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+76, 0, 0, 0, 50397184, 1, 0, 0, 0, 0, 3, '84705');

 -- Agatha
DELETE FROM `creature` WHERE `guid`= @CGUID+77;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+77, 44608, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 0, 506.345, 1575.72, 131.741, 4.31476, 300, 0, 0, 9030, 6520, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+77;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+77, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, '84183');

 -- Daschla
DELETE FROM `creature` WHERE `guid`= @CGUID+78;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+78, 44609, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 0, 507.379, 1570.7, 131.741, 3.30946, 300, 0, 0, 9030, 6520, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+78;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+78, 0, 0, 0, 50331648, 1, 0, 0, 0, 0, 3, '84183');

 -- High Warlord Cromush
DELETE FROM `creature` WHERE `guid`= @CGUID+79;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+79, 44640, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 1, 549.267, 1572.28, 131.53, 0.161007, 300, 0, 0, 387450, 0, 2, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+79;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+79, 446401, 0, 0, 0, 1, 0, 0, 0, 0, 3, '84183');

 -- Admiral Hatchet
DELETE FROM `creature` WHERE `guid`= @CGUID+80;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+80, 44916, 0, 130, 228, 1, 0, 1, 169, 0, -1, 0, 1, 492.516, 1527.46, 130.503, 5.52134, 300, 0, 0, 1640, 0, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+80;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+80, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, '84183 83847');

 -- Warlord Torok
DELETE FROM `creature` WHERE `guid`= @CGUID+81;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(@CGUID+81, 44917, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 0, 494.368, 1529.64, 130.002, 5.41923, 300, 0, 0, 1500, 0, 0, 0, 0, 0, NULL, 0);

DELETE FROM `creature_addon` WHERE `guid`= @CGUID+81;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(@CGUID+81, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, '84183');

 -- Veteran Forsaken Trooper
UPDATE `creature_template`SET `npcflag`= 16777216, `ScriptName`= 'npc_silverpine_veteran_forsaken_trooper' WHERE `entry` = 45197;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=45197 AND `spell_id`=84379;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(45197, 84379, 1, 1);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=45197 AND `SourceEntry`=84379 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27180 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(18, 45197, 84379, 0, 0, 47, 0, 27180, 8, 0, 0, 0, 0, '', 'Allow spellclick if quest 27180 is active');

DELETE FROM `pickpocketing_loot_template` WHERE `Entry`=45197 AND `Item`=60862;
INSERT INTO `pickpocketing_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES 
(45197, 60862, 0, 100, 1, 0, 1, 0, 1, 1, 'Forsaken Insignia');

DELETE FROM `spell_area` WHERE `spell`=84459 AND `area`=229 AND `quest_start`=27180 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
(84459, 229, 27180, 0, 0, 0, 2, 3, 8, 0);

 -- Bloodfang Stalker
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_bloodfang_stalker' WHERE `entry` = 45195;
UPDATE `creature_template_addon` SET `auras` = '86237 86559' WHERE `entry` = 45195;

DELETE FROM `spell_area` WHERE `spell`=86560 AND `area`=229 AND `quest_start`=27098 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
(86560, 229, 27098, 27181, 0, 0, 2, 3, 64, 1);
 
 -- Forsaken Catapult
UPDATE `creature_template_addon` SET `auras` = '29266' WHERE `entry` = 4518;







 -- Arthura 
UPDATE `creature_template` SET `ScriptName`= 'npc_arthura_sepulcher' WHERE `entry` = 45318;
UPDATE `creature_template_addon` SET `bytes1` = 50331648 WHERE `entry` = 45318;

DELETE FROM `creature_text` WHERE `CreatureID`= 45318 AND `GroupID`= 0 AND `ID`= 0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(45318, 0, 0, 'Do not be afraid. I will not let you fall.', 12, 0, 0, 1, 0, 0, 0, 0, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`= 453180;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(453180, 0, 480.463, 1550.35, 155.601, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 1, 387.175, 1495.62, 170.981, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 2, 265.484, 1472.8, 180.727, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 3, 139.372, 1441.83, 198.614, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 4, 45.9503, 1416.35, 209.282, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 5, -56.9284, 1376.26, 202.527, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 6, -124.782, 1348.55, 197.044, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 7, -229.008, 1306.44, 188.31, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 8, -274.557, 1298.95, 185.372, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 9, -320.862, 1317.17, 182.359, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 10, -407.002, 1394.83, 171.236, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 11, -510.902, 1494.75, 172.678, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 12, -551.073, 1522.63, 172.754, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 13, -626.841, 1532.35, 173.785, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 14, -773.475, 1542.31, 176.662, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 15, -817.672, 1548.33, 171.853, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 16, -845.863, 1567, 154.912, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 17, -868.691, 1634.66, 114.451, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 18, -888.581, 1675.62, 93.9851, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 19, -914.318, 1672, 87.8073, NULL, 25, 0, 1, 1, 0, 100, 0),
(453180, 20, -937.953, 1636.42, 68.4057, NULL, 25, 0, 1, 0, 0, 100, 0);

 -- Orc Sea Dog
DELETE FROM `creature` WHERE `guid`= 321872;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(321872, 44913, 0, 130, 5386, 1, 0, 1, 169, 0, -1, 0, 0, 1045.19, 1567.43, 28.1601, 1.40073, 300, 0, 0, 1, 0, 0, 0, 0, 0, '', 0);

 -- Agatha
DELETE FROM `creature_template_addon` WHERE `entry`= 44951;
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

 -- Forsaken Trooper
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_male_fenris' WHERE `entry` = 44958;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_male_fenris' WHERE `entry` = 44959;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_male_fenris' WHERE `entry` = 44960;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_male_fenris' WHERE `entry` = 44961;

UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_female_fenris' WHERE `entry` = 44962;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_female_fenris' WHERE `entry` = 44963;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_female_fenris' WHERE `entry` = 44964;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_forsaken_trooper_female_fenris' WHERE `entry` = 44965;

DELETE FROM `creature_text` WHERE `CreatureID`=44958;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44958, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44958, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44958, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44958, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44958, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44959;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44959, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44959, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44959, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44959, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44959, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44960;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44960, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44960, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44960, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44960, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44960, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44961;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44961, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44961, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44961, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44961, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44961, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44962;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44962, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44962, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44962, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44962, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44962, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44963;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44963, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44963, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44963, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44963, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44963, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44964;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44964, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44964, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44964, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44964, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44964, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `CreatureID`=44965;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(44965, 0, 0, 'I am Forsaken.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44965, 0, 1, 'For the glory of the Dark Lady!', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44965, 0, 2, 'Wha... What has happened?', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44965, 0, 3, 'I am eternal... I am death.', 12, 0, 100, 16, 0, 0, 0, 0, 0, ''),
(44965, 0, 4, 'I LIVE!', 12, 0, 100, 16, 0, 0, 0, 0, 0, '');

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

 -- Lordaeron Quest stuff
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_warhorse_player_lordaeron' WHERE `entry` = 45041;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_warhorse_sylvanas_lordaeron' WHERE `entry` = 45057;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_sylvanas_lordaeron' WHERE `entry` = 45051;
UPDATE `creature_template` SET `ScriptName`= 'npc_silverpine_dreadguard_lordaeron' WHERE `entry` = 45588;

DELETE FROM `waypoint_data` WHERE `id`= 450570;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(450570, 1, 1361.34, 1026.57, 52.7564, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 2, 1341.41, 1015.48, 54.5546, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 3, 1326.32, 1011.06, 54.5992, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 4, 1310.18, 1039.85, 54.5992, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 5, 1284.02, 1074.41, 53.3174, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 6, 1271.46, 1090.13, 52.2455, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 7, 1252.99, 1106.65, 51.4217, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 8, 1222.99, 1130.19, 50.4758, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 9, 1202.34, 1144.84, 49.592, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 10, 1177.26, 1162.18, 48.9129, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 11, 1147.52, 1182.8, 48.2925, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 12, 1113.01, 1204.53, 46.7171, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 13, 1090.95, 1220.55, 46.3405, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 14, 1069.73, 1242.65, 46.2058, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 15, 1049.1, 1263.08, 46.0836, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 16, 1020.5, 1286.14, 45.9548, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 17, 998.104, 1299.97, 45.8613, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 18, 962.071, 1322.14, 46.134, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 19, 932.884, 1340.81, 47.1454, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 20, 912.49, 1349.2, 48.3521, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 21, 883.944, 1356.02, 51.5032, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 22, 864.58, 1361.52, 54.6625, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 23, 816.589, 1361.97, 56.856, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 24, 796.295, 1362.45, 60.7326, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 25, 768.406, 1362.56, 65.9252, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 26, 738.898, 1360.21, 70.7869, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 27, 719.656, 1352.28, 73.8382, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 28, 703.703, 1341.31, 76.9114, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 29, 685.394, 1323.57, 79.4473, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 30, 667.123, 1311.57, 81.713, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 31, 645.257, 1302.21, 84.9805, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 32, 637.26, 1308.36, 84.6332, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 33, 624.309, 1330.15, 84.646, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 34, 610.8, 1356.02, 87.0058, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 35, 596.417, 1387.09, 90.5174, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 36, 583.141, 1415.35, 95.7118, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 37, 572.297, 1434.98, 100.203, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 38, 550.889, 1457.77, 106.652, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 39, 532.182, 1475.22, 114.478, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 40, 516.007, 1488.57, 121.406, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 41, 505.278, 1507.11, 126.695, 0, 6.3564, 0, 1, 1, 0, 100, 0),
(450570, 42, 501.087, 1524.91, 128.901, 0, 6.3564, 0, 0, 1, 0, 100, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=264 AND `SourceEntry`=5369 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=26965 AND `ConditionValue2`=64 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 264, 5369, 0, 0, 47, 0, 26965, 64, 0, 1, 0, 0, '', 'Forsaken High Command - Add phase 264 - 26965 not rewarded');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=5369 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=26965 AND `ConditionValue2`=64 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 5369, 0, 2, 47, 0, 26965, 64, 0, 0, 0, 0, '', 'Forsaken High Command - Add phase 169 - 26965 rewarded');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=267 AND `SourceEntry`=5369 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 267, 5369, 0, 2, 47, 0, 27098, 8, 0, 0, 0, 0, '', 'Forsaken High Command - Add phase 267 - 27098 taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=267 AND `SourceEntry`=130 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 267, 130, 0, 2, 47, 0, 27098, 8, 0, 0, 0, 0, '', 'Silverpine Forest - Add phase 267 - 27098 taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=267 AND `SourceEntry`=228 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 267, 228, 0, 2, 47, 0, 27098, 8, 0, 0, 0, 0, '', 'The Sepulcher - Add phase 267 - 27098 taken');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=5369 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 5369, 0, 2, 47, 0, 27098, 8, 0, 1, 0, 0, '', 'Forsaken High Command - Remove Phase 169 - 27098 taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=130 AND `SourceId`=0 AND `ElseGroup`=1 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 130, 0, 1, 47, 0, 27098, 8, 0, 1, 0, 0, '', 'Silverpine Forest - Remove Phase 169 - 27098 taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=169 AND `SourceEntry`=228 AND `SourceId`=0 AND `ElseGroup`=1 AND `ConditionTypeOrReference`=47 AND `ConditionTarget`=0 AND `ConditionValue1`=27098 AND `ConditionValue2`=8 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(26, 169, 228, 0, 1, 47, 0, 27098, 8, 0, 1, 0, 0, '', 'The Sepulcher - Remove Phase 169 - 27098 taken');

DELETE FROM `phase_area` WHERE `AreaId`=5369 AND `PhaseId`=169;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES (5369, 169, 'Forsaken High Command after 26965 is rewarded');
DELETE FROM `phase_area` WHERE `AreaId`=5369 AND `PhaseId`=264;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES (5369, 264, 'Forsaken High Command before 26965 is rewarded');
DELETE FROM `phase_area` WHERE `AreaId`=5369 AND `PhaseId`=267;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES (5369, 267, 'Forsaken High Command during 27098');
DELETE FROM `phase_area` WHERE `AreaId`=130 AND `PhaseId`=267;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES (130, 267, 'Silverpine Forest during 27098');
DELETE FROM `phase_area` WHERE `AreaId`=228 AND `PhaseId`=267;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES (228, 267, 'The Sepulcher during 27098');

 -- According to SummonProperties.dbc, this RecID has ParamType 4, not 3; in fact, it wouldn't make the entire script work because of this.
UPDATE `summon_properties_parameters` SET `ParamType`= 4 WHERE `RecID` = 3033;

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
DELETE FROM `creature_addon` WHERE `guid` = 321893;
INSERT INTO `creature_addon` (`guid`, `waypointPathId`, `cyclicSplinePathId`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(321893, 445470, 0, 0, 0, 1, 0, 0, 0, 0, 0, '');

UPDATE `creature` SET `spawndist`= 0 WHERE `guid` = 321729;
DELETE FROM `creature_addon` WHERE `guid` = 321729;
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

UPDATE creature_template SET `ScriptName` = 'npc_silverpine_sylvanas_fhc' WHERE `entry` = 44365;
UPDATE creature_template SET `npcflag` = 16777216, `ScriptName` = 'npc_silverpine_armoire_click' WHERE `entry` = 44894;
UPDATE creature_template SET `unit_flags` = 33554440, `VehicleId` = 1055, `ScriptName` = 'npc_silverpine_armoire' WHERE `entry` = 44893;
UPDATE creature_template SET `unit_flags` = 2, `unit_flags2` = 16384, `VehicleId` = 1059, `ScriptName` = 'npc_silverpine_packleader_ivar_bloodfang_exhanguinate' WHERE `entry` = 44884;
UPDATE creature_template SET `unit_flags` = 2, `unit_flags2` = 16384, `ScriptName` = 'npc_silverpine_lord_darius_crowley_exhanguinate' WHERE `entry` = 44883;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_deathstalker' WHERE `entry` = 44789;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_deathstalker' WHERE `entry` = 44790;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_deathstalker_rane_yorick' WHERE `entry` = 44882;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_bat_handler_maggotbreath'  WHERE `entry` = 44825;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_vilefine_murlocks'  WHERE `entry` = 1768;
UPDATE creature_template SET `ScriptName` = 'npc_silverpine_vilefine_murlocks'  WHERE `entry` = 1908;
UPDATE creature_template SET `VehicleId` = 1051, `ScriptName` = 'npc_silverpine_forsaken_bat'  WHERE `entry` = 44821;
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
(44365, 3, 0, 'With the death of the Lich King, many of the more intelligent Scourge became... unemployed. Those \'fiends,\' as you so delicately put it, are called val\'kyr. They are under my command now...', 12, 0, 100, 0, 0, 20462, 0, 44702, 0, 'VO_QE_SP_Sylvanas_SPEvent04'),
(44365, 4, 0, '...and they are part of the reason that I asked to see you.', 12, 0, 100, 1, 0, 20463, 0, 44703, 0, 'VO_QE_SP_Sylvanas_SPEvent05'),
(44365, 5, 0, 'Very well, Warchief. I have solved the plight of the Forsaken!', 12, 0, 100, 5, 0, 20464, 0, 44705, 0, 'VO_QE_SP_Sylvanas_SPEvent06'),
(44365, 6, 0, 'As a race, we Forsaken are unable to procreate.', 12, 0, 100, 274, 0, 20465, 0, 44706, 0, 'VO_QE_SP_Sylvanas_SPEvent07'),
(44365, 7, 0, 'With the aid of the val\'kyr, we are now able to take the corpses of the fallen and create new Forsaken.', 12, 0, 100, 0, 0, 20466, 0, 44707, 0, 'VO_QE_SP_Sylvanas_SPEvent08'),
(44365, 8, 0, 'Agatha, show the Warchief!', 12, 0, 100, 5, 0, 20467, 0, 44709, 0, 'VO_QE_SP_Sylvanas_SPEvent09'),
(44365, 9, 0, 'Warchief, without these new Forsaken my people would die out... Our hold upon Gilneas and northern Lordaeron would crumble.', 12, 0, 100, 0, 0, 20468, 0, 44715, 0, 'VO_QE_SP_Sylvanas_SPEvent10'),
(44365, 10, 0, 'Isn\'t it obvious, Warchief? I serve the Horde.', 12, 0, 100, 66, 0, 20469, 0, 44718, 0, 'VO_QE_SP_Sylvanas_SPEvent11');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44615 AND `GroupID`=0 AND `ID`=0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44615, 0, 0, 'Move, Forsaken!', 12, 0, 100, 25, 2, 0, 0, 0, 0, '');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44640 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44640, 0, 0, 'ABBERATION!', 12, 0, 100, 5, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01'),
(44640, 1, 0, 'As you command, Warchief.', 12, 0, 100, 66, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01');
 
DELETE FROM `creature_text` WHERE `CreatureID`=44629 AND `ID`=0 AND `GroupID` BETWEEN 0 AND 8;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `SoundType`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(44629, 0, 0, 'This better be important, Sylvanas. You know how I detest this place and its foul stench. Why have you called for me?', 12, 0, 100, 0, 0, 20496, 0, 0, 0, 'VO_QE_Garrosh_SPEvent01'),
(44629, 1, 0, 'And more importantly, what are those Scourge fiends doing here?', 12, 0, 100, 25, 0, 20497, 0, 0, 0, 'VO_QE_Garrosh_SPEvent02'),
(44629, 2, 0, 'Get on with it, Sylvanas.', 12, 0, 100, 1, 0, 20498, 0, 0, 0, 'VO_QE_Garrosh_SPEvent03'),
(44629, 3, 0, 'What you have done here, Sylvanas... it goes against the laws of nature. Disgusting is the only word I have to describe it.', 12, 0, 100, 0, 0, 20499, 0, 0, 0, 'VO_QE_Garrosh_SPEvent04'),
(44629, 4, 0, 'Have you given any thought to what this means, Sylvanas?', 12, 0, 100, 6, 0, 20500, 0, 0, 0, 'VO_QE_Garrosh_SPEvent05'),
(44629, 5, 0, 'What difference is there between you and the Lich King now?', 12, 0, 100, 6, 0, 20501, 0, 0, 0, 'VO_QE_Garrosh_SPEvent06'),
(44629, 6, 0, 'Watch your clever mouth, bitch.', 12, 0, 100, 397, 0, 20502, 0, 0, 0, 'VO_QE_Garrosh_SPEvent07'),
(44629, 7, 0, 'Cromush, you stay behind and make sure the Banshee Queen is well "guarded." I will be expecting a full report when next we meet.', 12, 0, 100, 0, 0, 20503, 0, 0, 0, 'VO_QE_Garrosh_SPEvent08'),
(44629, 8, 0, 'Remember, Sylvanas, eventually we all have to stand before our maker and face judgment. Your day may come sooner than others...', 12, 0, 100, 0, 0, 20504, 0, 0, 0, 'VO_QE_Garrosh_SPEvent09');
 
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

-- These were extracted from https://cata-twinhead.twinstar.cz/?npc=45496, which a Cataclysm database on version 15595
DELETE FROM `npc_vendor` WHERE `entry`=45496;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`, `VerifiedBuild`) VALUES 
(45496, 1, 159, 0, 0, 0, 1, 0, 15595),
(45496, 2, 1179, 0, 0, 0, 1, 0, 15595),
(45496, 3, 4540, 0, 0, 0, 1, 0, 15595),
(45496, 4, 4541, 0, 0, 0, 1, 0, 15595),
(45496, 5, 4496, 0, 0, 0, 1, 0, 15595),
(45496, 6, 4498, 0, 0, 0, 1, 0, 15595),
(45496, 7, 4470, 0, 0, 0, 1, 0, 15595),
(45496, 8, 5042, 0, 0, 0, 1, 0, 15595),
(45496, 9, 1205, 0, 0, 0, 1, 0, 15595), 
(45496, 10, 4542, 0, 0, 0, 1, 0, 15595),
(45496, 11, 5048, 0, 0, 0, 1, 0, 15595);

 -- These were extracted from https://cata-twinhead.twinstar.cz/?npc=45497, which a Cataclysm database on version 15595
DELETE FROM `npc_vendor` WHERE `entry`=45497;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`, `VerifiedBuild`) VALUES 
(45497, 1, 159, 0, 0, 0, 1, 0, 15595),
(45497, 2, 1179, 0, 0, 0, 1, 0, 15595),
(45497, 3, 4540, 0, 0, 0, 1, 0, 15595),
(45497, 4, 4541, 0, 0, 0, 1, 0, 15595),
(45497, 5, 4496, 0, 0, 0, 1, 0, 15595),
(45497, 6, 4498, 0, 0, 0, 1, 0, 15595),
(45497, 7, 4470, 0, 0, 0, 1, 0, 15595),
(45497, 8, 5042, 0, 0, 0, 1, 0, 15595),
(45497, 9, 7005, 0, 0, 0, 1, 0, 15595), 
(45497, 10, 2901, 0, 0, 0, 1, 0, 15595),
(45497, 11, 5956, 0, 0, 0, 1, 0, 15595),
(45497, 12, 39505, 0, 0, 0, 1, 0, 15595),
(45497, 13, 6256, 0, 0, 0, 1, 0, 15595),
(45497, 14, 6217, 0, 0, 0, 1, 0, 15595),
(45497, 15, 3371, 0, 0, 0, 1, 0, 15595),
(45497, 16, 2320, 0, 0, 0, 1, 0, 15595),
(45497, 17, 2321, 0, 0, 0, 1, 0, 15595),
(45497, 18, 2678, 0, 0, 0, 1, 0, 15595),
(45497, 19, 2880, 0, 0, 0, 1, 0, 15595),
(45497, 20, 4289, 0, 0, 0, 1, 0, 15595),
(45497, 21, 6529, 0, 0, 0, 1, 0, 15595),
(45497, 22, 6530, 0, 0, 0, 1, 0, 15595),
(45497, 23, 2605, 0, 0, 0, 1, 0, 15595),
(45497, 24, 6260, 0, 0, 0, 1, 0, 15595),
(45497, 25, 2604, 0, 0, 0, 1, 0, 15595),
(45497, 26, 2324, 0, 0, 0, 1, 0, 15595),
(45497, 27, 39354, 0, 0, 0, 1, 0, 15595),
(45497, 28, 20815, 0, 0, 0, 1, 0, 15595);

UPDATE quest_template_addon SET `PrevQuestID` = '27098', `NextQuestID` = '27232' WHERE `ID` = 27231;
UPDATE quest_template_addon SET `PrevQuestID` = '27098' WHERE `ID` = 27226;
UPDATE quest_template_addon SET `PrevQuestID` = '27094' WHERE `ID` = 27096;
UPDATE quest_template_addon SET `PrevQuestID` = '27195' WHERE `ID` = 27290;
UPDATE quest_template_addon SET `PrevQuestID` = '26965' WHERE `ID` = 27290;

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

 -- Detect: Quest Invis Zone 1
DELETE FROM `spell_area` WHERE `spell`=83232 AND `area`=5369 AND `quest_start`=26964 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES
(83232, 5369, 26964, 27098, 0, 0, 2, 3, 66, 1);

 -- Detect: Quest Invis Zone 3
DELETE FROM `spell_area` WHERE `spell`=83989 AND `area`=5369 AND `quest_start`=26964 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
(83989, 5369, 26964, 27097, 0, 0, 2, 3, 74, 1);

 -- Detect: Quest Invis Zone 5
DELETE FROM `spell_area` WHERE `spell`=84241 AND `area`=5386 AND `quest_start`=27065 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
(84241, 5386, 27065, 27098, 0, 0, 2, 3, 74, 1);

DELETE FROM `spell_script_names` WHERE `spell_id`=83173 AND `ScriptName`='spell_silverpine_raise_forsaken_83173';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83173, 'spell_silverpine_raise_forsaken_83173');

DELETE FROM `spell_script_names` WHERE `spell_id`=83149 AND `ScriptName`='spell_silverpine_forsaken_trooper_masterscript_high_command';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83149, 'spell_silverpine_forsaken_trooper_masterscript_high_command');

DELETE FROM `spell_script_names` WHERE `spell_id`=80365 AND `ScriptName`='spell_silverpine_flurry_of_claws';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80365, 'spell_silverpine_flurry_of_claws');

DELETE FROM `spell_script_names` WHERE `spell_id`=83594 AND `ScriptName`='spell_silverpine_go_home';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83594, 'spell_silverpine_go_home');

DELETE FROM `spell_script_names` WHERE `spell_id`=83788 AND `ScriptName`='spell_silverpine_hide_in_armoire';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83788, 'spell_silverpine_hide_in_armoire');

DELETE FROM `spell_script_names` WHERE `spell_id`=83865 AND `ScriptName`='spell_silverpine_sea_pup_trigger';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83865, 'spell_silverpine_sea_pup_trigger');

DELETE FROM `spell_script_names` WHERE `spell_id`=83838 AND `ScriptName`='spell_silverpine_pick_up_orc_crate';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83838, 'spell_silverpine_pick_up_orc_crate');

DELETE FROM `spell_script_names` WHERE `spell_id`=83978 AND `ScriptName`='spell_silverpine_agatha_broadcast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83978, 'spell_silverpine_agatha_broadcast');

DELETE FROM `spell_script_names` WHERE `spell_id`=83990 AND `ScriptName`='spell_silverpine_notify_agatha';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83990, 'spell_silverpine_notify_agatha');

DELETE FROM `spell_script_names` WHERE `spell_id`=83997 AND `ScriptName`='spell_silverpine_forsaken_trooper_masterscript_fenris';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83997, 'spell_silverpine_forsaken_trooper_masterscript_fenris');

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

DELETE FROM `waypoint_data` WHERE `id`=448820;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448820, 1, 1300.27, 1190.11, 52.600067, 2.45043, 3, 0, 1, 0, 0, 0, 0),
(448820, 2, 1288.52, 1196.86, 52.850067, 2.49362, 3, 0, 1, 0, 0, 0, 0),
(448820, 3, 1287.52, 1205.11, 53.100067, 1.70037, 3, 0, 1, 0, 0, 0, 0),
(448820, 4, 1294.27, 1207.36, 54.100067, 1.53936, 3, 0, 1, 0, 0, 0, 0),
(448820, 5, 1299.27, 1207.86, 54.100067, 0.373047, 3, 0, 1, 0, 0, 0, 0),
(448820, 6, 1304.27, 1210.36, 54.100067, 0.325923, 3, 0, 1, 0, 0, 0, 0),
(448820, 7, 1304.77, 1217.86, 54.100067, 0.93068, 3, 0, 1, 0, 0, 0, 0),
(448820, 8, 1298.52, 1221.36, 54.100067, 1.36265, 3, 0, 1, 0, 0, 0, 0),
(448820, 9, 1298.27, 1218.86, 55.100067, 1.65325, 3, 0, 1, 0, 0, 0, 0),
(448820, 10, 1298.02, 1216.61, 56.350067, 2.43864, 3, 0, 1, 0, 0, 0, 0),
(448820, 11, 1297.27, 1212.86, 58.600067, 3.94268, 3, 0, 1, 0, 0, 0, 0),
(448820, 12, 1298.27, 1206.86, 58.600067, 4.06835, 3, 0, 1, 0, 0, 0, 0),
(448820, 13, 1307.27, 1206.11, 58.600067, 4.72023, 3, 0, 1, 0, 0, 0, 0),
(448820, 14, 1311.27, 1205.86, 58.600067, 5.53704, 3, 0, 1, 0, 0, 0, 0),
(448820, 15, 1312.67, 1208.86, 58.5123, 6.2596, 3, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448821;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448821, 1, 1313.435, 1210.425, 58.7561, 0, 2.45043, 0, 1, 0, 0, 0, 0),
(448821, 2, 1313.7, 1211.99, 58.4999, 0, 2.45043, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448830;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448830, 0, 1299.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 1, 1300.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 2, 1302.33, 1206.44, 58.499, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 3, 1305.32, 1206.43, 58.5126, 0, 3, 0, 1, 0, 0, 0, 0),
(448830, 4, 1313.48, 1206.09, 58.5119, 0, 3, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448840;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448840, 0, 1296.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 1, 1297.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 2, 1302.31, 1206.61, 58.4984, 0, 3, 0, 1, 0, 0, 0, 0),
(448840, 3, 1308.4, 1206.28, 58.5109, 0, 3, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448841;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448841, 0, 1309.4451, 1206.865, 58.7611, 0, 2.4768722, 0, 1, 0, 0, 0, 0),
(448841, 1, 1310.49, 1207.45, 58.5113, 0, 2.4768722, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448842;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448842, 0, 1312.585, 1209.135, 58.7603, 0, 2.4704797, 0, 1, 0, 0, 0, 0),
(448842, 1, 1313.18, 1210.32, 58.5093, 0, 2.4704797, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448843;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448843, 0, 1311.25, 1208.875, 58.7602, 0, 2.4936619, 0, 1, 0, 0, 0, 0),
(448843, 1, 1309.32, 1206.43, 58.5111, 0, 2.4936619, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448844;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448844, 0, 1304.2949, 1207.105, 58.760803, 0, 7.878883, 0, 1, 0, 0, 0, 0),
(448844, 1, 1299.0449, 1209.605, 58.760803, 0, 7.878883, 0, 1, 0, 0, 0, 0),
(448844, 2, 1297.27, 1212.28, 58.5105, 0, 7.878883, 0, 0, 0, 0, 0, 0);

DELETE FROM `waypoint_data` WHERE `id`=448831;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448831, 0, 1310.28, 1206.0249, 58.76205, 0, 2.4830027, 0, 1, 0, 0, 0, 0),
(448831, 1, 1305.58, 1206.46, 58.5122, 0, 2.4830027, 0, 0, 0, 0, 0, 0);

-- Forsaken Bat
DELETE FROM `waypoint_data` WHERE `id`=448210;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(448210, 0, 1374.36, 1029.12, 67.8815, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 1, 1289.08, 1013.22, 67.8815, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 2, 1224, 997.236, 59.9371, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 3, 1178.72, 957.238, 47.2426, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 4, 1156.5, 852.318, 42.7704, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 5, 1158.1, 725.983, 42.7704, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 6, 1187.67, 575.212, 46.7704, 0, 17.794235, 0, 1, 1, 0, 100, 0),
(448210, 7, 1202.33, 472.97, 65.4648, 0, 17.794235, 0, 0, 1, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=448211;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448211, 0, 1214.8, 414.792, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 1, 1221.35, 323.231, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 2, 1189.93, 281.372, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 3, 1100.24, 212.868, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 4, 1015.84, 188.21, 63.0412, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 5, 939.417, 163.743, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 6, 874.302, 114.092, 61.0968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 7, 831.491, 59.3177, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 8, 770.872, 70.7726, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 9, 759.814, 109.738, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 10, 803.745, 181.882, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 11, 840.674, 202.097, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 12, 846.651, 232.344, 56.5968, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 13, 805.031, 261.606, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 14, 759.528, 265.283, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 15, 693.755, 282.512, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 16, 658.036, 357.233, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 17, 639.776, 433.974, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 18, 676.128, 470.752, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 19, 706.495, 454.023, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 20, 713.505, 380.88, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 21, 726.146, 352.188, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 22, 758.299, 321.101, 59.9579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 23, 797.684, 295.753, 58.1801, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 24, 882.036, 252.597, 52.1245, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 25, 929.667, 225.609, 52.1245, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 26, 987.259, 219.582, 52.1245, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 27, 1030.56, 276.193, 52.1245, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 28, 1058.35, 308.229, 49.3745, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 29, 1121.81, 372.755, 56.4579, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 30, 1167.09, 438.865, 58.4857, 0, 17.982668, 0, 1, 1, 0, 100, 0),
(448211, 31, 1197.37, 451.757, 64.569, 0, 17.982668, 0, 0, 1, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=448212;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(448212, 0, 931.068, 352.101, 81.027, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 1, 952.535, 571.493, 108.303, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 2, 970.028, 664.392, 110.83, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 3, 1031.97, 754.155, 111.441, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 4, 1234.44, 880.16, 97.9414, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 5, 1345.66, 970.281, 75.1914, 0, 35.78791, 0, 1, 1, 0, 100, 0),
(448212, 6, 1415.9, 1018.08, 55.3025, 0, 35.78791, 0, 0, 1, 0, 100, 0);

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
(446321, 15, 1442.73, 1063.33, 60.4767, 0, 0, 0, 00, 0, 0, 100, 0);

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

DELETE FROM `waypoint_data` WHERE `id`=446320;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(446320, 1, 1438.05, 1068.76, 60.7803, 0, 2.51749, 0, 1, 0, 0, 100, 0),
(446320, 2, 1442.28, 1059.43, 60.7893, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 3, 1441.53, 1056.43, 60.5393, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 4, 1441.03, 1054.43, 60.2893, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 5, 1440.53, 1052.93, 59.7893, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 6, 1440.03, 1050.93, 59.2893, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 7, 1439.78, 1049.93, 59.0393, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446320, 8, 1427.56, 1030.52, 52.9738, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446320, 9, 1426.56, 1029.52, 52.9738, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446320, 10, 1415.81, 1025.27, 53.2238, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446320, 11, 1413.73, 1024.23, 53.1286, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446320, 12, 1407.48, 1021.98, 53.3786, 0, 2.49884, 0, 1, 0, 0, 100, 0),
(446320, 13, 1400.1, 1019.1, 53.3104, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 14, 1387.85, 1009.35, 53.0604, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 15, 1392.1951, 1012.625, 53.310387, 0, 2.4977417, 0, 1, 0, 0, 100, 0),
(446320, 16, 1400.496, 1019.28534, 53.38082, 0, 2.49989, 0, 1, 0, 0, 100, 0),
(446320, 17, 1413.4482, 1024.3733, 53.147682, 0, 2.50036795, 0, 1, 0, 0, 100, 0),
(446320, 18, 1413.9482, 1024.6233, 53.147682, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446320, 19, 1420.1982, 1026.8733, 52.897682, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446320, 20, 1426.802, 1029.8049, 53.151054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 21, 1428.552, 1032.0549, 53.401054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 22, 1429.802, 1033.8049, 53.901054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 23, 1430.802, 1035.3049, 54.151054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 24, 1432.052, 1036.8049, 54.651054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 25, 1432.552, 1037.5549, 54.901054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 26, 1433.802, 1039.0549, 55.151054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 27, 1434.302, 1040.0549, 55.401054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 28, 1435.052, 1040.8049, 55.651054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 29, 1436.052, 1042.3049, 56.151054, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446320, 30, 1437.0781, 1043.4365, 56.76165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 31, 1437.5781, 1044.1865, 57.26165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 32, 1438.8281, 1045.9365, 57.76165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 33, 1438.8281, 1046.1865, 58.01165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 34, 1438.8281, 1046.1865, 58.26165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 35, 1439.3281, 1047.9365, 58.26165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 36, 1439.8281, 1049.9365, 59.01165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 37, 1440.3281, 1051.6865, 59.51165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 38, 1440.8281, 1053.6865, 60.01165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 39, 1441.3281, 1055.4365, 60.26165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446320, 40, 1441.8103, 1057.7498, 60.610058, 0, 2.4880388, 0, 1, 0, 0, 100, 0),
(446320, 41, 1442.3103, 1059.4998, 60.610058, 0, 2.4880388, 0, 1, 0, 0, 100, 0),
(446320, 42, 1437.824, 1068.9791, 60.76616, 0, 2.5123012, 0, 1, 0, 0, 100, 0),
(446320, 43, 1429.38, 1076.24, 60.4836, 0, 2.5123012, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=446321;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(446321, 1, 1352.725, 1059.25, 53.049904, 0, 2.51749, 0, 1, 0, 0, 100, 0),
(446321, 2, 1360.8423, 1063.4806, 53.141506, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 3, 1369.0923, 1071.7306, 53.391506, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 4, 1371.5746, 1074.4009, 53.789463, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 5, 1373.5746, 1076.4009, 53.289463, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 6, 1375.8246, 1078.6509, 53.039463, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 7, 1382.3246, 1085.6509, 53.289463, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446321, 8, 1383.8246, 1086.9009, 53.539463, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446321, 9, 1384.5746, 1087.6509, 53.789463, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446321, 10, 1386.8806, 1090.0674, 54.586235, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446321, 11, 1388.1306, 1091.5674, 54.836235, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446321, 12, 1388.8806, 1092.3174, 55.086235, 0, 2.49884, 0, 1, 0, 0, 100, 0),
(446321, 13, 1388.8806, 1092.3174, 55.336235, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 14, 1390.8806, 1092.5674, 55.836235, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 15, 1393.6306, 1093.0674, 57.086235, 0, 2.4977417, 0, 1, 0, 0, 100, 0),
(446321, 16, 1394.6306, 1093.3174, 57.336235, 0, 2.49989, 0, 1, 0, 0, 100, 0),
(446321, 17, 1396.3806, 1093.8174, 58.336235, 0, 2.50036795, 0, 1, 0, 0, 100, 0),
(446321, 18, 1399.3806, 1094.3174, 59.586235, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446321, 19, 1400.3806, 1094.5674, 59.836235, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446321, 20, 1402.1306, 1095.0674, 60.336235, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 21, 1403.1306, 1095.3174, 60.586235, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 22, 1406.7034, 1095.9951, 60.7336, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 23, 1415.4838, 1085.996, 60.7336, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 24, 1419.93, 1084.92, 60.4786, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 25, 1415.3938, 1086.3098, 60.7336, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 26, 1406.312, 1096.0481, 60.624386, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 27, 1402.562, 1095.0481, 60.374386, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 28, 1400.562, 1094.5481, 59.874386, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 29, 1397.812, 1094.0481, 58.624386, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446321, 30, 1396.812, 1093.7981, 58.374386, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 31, 1394.812, 1093.2981, 57.624386, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 32, 1394.062, 1093.0481, 57.124386, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 33, 1392.062, 1093.0481, 56.624386, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 34, 1371.224, 1074.0079, 53.60165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 35, 1366.474, 1069.2579, 53.35165, 0, 2.3907468, 0, 1, 0, 0, 100, 0),
(446321, 36, 1360.3677, 1063.3889, 53.05648, 0, 2.3907468, 0, 1, 0, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=446400;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(446400, 1, 1354.036, 1035.0999, 52.81333, 0, 2.51749, 0, 1, 0, 0, 100, 0),
(446400, 2, 1359.1589, 1022.6127, 52.732635, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 3, 1366.1589, 1019.3627, 52.982635, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 4, 1372.1357, 1016.7146, 53.057182, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 5, 1376.1357, 1017.4646, 53.307182, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 6, 1385.0673, 1019.3989, 53.502197, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 7, 1389.9948, 1033.2056, 53.689217, 0, 2.44121, 0, 1, 0, 0, 100, 0),
(446400, 8, 1388.9948, 1035.9556, 53.689217, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446400, 9, 1388.2448, 1037.7056, 53.939217, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446400, 10, 1387.4948, 1039.4556, 54.189217, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446400, 11, 1386.3665, 1042.5286, 54.28894, 0, 2.49221, 0, 1, 0, 0, 100, 0),
(446400, 12, 1384.6165, 1043.5286, 54.53894, 0, 2.49884, 0, 1, 0, 0, 100, 0),
(446400, 13, 1382.3665, 1045.0286, 54.28894, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446400, 14, 1380.8665, 1046.2786, 54.03894, 0, 2.50065, 0, 1, 0, 0, 100, 0),
(446400, 15, 1378.9524, 1047.5721, 53.823082, 0, 2.4977417, 0, 1, 0, 0, 100, 0),
(446400, 16, 1377.7024, 1048.3221, 53.823082, 0, 2.49989, 0, 1, 0, 0, 100, 0),
(446400, 17, 1368.2705, 1049.8816, 53.36094, 0, 2.50036795, 0, 1, 0, 0, 100, 0),
(446400, 18, 1360.7705, 1049.1316, 53.11094, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446400, 19, 1356.5281, 1048.5553, 53.031837, 0, 2.5003679, 0, 1, 0, 0, 100, 0),
(446400, 20, 1355.2781, 1042.5553, 52.781837, 0, 2.50065, 0, 1, 0, 0, 100, 0);

-- Horde Hauler
DELETE FROM `waypoint_data` WHERE `id`= 447310;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(447310, 0, 1442.45, 683.194, 47.21392, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 1, 1422.39, 713.509, 47.1535, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 2, 1410.4, 745.214, 47.251, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 3, 1394.08, 784.649, 47.12907, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 4, 1376.23, 828.832, 48.75095, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 5, 1365.51, 856.457, 51.24048, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 6, 1354.39, 887.825, 54.36447, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 7, 1343.64, 925.507, 54.68269, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 8, 1337.94, 956.62, 54.63557, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 9, 1332.58, 998.137, 54.67904, 0, 13.95485, 0, 1, 1, 0, 100, 0),
(447310, 10, 1326.23, 1016.23, 54.71355, 0, 13.95485, 0, 1, 1, 0, 100, 0);

DELETE FROM `waypoint_data_addon` WHERE `PathID`= 447310;
INSERT INTO `waypoint_data_addon` (`PathID`, `PointID`, `SplinePointIndex`, `PositionX`, `PositionY`, `PositionZ`) VALUES
(447310, 0, 0, 1494.085, 627.3845, 47.29598),
(447310, 0, 1, 1480.585, 642.1345, 47.29598),
(447310, 0, 2, 1471.335, 652.1345, 47.04598),
(447310, 0, 3, 1460.585, 663.8845, 47.29598),
(447310, 0, 4, 1457.835, 666.8845, 47.29598),
(447310, 1, 0, 1442.571, 683.4491, 47.38478),
(447310, 2, 0, 1422.417, 713.7669, 47.21889),
(447310, 3, 0, 1410.326, 745.4479, 47.41087),
(447310, 4, 0, 1393.997, 784.8323, 47.45751),
(447310, 4, 1, 1383.747, 810.5823, 47.70751),
(447310, 4, 2, 1381.247, 817.0823, 47.95751),
(447310, 4, 3, 1379.997, 819.8323, 47.95751),
(447310, 4, 4, 1378.997, 822.5823, 48.20751),
(447310, 4, 5, 1378.247, 824.5823, 48.45751),
(447310, 4, 6, 1377.497, 826.3323, 48.70751),
(447310, 5, 0, 1378.819, 822.9114, 48.58068),
(447310, 5, 1, 1378.069, 824.6614, 48.83068),
(447310, 5, 2, 1377.319, 826.6614, 49.08068),
(447310, 5, 3, 1376.319, 829.1614, 49.08068),
(447310, 5, 4, 1374.819, 832.9114, 49.33068),
(447310, 5, 5, 1374.069, 834.6614, 49.58068),
(447310, 5, 6, 1373.319, 836.6614, 49.83068),
(447310, 5, 7, 1372.569, 839.1614, 50.08068),
(447310, 5, 8, 1370.819, 843.6614, 50.33068),
(447310, 5, 9, 1370.069, 845.6614, 50.58068),
(447310, 5, 10, 1369.319, 847.4114, 50.83068),
(447310, 5, 11, 1367.819, 851.1614, 51.08068),
(447310, 5, 12, 1366.319, 854.9114, 51.33068),
(447310, 6, 0, 1373.455, 836.5768, 49.93503),
(447310, 6, 1, 1372.205, 839.3268, 50.18503),
(447310, 6, 2, 1370.455, 844.0768, 50.43503),
(447310, 6, 3, 1369.705, 845.8268, 50.68503),
(447310, 6, 4, 1368.955, 847.8268, 50.93503),
(447310, 6, 5, 1367.455, 851.5768, 51.18503),
(447310, 6, 6, 1366.205, 855.0768, 51.43503),
(447310, 6, 7, 1365.455, 856.8268, 51.68503),
(447310, 6, 8, 1365.205, 857.5768, 51.93503),
(447310, 6, 9, 1364.455, 859.5768, 51.93503),
(447310, 6, 10, 1363.455, 863.0768, 52.18503),
(447310, 6, 11, 1361.705, 867.8268, 52.43503),
(447310, 6, 12, 1360.705, 870.5768, 52.68503),
(447310, 6, 13, 1359.705, 873.3268, 52.93503),
(447310, 6, 14, 1359.455, 874.3268, 53.18503),
(447310, 6, 15, 1357.455, 880.0768, 53.43503),
(447310, 6, 16, 1356.455, 882.8268, 53.68503),
(447310, 6, 17, 1355.705, 884.8268, 53.93503),
(447310, 6, 18, 1355.455, 885.5768, 54.18503),
(447310, 6, 19, 1354.705, 887.5768, 54.43503),
(447310, 7, 0, 1361.398, 867.9194, 52.69784),
(447310, 7, 1, 1360.398, 870.9194, 52.94784),
(447310, 7, 2, 1359.398, 873.6694, 53.19784),
(447310, 7, 3, 1359.148, 874.6694, 53.44784),
(447310, 7, 4, 1357.148, 880.1694, 53.44784),
(447310, 7, 5, 1356.148, 883.1694, 53.69784),
(447310, 7, 6, 1355.398, 884.9194, 53.94784),
(447310, 7, 7, 1355.148, 885.9194, 54.19784),
(447310, 7, 8, 1354.398, 887.9194, 54.44784),
(447310, 7, 9, 1354.398, 888.1694, 54.44784),
(447310, 7, 10, 1352.648, 893.9194, 54.69784),
(447310, 8, 0, 1343.718, 925.8083, 54.89843),
(447310, 9, 0, 1338.005, 956.8096, 54.91347),
(447310, 10, 0, 1332.704, 998.3045, 54.93778);

DELETE FROM `waypoint_data` WHERE `id`=447311;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(447311, 1, 1301.8558, 1057.7549, 54.70073, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447311, 2, 1298.6058, 1063.0049, 54.45073, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447311, 3, 1296.6058, 1066.5049, 54.20073, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447311, 4, 1293.8558, 1069.5049, 53.95073, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 5, 1290.1058, 1074.0049, 53.70073, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 6, 1287.1058, 1077.7549, 53.45073, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 7, 1282.9219, 1082.7007, 53.21805, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 8, 1279.6719, 1086.4507, 52.96805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 9, 1277.9219, 1088.7007, 52.71805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 10, 1274.6719, 1092.4507, 52.46805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 11, 1272.1719, 1095.7007, 52.21805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 12, 1271.4219, 1096.2007, 51.96805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 13, 1269.1719, 1098.2007, 51.96805, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 14, 1249.4678, 1115.2957, 51.412437, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 15, 1247.2178, 1117.2957, 51.162437, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 16, 1245.7178, 1118.5457, 50.912437, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 17, 1242.7178, 1121.0457, 50.912437, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 18, 1215.3215, 1141.2902, 50.76069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 19, 1211.0715, 1144.2902, 50.26069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 20, 1208.8215, 1146.0402, 50.01069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 21, 1201.5715, 1151.0402, 49.76069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 22, 1193.5715, 1156.7902, 49.76069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 23, 1186.0715, 1162.0402, 49.51069, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447311, 24, 1178.995, 1167.1593, 49.256428, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 25, 1173.995, 1170.4093, 49.006428, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 26, 1162.995, 1176.9093, 48.506428, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 27, 1148.3689, 1185.9758, 48.40149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 28, 1145.8689, 1187.4758, 48.15149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 29, 1143.1189, 1189.2258, 48.15149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 30, 1137.1189, 1193.2258, 47.90149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 31, 1133.8689, 1195.4758, 47.65149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 32, 1128.3689, 1198.9758, 47.40149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 33, 1121.6189, 1203.4758, 47.40149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 34, 1116.6189, 1206.7258, 47.15149, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 35, 1108.883, 1211.9009, 46.678432, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 36, 1104.633, 1214.6509, 46.678432, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 37, 1082.9248, 1235.3293, 46.52749, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 38, 1079.1748, 1238.8293, 46.52749, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 39, 1058.2335, 1260.6628, 46.37308, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 40, 1045.2971, 1271.643, 46.26187, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 41, 1031.2971, 1283.393, 46.26187, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 42, 1003.7307, 1301.1025, 46.15664, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 43, 973.5968, 1320.0383, 46.229275, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 44, 966.0968, 1324.7883, 46.229275, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 45, 956.6478, 1330.6003, 46.773254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 46, 946.3978, 1336.8503, 46.773254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 47, 944.8978, 1337.6003, 47.023254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 48, 942.1478, 1338.8503, 47.023254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 49, 936.6478, 1341.1003, 47.273254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 50, 934.1478, 1342.3503, 47.273254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 51, 927.8978, 1345.1003, 47.523254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 52, 924.1478, 1346.6003, 47.773254, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 53, 920.2605, 1348.6997, 48.46779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 54, 916.5105, 1350.1997, 48.71779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 55, 914.7605, 1350.9497, 48.71779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 56, 911.7605, 1351.6997, 48.96779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 57, 907.0105, 1353.1997, 49.21779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 58, 904.2605, 1353.6997, 49.46779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 59, 900.5105, 1354.9497, 49.71779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 60, 895.7605, 1356.1997, 49.96779, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447311, 61, 894.6682, 1356.8383, 50.396946, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447311, 62, 892.6682, 1357.3383, 50.646946, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447311, 63, 890.6682, 1357.8383, 50.896946, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447311, 64, 888.6682, 1358.3383, 51.146946, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 65, 887.9182, 1358.3383, 51.396946, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 66, 885.9182, 1359.0883, 51.896946, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 67, 884.1682, 1359.5883, 51.896946, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 68, 883.4182, 1360.3383, 52.396946, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 69, 881.4182, 1360.8383, 52.396946, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 70, 879.4182, 1361.3383, 52.646946, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 71, 878.6682, 1361.5883, 52.896946, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447311, 72, 876.9182, 1362.0883, 53.146946, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447311, 73, 875.1682, 1362.5883, 53.396946, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447311, 74, 871.9726, 1362.9775, 53.800243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 75, 870.9726, 1362.9775, 54.050243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 76, 868.9726, 1363.2275, 54.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 77, 867.9726, 1363.4775, 54.550243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 78, 866.7226, 1363.4775, 55.050243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 79, 864.4726, 1362.9775, 55.050243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 80, 862.9726, 1362.9775, 55.300243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 81, 862.2226, 1360.7275, 55.300243, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447311, 82, 859.4726, 1358.2275, 55.300243, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447311, 83, 858.9726, 1358.2275, 55.550243, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447311, 84, 854.9726, 1358.4775, 55.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 85, 854.7226, 1358.4775, 55.800243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 86, 853.9726, 1358.4775, 55.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 87, 853.4726, 1358.4775, 55.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 88, 851.7226, 1358.4775, 55.800243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 89, 846.9726, 1358.7275, 55.800243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 90, 846.2226, 1358.7275, 55.800243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 91, 845.7226, 1358.9775, 55.800243, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447311, 92, 845.4726, 1358.9775, 56.050243, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447311, 93, 845.2226, 1358.9775, 56.050243, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447311, 94, 842.2226, 1358.9775, 55.800243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 95, 841.2226, 1358.9775, 56.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 96, 836.9726, 1359.2275, 56.050243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 97, 836.4726, 1359.2275, 56.300243, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447311, 98, 836.2226, 1359.2275, 56.050243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 99, 834.7226, 1359.2275, 56.300243, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 100, 831.84766, 1359.3313, 56.550697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 101, 827.09766, 1359.5813, 56.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 102, 824.09766, 1359.8313, 56.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 103, 822.84766, 1359.8313, 57.050697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 104, 820.09766, 1360.0813, 56.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 105, 819.34766, 1360.0813, 56.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 106, 817.34766, 1360.3313, 57.300697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 107, 816.34766, 1360.3313, 57.550697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 108, 814.34766, 1360.5813, 57.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 109, 812.34766, 1360.5813, 58.300697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 110, 810.34766, 1360.5813, 58.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 111, 808.34766, 1360.8313, 59.300697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 112, 807.59766, 1360.8313, 59.550697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 113, 804.59766, 1360.8313, 59.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 114, 802.59766, 1360.8313, 59.800697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 115, 801.59766, 1360.8313, 60.050697, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 116, 799.20886, 1361.2817, 60.620926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 117, 798.20886, 1361.2817, 60.870926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 118, 796.20886, 1361.2817, 61.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 119, 795.20886, 1361.2817, 61.370926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 120, 793.20886, 1361.5317, 61.870926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 121, 792.20886, 1361.5317, 62.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 122, 790.20886, 1361.5317, 62.370926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 123, 788.20886, 1361.5317, 62.870926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 124, 786.20886, 1361.7817, 63.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 125, 784.20886, 1361.7817, 63.370926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 126, 783.20886, 1361.7817, 63.620926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 127, 781.70886, 1361.7817, 63.870926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 128, 780.70886, 1361.7817, 64.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 129, 778.70886, 1362.0317, 64.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 130, 776.95886, 1362.0317, 64.620926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 131, 774.95886, 1362.0317, 64.870926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 132, 773.95886, 1362.0317, 65.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 133, 771.95886, 1362.2817, 65.620926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 134, 769.95886, 1362.2817, 66.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 135, 766.95886, 1362.5317, 66.370926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 136, 765.95886, 1362.5317, 66.620926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 137, 763.95886, 1362.7817, 67.120926, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 138, 761.7096, 1362.7507, 67.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 139, 760.7096, 1362.7507, 67.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 140, 758.7096, 1363.0007, 68.40903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 141, 756.7096, 1363.0007, 68.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 142, 756.4596, 1363.0007, 68.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 143, 755.4596, 1363.0007, 68.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 144, 753.4596, 1362.5007, 69.15903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 145, 752.4596, 1362.2507, 69.40903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 146, 750.7096, 1361.7507, 69.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 147, 748.7096, 1361.2507, 69.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 148, 746.7096, 1360.7507, 69.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 149, 744.7096, 1360.2507, 70.15903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 150, 742.9596, 1359.7507, 70.40903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 151, 741.2096, 1359.2507, 70.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 152, 739.2096, 1359.0007, 70.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 153, 737.2096, 1358.5007, 71.40903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 154, 735.2096, 1358.2507, 71.65903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 155, 733.4596, 1357.7507, 71.90903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 156, 731.4596, 1357.2507, 72.15903, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 157, 728.2733, 1356.1975, 72.75487, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 158, 726.2733, 1355.6975, 73.00487, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 159, 725.5233, 1355.1975, 73.25487, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 160, 723.7733, 1354.1975, 73.75487, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 161, 721.2733, 1352.6975, 74.00487, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 162, 719.62805, 1351.5182, 74.523605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 163, 716.87805, 1350.0182, 74.773605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 164, 715.12805, 1349.0182, 75.023605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 165, 714.37805, 1348.5182, 75.273605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 166, 712.62805, 1347.5182, 75.523605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 167, 710.87805, 1346.5182, 76.023605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 168, 709.37805, 1345.5182, 76.273605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 169, 707.62805, 1344.2682, 76.523605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 170, 706.62805, 1343.7682, 76.773605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 171, 705.12805, 1342.5182, 77.023605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 172, 703.62805, 1341.0182, 77.273605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 173, 702.87805, 1340.5182, 77.523605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 174, 700.87805, 1338.5182, 77.773605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 175, 698.62805, 1336.5182, 78.273605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 176, 697.12805, 1335.2682, 78.523605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 177, 694.87805, 1333.2682, 78.773605, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 178, 692.52, 1331.0784, 79.35908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 179, 690.27, 1329.0784, 79.60908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 180, 687.27, 1326.3284, 79.85908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 181, 685.02, 1324.3284, 80.10908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 182, 684.77, 1324.0784, 80.10908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 183, 683.02, 1323.0784, 80.35908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 184, 679.52, 1321.0784, 80.60908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 185, 678.02, 1319.8284, 80.85908, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 186, 675.3156, 1318.2737, 80.93302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 187, 673.5656, 1317.2737, 81.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 188, 671.0656, 1315.7737, 81.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 189, 669.3156, 1314.7737, 81.68302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 190, 666.8156, 1313.2737, 82.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 191, 665.0656, 1312.0237, 82.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 192, 663.5656, 1311.0237, 82.93302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 193, 660.8156, 1309.5237, 83.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 194, 660.0656, 1309.0237, 83.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 195, 658.3156, 1308.2737, 83.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 196, 657.0656, 1307.2737, 83.68302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 197, 655.0656, 1306.7737, 83.93302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 198, 652.5656, 1305.5237, 84.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 199, 651.8156, 1305.2737, 84.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 200, 649.8156, 1304.5237, 84.68302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 201, 648.0656, 1303.7737, 84.93302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 202, 645.0656, 1302.7737, 85.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 203, 643.3156, 1302.0237, 85.43302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 204, 640.5656, 1301.0237, 85.68302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 205, 637.5656, 1300.0237, 85.93302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 206, 634.8156, 1298.7737, 86.18302, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447311, 207, 632.7715, 1297.8721, 86.28932, 0, 13.9246, 0, 1, 1, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=447312;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(447312, 1, 631.221, 1296.77, 86.67, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 2, 628.721, 1294.77, 86.92, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 3, 624.721, 1291.77, 87.17, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 4, 617.721, 1286.52, 87.42, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 5, 589.807, 1265.23, 87.2883, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 6, 588.057, 1264.23, 87.2883, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 7, 575.969, 1253.85, 87.2096, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 8, 568.969, 1248.1, 86.9596, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 9, 563.969, 1244.1, 86.7096, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 10, 559.273, 1241.63, 86.7139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 11, 552.023, 1238.38, 86.9639, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 12, 550.273, 1237.38, 87.2139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 13, 548.523, 1236.63, 87.4639, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 14, 546.523, 1235.88, 87.7139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 15, 543.023, 1234.38, 87.7139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 16, 541.273, 1233.38, 87.9639, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 17, 539.273, 1232.63, 88.2139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 18, 537.783, 1232.48, 88.0761, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 19, 536.023, 1232.13, 88.4639, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 20, 534.273, 1231.88, 88.7139, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 21, 532.273, 1231.88, 88.9639, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 22, 528.076, 1231.21, 89.2745, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 23, 526.076, 1231.21, 89.5245, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 24, 510.076, 1230.21, 89.2745, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 25, 500.326, 1229.96, 89.0245, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 26, 486.016, 1229.78, 88.9304, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 27, 473.766, 1229.78, 88.6804, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 28, 465.766, 1230.28, 88.4304, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 29, 453.016, 1230.78, 88.1804, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 30, 446.838, 1231.2, 87.8797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 31, 441.838, 1231.45, 87.6297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 32, 438.838, 1231.45, 87.3797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 33, 435.838, 1231.7, 87.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 34, 433.838, 1231.7, 86.8797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 35, 430.838, 1231.95, 86.6297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 36, 428.838, 1231.95, 86.3797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 37, 426.838, 1232.2, 86.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 38, 426.588, 1232.2, 86.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 39, 423.588, 1232.45, 85.8797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 40, 421.588, 1232.45, 85.6297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 41, 419.588, 1232.7, 85.3797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 42, 417.588, 1232.95, 85.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 43, 415.588, 1233.2, 85.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 44, 412.838, 1233.45, 84.8797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 45, 410.838, 1233.45, 84.6297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 46, 407.838, 1233.7, 84.3797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 47, 405.838, 1233.95, 84.1297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 48, 402.838, 1234.2, 83.8797, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 49, 400.838, 1234.45, 83.6297, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 50, 397.535, 1234.75, 83.0801, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 51, 394.535, 1235.25, 82.8301, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 52, 387.535, 1235.75, 82.5801, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 53, 381.035, 1236.5, 82.5801, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 54, 371.535, 1238.5, 82.3301, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 55, 365.503, 1240.08, 82.0813, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 56, 357.503, 1242.08, 81.8313, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 57, 354.003, 1242.83, 81.5813, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 58, 346.503, 1245.33, 81.3313, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 59, 341.753, 1246.83, 81.0813, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 60, 335.253, 1248.58, 80.8313, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 61, 331.503, 1249.83, 80.8313, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 62, 325.377, 1251.99, 80.4257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 63, 319.627, 1253.74, 80.1757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 64, 316.127, 1254.74, 79.9257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 65, 313.377, 1255.99, 79.9257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 66, 309.627, 1257.24, 79.6757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 67, 306.627, 1258.24, 79.4257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 68, 303.877, 1259.24, 79.1757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 69, 302.127, 1259.74, 78.9257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 70, 298.377, 1260.99, 78.6757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 71, 296.627, 1261.74, 78.6757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 72, 293.877, 1262.74, 78.4257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 73, 291.877, 1263.49, 78.1757, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 74, 289.127, 1264.49, 77.9257, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 75, 286.092, 1265.6, 77.2982, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 76, 279.342, 1268.1, 77.0482, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 77, 276.092, 1269.1, 77.0482, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 78, 266.342, 1270.1, 77.0482, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 79, 252.192, 1271.39, 76.9999, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 80, 248.192, 1271.14, 76.7499, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 81, 244.111, 1270.81, 76.4318, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 82, 238.111, 1270.31, 76.1818, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 83, 234.111, 1270.06, 75.9318, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 84, 229.361, 1269.56, 75.9318, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 85, 228.361, 1269.56, 75.6818, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 86, 226.361, 1268.81, 75.4318, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 87, 224.611, 1268.31, 75.1818, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 88, 221.861, 1267.56, 75.1818, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 89, 212.984, 1265.44, 74.8537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 90, 207.984, 1264.19, 74.6037, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 91, 204.234, 1263.19, 74.3537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 92, 200.234, 1262.19, 74.1037, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 93, 196.984, 1261.44, 73.8537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 94, 192.984, 1260.44, 73.6037, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 95, 190.234, 1259.69, 73.3537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 96, 189.484, 1259.44, 73.1037, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 97, 187.484, 1259.19, 72.8537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 98, 185.734, 1258.44, 72.8537, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 99, 181.616, 1257.19, 72.3178, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 100, 178.616, 1256.44, 72.0678, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 101, 176.616, 1255.94, 71.8178, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 102, 173.866, 1255.19, 71.5678, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 103, 169.866, 1254.19, 71.5678, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 104, 165.116, 1252.69, 71.5678, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 105, 156.616, 1250.19, 71.5678, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 106, 132.667, 1242.67, 71.1781, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 107, 129.667, 1241.92, 70.9281, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 108, 129.417, 1241.67, 70.9281, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 109, 126.417, 1240.92, 70.6781, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 110, 122.917, 1239.92, 70.4281, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 111, 121.577, 1239.66, 70.3957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 112, 120.827, 1239.16, 70.1457, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 113, 117.827, 1238.41, 69.8957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 114, 115.077, 1237.41, 69.6457, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 115, 107.327, 1235.16, 69.3957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 116, 102.577, 1233.66, 69.1457, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 117, 99.8275, 1232.91, 68.8957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 118, 98.5775, 1232.66, 68.8957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 119, 96.5775, 1232.16, 68.6457, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 120, 93.5775, 1231.16, 68.6457, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 121, 90.8275, 1230.41, 68.3957, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 122, 51.8593, 1219.93, 66.3207, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 123, 48.1093, 1219.18, 66.0707, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 124, 42.1093, 1217.68, 66.0707, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 125, 36.1093, 1215.93, 65.8207, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 126, 33.3593, 1215.18, 65.5707, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 127, 24.8593, 1212.93, 65.5707, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 128, 22.1093, 1212.18, 65.3207, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 129, 6.78019, 1207.54, 65.0166, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 130, -3.21981, 1204.04, 64.7666, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 131, -22.9334, 1196.85, 64.8054, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 132, -28.4334, 1194.6, 64.5554, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 133, -46.9757, 1187.27, 64.3674, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 134, -55.2257, 1184.02, 64.3674, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 135, -74.1176, 1186.06, 64.1428, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 136, -79.3676, 1190.31, 63.8928, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 137, -82.3676, 1193.06, 63.6428, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 138, -85.6176, 1195.31, 63.3928, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 139, -87.8676, 1197.06, 63.1428, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 140, -90.5064, 1199.33, 62.8232, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 141, -93.5064, 1201.83, 62.5732, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 142, -94.2564, 1202.58, 62.3232, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 143, -97.5064, 1205.08, 61.8232, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 144, -98.2564, 1205.33, 61.8232, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 145, -99.5064, 1206.33, 61.5732, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 146, -100.506, 1206.83, 61.5732, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 147, -102.129, 1207.71, 61.2121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 148, -103.879, 1208.71, 60.9621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 149, -105.629, 1209.71, 60.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 150, -108.379, 1211.21, 60.2121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 151, -110.129, 1211.96, 59.7121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 152, -111.879, 1212.96, 59.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 153, -113.629, 1213.71, 58.9621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 154, -115.379, 1214.71, 58.7121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 155, -117.129, 1215.71, 58.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 156, -119.629, 1217.21, 58.2121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 157, -121.129, 1217.96, 57.9621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 158, -122.379, 1218.71, 57.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 159, -123.379, 1218.71, 57.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 160, -127.379, 1219.21, 57.2121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 161, -129.379, 1219.46, 56.9621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 162, -133.379, 1219.71, 56.7121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 163, -134.379, 1219.96, 56.4621, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 164, -139.129, 1220.46, 56.2121, 0, 14, 0, 1, 1, 0, 100, 0),
(447312, 165, -140.623, 1220.49, 55.7702, 0, 14, 0, 1, 1, 0, 100, 0);

DELETE FROM `waypoint_data` WHERE `id`=447313;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `velocity`, `delay`, `smoothTransition`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(447313, 1, -145.31, 1219.095, 56.059937, 0, 13.9878, 0, 1, 1, 0, 100, 0),
(447313, 2, -146.31, 1218.845, 55.809937, 0, 13.987, 0, 1, 1, 0, 100, 0),
(447313, 3, -152.06, 1217.095, 56.059937, 0, 13.9967, 0, 1, 1, 0, 100, 0),
(447313, 4, -154.06, 1216.595, 56.309937, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447313, 5, -157.81, 1215.345, 56.809937, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447313, 6, -159.81, 1214.845, 56.559937, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447313, 7, -160.81, 1214.595, 56.809937, 0, 13.9845, 0, 1, 1, 0, 100, 0),
(447313, 8, -162.56, 1213.845, 56.559937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 9, -163.56, 1213.595, 56.309937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 10, -164.56, 1213.345, 56.559937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 11, -167.56, 1212.595, 56.309937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 12, -168.31, 1212.345, 56.559937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 13, -171.31, 1211.345, 56.809937, 0, 13.9246, 0, 1, 1, 0, 100, 0),
(447313, 14, -175.06, 1210.345, 57.059937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 15, -179.06, 1209.095, 57.809937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 16, -181.56, 1208.595, 58.559937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 17, -185.56, 1207.345, 58.809937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 18, -189.31, 1206.345, 59.059937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 19, -192.06, 1205.345, 59.559937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 20, -196.06, 1204.345, 59.809937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 21, -199.81, 1203.095, 60.309937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 22, -202.81, 1202.345, 60.809937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 23, -206.56, 1201.095, 61.059937, 0, 13.9271, 0, 1, 1, 0, 100, 0),
(447313, 24, -210.31, 1200.095, 61.309937, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 25, -222.52264, 1194.4482, 63.39486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 26, -225.02264, 1192.6982, 63.89486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 27, -234.02264, 1186.6982, 64.14486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 28, -237.77264, 1184.1982, 64.14486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 29, -240.77264, 1184.4482, 64.39486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 30, -252.52264, 1185.1982, 64.14486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 31, -273.52264, 1186.4482, 63.89486, 0, 13.9747, 0, 1, 1, 0, 100, 0),
(447313, 32, -277.264, 1186.42, 63.713806, 0, 13.9747, 0, 1, 1, 0, 100, 0);

