DELETE FROM `spell_script_names` WHERE `ScriptName`= 'spell_dru_pounce' OR `ScriptName`= 'spell_dru_ravage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(9005, 'spell_dru_pounce'), (6785, 'spell_dru_ravage');