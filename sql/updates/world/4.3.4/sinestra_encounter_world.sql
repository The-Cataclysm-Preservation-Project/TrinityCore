-- Correct unit_flag from Pulsing Twilight Eggs
UPDATE `creature_template` SET `unit_flags` = '0' WHERE `creature_template`.`entry` = 46842;

-- Correct unit_flag and ScriptName from Twilight Drakes
UPDATE `creature_template` SET `unit_flags` = '0',  `ScriptName` = 'npc_sinestra_add' WHERE `creature_template`.`entry` = 55636;

-- Correct ScriptName from Spitcallers
UPDATE `creature_template` SET `ScriptName` = 'npc_sinestra_add' WHERE `creature_template`.`entry` = 48415;

-- Correct ScriptName from Twilight Sinestra Whelps
UPDATE `creature_template` SET `ScriptName` = 'npc_sinestra_twilight_whelp' WHERE `creature_template`.`entry` = 48050;

-- Correct Sinestra ScriptName and unit_flag 
UPDATE `creature_template` SET `unit_flags` = '64', `ScriptName` = 'boss_sinestra' WHERE `creature_template`.`entry` = 45213;

-- Make flask not attackable/selectable
UPDATE `creature_template` SET `unit_flags` = '33554560', `flags_extra` = '128', `speed_run` = '0' , `speed_walk` = '0' WHERE `creature_template`.`entry` = 48018;

-- Remove existings Eggs , Calen and Flames trigger
DELETE FROM `creature` WHERE `id` IN(46277, 46842, 51629);

-- Delete existing loot chest
DELETE FROM `gameobject` WHERE `id` = 208045;