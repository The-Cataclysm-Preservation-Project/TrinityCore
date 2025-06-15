UPDATE `spell_script_names` SET `ScriptName`= 'spell_dk_killing_machine_triggered' WHERE `spell_id`= 51124;
DELETE FROM `spell_proc` WHERE `SpellId` IN (51123, 51127, 51128);
INSERT INTO `spell_proc` (`SpellId`, `ProcsPerMinute`) VALUES
(51123, 1),
(51127, 3),
(51128, 5);

DELETE FROM `spell_script_names` WHERE `ScriptName`= 'spell_dk_killing_machine';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(-51123, 'spell_dk_killing_machine');
