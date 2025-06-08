DROP TABLE IF EXISTS `spell_bonus_data`;
DELETE FROM `command` WHERE `name`= 'reload spell_bonus_data';

DELETE FROM `spell_script_names` WHERE `ScriptName` IN
('spell_hun_arcane_shot',
'spell_hun_steady_shot',
'spell_rog_hemorrhage',
'spell_dk_icy_touch');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(3044, 'spell_hun_arcane_shot'),
(56641, 'spell_hun_steady_shot'),
(16511, 'spell_rog_hemorrhage'),
(45477, 'spell_dk_icy_touch');
