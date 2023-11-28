DELETE FROM `areatrigger_scripts` WHERE `entry` IN (5542, 5543, 5544, 5545, 5546, 5547, 5548, 5549, 5550, 5551, 5552, 5553, 5554, 5567, 5568, 5574, 5575, 5576, 5577);

INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5542, 'at_gasping_for_breath'),
(5543, 'at_gasping_for_breath'),
(5544, 'at_gasping_for_breath'),
(5545, 'at_gasping_for_breath'),
(5546, 'at_gasping_for_breath'),
(5547, 'at_gasping_for_breath'),
(5548, 'at_gasping_for_breath'),
(5549, 'at_gasping_for_breath'),
(5550, 'at_gasping_for_breath'),
(5551, 'at_gasping_for_breath'),
(5552, 'at_gasping_for_breath'),
(5553, 'at_gasping_for_breath'),
(5554, 'at_gasping_for_breath'),
(5567, 'at_gasping_for_breath'),
(5568, 'at_gasping_for_breath'),
(5574, 'at_gasping_for_breath'),
(5575, 'at_gasping_for_breath'),
(5576, 'at_gasping_for_breath'),
(5577, 'at_gasping_for_breath');

DELETE FROM `spell_script_names` WHERE `spell_id`=68737;
DELETE FROM `spell_script_names` WHERE `spell_id`=68741;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(68737, 'spell_gilneas_save_drowning_milita_effect'),
(68741, 'spell_gilneas_drowning_vehicle_exit_dummy');

UPDATE `creature_text` SET `BroadcastTextId`='36418' WHERE  `CreatureID`=36440 AND `GroupID`=0 AND `ID`=0;
UPDATE `creature_text` SET `GroupID`='0', `ID`='1', `BroadcastTextId`='36417' WHERE  `CreatureID`=36440 AND `GroupID`=1 AND `ID`=0;
UPDATE `creature_text` SET `GroupID`='0', `ID`='3', `BroadcastTextId`='36419' WHERE  `CreatureID`=36440 AND `GroupID`=2 AND `ID`=0;

UPDATE `creature` SET `terrainSwapMap`='655' WHERE  `guid` IN (255797, 255810, 255819, 255821, 255823, 255825, 255833, 255837, 255843, 255844, 255853);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 36440;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(36440, 68735, 1, 0);
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 18) AND (`SourceGroup` IN (36440));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(18, 36440, 68735, 0, 0, 9, 0, 14395, 0, 0, 0, 'Clicker has quest Gasping for Breath (14395) active'),
(18, 36440, 68735, 0, 0, 28, 0, 14395, 0, 0, 1, 'Clicker hasn\'t completed quest Gasping for Breath (14395) (or completed and rewarded)');

 -- Drowning Watchman smart ai
SET @ENTRY := 36440;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(@ENTRY, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 11, 47020, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellclick - Self: Cast spell  Ride Vehicle (47020) on Clicker'),
(@ENTRY, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On spellclick - Self: Remove npc flags SPELLCLICK'),
(@ENTRY, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 68730, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On spellclick - Self: Remove aura due to spell  Drowning (68730)'),
(@ENTRY, 0, 3, 4, 8, 0, 100, 1, 68739, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On spell  Drowning Militia Dummy (68739) hit - Self: Despawn in 10 s'),
(@ENTRY, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 67, 1, 1500, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'On spell  Drowning Militia Dummy (68739) hit - Trigger timed event timedEvent[1] in 1500 - 1500 ms'),
(@ENTRY, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 67, 2, 3000, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'On spell  Drowning Militia Dummy (68739) hit - Trigger timed event timedEvent[2] in 3000 - 3000 ms'),
(@ENTRY, 0, 6, 0, 59, 0, 30, 0, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On timed event timedEvent[1] triggered - Self: Set stand state to KNEEL'),
(@ENTRY, 0, 7, 0, 59, 0, 100, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On timed event timedEvent[2] triggered - Self: Talk I... I thought I was a goner.  Thanks. (0) to invoker');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 36440 AND `SourceId` = 0;
