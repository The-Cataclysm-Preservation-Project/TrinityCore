DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
'spell_item_nokaled_the_elements_of_death_lfr',
'spell_item_nokaled_the_elements_of_death_normal',
'spell_item_nokaled_the_elements_of_death_heroic');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109866, 'spell_item_nokaled_the_elements_of_death_lfr'),
(107786, 'spell_item_nokaled_the_elements_of_death_normal'),
(109873, 'spell_item_nokaled_the_elements_of_death_heroic');
