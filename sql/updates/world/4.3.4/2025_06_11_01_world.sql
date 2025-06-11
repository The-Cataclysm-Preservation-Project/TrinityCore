DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dk_blood_strike', 'spell_dk_obliterate', 'spell_dk_heart_strike');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(45902, 'spell_dk_blood_strike'),
(49020, 'spell_dk_obliterate'),
(66198, 'spell_dk_obliterate'),
(55050, 'spell_dk_heart_strike');
