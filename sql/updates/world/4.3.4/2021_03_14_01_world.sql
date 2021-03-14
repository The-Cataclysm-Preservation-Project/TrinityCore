/*Through Fire and Flames https://www.wowhead.com/quest=13878/through-fire-and-flames*/
/*Chain Origin (Trigger Bunny) 54324 */
UPDATE `creature` SET `spawntimesecs` = 90 WHERE `id` = 34287;
UPDATE `creature_template` SET `AIName` = 'SmartAI' ,`flags_extra`=128 WHERE `entry`= 34287;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-304052,-304053,-304051);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-304052, 0, 0, 0, 1, 0, 100, 0, 1, 1, 10, 12, 11, 65072, 4, 0, 0, 0, 0, 10, 304050, 34285, 0, 0, 0, 0, 0, 'Trigger Bunny Cast Chains to Trapped Wolf'),
(-304053, 0, 0, 0, 1, 0, 100, 0, 1, 1, 10, 12, 11, 65072, 4, 0, 0, 0, 0, 10, 304049, 34285, 0, 0, 0, 0, 0, 'Trigger Bunny Cast Chains to Trapped Wolf'),
(-304051, 0, 0, 0, 1, 0, 100, 0, 1, 1, 10, 12, 11, 65072, 4, 0, 0, 0, 0, 10, 304048, 34285, 0, 0, 0, 0, 0, 'Trigger Bunny Cast Chains to Trapped Wolf');

/*Wolf Chains (gObjects)*/
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (195001,195003,195004);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (195001,195003,195004);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(195001, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 304049, 34285, 0, 0, 0, 0, 0, 'Wolf Chains On Gossip Hello Set Data On Trigger Bunny'),
(195003, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 304048, 34285, 0, 0, 0, 0, 0, 'Wolf Chains On Gossip Hello Set Data 0n Trigger Bunny'),
(195004, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 304050, 34285, 0, 0, 0, 0, 0, 'Wolf Chains On Gossip Hello Set Data On Trigger Bunny');

/*Trapped Wolf (NPC)*/
UPDATE `creature_template` SET `AIName` = 'SmartAI'  WHERE `entry`= 34285;
UPDATE `creature` SET `spawntimesecs` = 90 WHERE `id` = 34285;
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (-304048,-304049,-304050,30404800,30404801,30404802);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(-304048, 0, 0, 0, 38, 0, 100, 1, 1, 1, 0, 0, 0, 80, 30404800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf on Data Set Run'),
(-304048, 0, 1, 0, 40, 0, 100, 0, 3, 34285, 0, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Despawn'),
(30404800, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 10, 304051, 34287, 0, 0, 0, 0, 0, 'Kill The Bunny To Reset'),
(30404800, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 84277, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dust Animation'),
(30404800, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 97, 10, 10, 0, 0, 0, 0, 1, 0, 0, 0, -5.18029, -3628.84, 27.3361, 0, 'On respawn  - None: Jump to pos (-5.18029, -3628.84, 27.3361, 0) with speed XY 0 and speed Z 0'),
(30404800, 9, 4, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, 53, 1, 34285, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf WP Start'),
(30404800, 9, 5, 0, 0, 0, 100, 0, 1, 1, 1, 1, 0, 33, 34285, 0, 0, 0, 0, 0, 21, 7, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Quest Credit'),
(-304049, 0, 0, 0, 38, 0, 100, 1, 1, 1, 0, 0, 0, 80, 30404801, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf on Data Set Run'),
(-304049, 0, 1, 0, 40, 0, 100, 0, 3, 34285, 0, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Despawn'),
(30404801, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 10, 304053, 34287, 0, 0, 0, 0, 0, 'Kill The Bunny To Reset'),
(30404801, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 84277, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dust Animation'),
(30404801, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 97, 10, 10, 0, 0, 0, 0, 1, 0, 0, 0, -5.18029, -3628.84, 27.3361, 0, 'On respawn  - None: Jump to pos (-5.18029, -3628.84, 27.3361, 0) with speed XY 0 and speed Z 0'),
(30404801, 9, 4, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, 53, 1, 34285, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf WP Start'),
(30404801, 9, 5, 0, 0, 0, 100, 0, 1, 1, 1, 1, 0, 33, 34285, 0, 0, 0, 0, 0, 21, 7, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Quest Credit'),
(-304050, 0, 0, 0, 38, 0, 100, 1, 1, 1, 0, 0, 0, 80, 30404802, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf on Data Set Run'),
(-304050, 0, 1, 0, 40, 0, 100, 0, 3, 34285, 0, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Despawn'),
(30404802, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 10, 304052, 34287, 0, 0, 0, 0, 0, 'Kill The Bunny To Reset'),
(30404802, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 84277, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dust Animation'),
(30404802, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 97, 10, 10, 0, 0, 0, 0, 1, 0, 0, 0, -5.18029, -3628.84, 27.3361, 0, 'On respawn  - None: Jump to pos (-5.18029, -3628.84, 27.3361, 0) with speed XY 0 and speed Z 0'),
(30404802, 9, 4, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, 53, 1, 34285, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf WP Start'),
(30404802, 9, 5, 0, 0, 0, 100, 0, 1, 1, 1, 1, 0, 33, 34285, 0, 0, 0, 0, 0, 21, 7, 0, 0, 0, 0, 0, 0, 'Trapped Wolf Quest Credit');


DELETE FROM `waypoints` WHERE `entry` = 34285;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(34285, 1, 4.02982, -3620.66, 27.2415, 'Trapped Wolf WP'),
(34285, 2, 56.7787, -3615.88, 29.6103, 'Trapped Wolf WP'),
(34285, 3, 56.8, -3615.9, 29.7103, 'Trapped Wolf WP');


/*Burning Farm Fire Effect*/
UPDATE `creature_template` SET `AIName` = 'SmartAI', `flags_extra`=128 WHERE `entry`= 34280;
DELETE FROM `smart_scripts` WHERE `entryorguid` =34280;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(34280, 0, 0, 0, 1, 0, 100, 0, 1, 2, 25000, 25000, 11, 80175, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Barrens Burning Farm Fire Effect');

/*Dead Peon*/
UPDATE `creature_template` SET `npcflag` = 0, `unit_flags` = 537133568, `unit_flags2` = 2049, `dynamicflags` = 2049, `flags_extra` = 2 WHERE `entry` = 14901;

