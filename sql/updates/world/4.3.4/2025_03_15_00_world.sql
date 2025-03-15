UPDATE `creature` SET `unit_flags`= `unit_flags` & ~0x8000 WHERE `guid` IN (110369, 110370, 110371, 110378, 110379, 110380, 110381, 110382, 110383, 110384, 110386, 110389, 110390, 110540, 110541, 110544, 110545, 110546, 110547, 110551, 110552, 110553, 110554);
UPDATE `creature_template` SET `flags_extra`= `flags_extra` & ~65536 WHERE `entry` IN (58862, 58863, 58864, 49975, 49981, 49987, 48940, 54056, 54057, 41841);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (29266, 80636) AND `ScriptName`= 'spell_gen_feign_death_all_flags_uninteractible'; 
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(29266, 'spell_gen_feign_death_all_flags_uninteractible'),
(80636, 'spell_gen_feign_death_all_flags_uninteractible');
