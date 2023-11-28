DELETE FROM `spell_script_names` WHERE `ScriptName` IN 
('spell_pri_silence',
'spell_dk_strangulate',
'spell_gen_arcane_torrent_racial');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(15487, 'spell_pri_silence'),
(47476, 'spell_dk_strangulate'),
(25046, 'spell_gen_arcane_torrent_racial'),
(28730, 'spell_gen_arcane_torrent_racial'),
(50613, 'spell_gen_arcane_torrent_racial'),
(69179, 'spell_gen_arcane_torrent_racial'),
(80483, 'spell_gen_arcane_torrent_racial');
