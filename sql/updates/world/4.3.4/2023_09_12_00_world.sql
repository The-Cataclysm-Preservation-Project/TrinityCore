 -- Bloodfang Worgen smart ai
SET @ENTRY := 35118;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = @ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(@ENTRY, 0, 0, 0, 2, 0, 100, 1, 0, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'When health between 0%-30%% (once) - Self: Cast spell  Enrage (8599) on Self // Bloodfang Worgen - Between 0-30% Health - Cast \'Enrage\' (No Repeat)'),
(@ENTRY, 0, 1, 0, 8, 0, 100, 0, 5143, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spell  Arcane Missiles (5143) hit - Caster: Give kill credit Spell Practice Credit (44175)'),
(@ENTRY, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spell  Eviscerate (2098) hit - Caster: Give kill credit Spell Practice Credit (44175)'),
(@ENTRY, 0, 3, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spell  Steady Shot (56641) hit - Caster: Give kill credit Spell Practice Credit (44175)'),
(@ENTRY, 0, 4, 0, 8, 0, 100, 0, 348, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spell  Immolate (348) hit - Caster: Give kill credit Spell Practice Credit (44175)'),
(@ENTRY, 0, 5, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spell  Charge (100) hit - Caster: Give kill credit Spell Practice Credit (44175)');


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 35118 AND `SourceId` = 0;
