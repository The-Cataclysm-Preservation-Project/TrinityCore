INSERT INTO `conditions` VALUES (15, 6944, 1, 0, 0, 47, 0, 26998, 8, 0, 0, 0, 0, '', 'Bat Handler Maggotbreath - Show gossip if quest 26998 is active');
INSERT INTO `conditions` VALUES (18, 44894, 83756, 0, 0, 47, 0, 27045, 8, 0, 0, 0, 0, '', 'Allow spellclick if quest 27045 is active');
INSERT INTO `conditions` VALUES (13, 1, 83464, 0, 2, 31, 0, 3, 44737, 0, 0, 0, 0, '', 'Chain Visual Left - Target Subdued Forest Ettin');
INSERT INTO `conditions` VALUES (13, 1, 83467, 0, 2, 31, 0, 3, 44737, 0, 0, 0, 0, '', 'Chain Visual Right - Target Subdued Forest Ettin');
INSERT INTO `conditions` VALUES (13, 1, 83763, 0, 0, 31, 0, 3, 44883, 0, 0, 0, 0, '', 'Armoire Camera - Target Crowley');
INSERT INTO `conditions` VALUES (13, 1, 83764, 0, 0, 31, 0, 3, 44884, 0, 0, 0, 0, '', 'Armoire Camera - Target Ivar');

 -- Dark Ranger
SET @CGUID := 399000;
INSERT INTO `creature` VALUES (@CGUID, 44632, 0, 130, 5369, 1, 0, 1, 169, 0, -1, 0, 0, 1360.47, 1063.27, 52.9999, 3.92421, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0);
INSERT INTO `creature` VALUES (@CGUID+1, 44632, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1428.24, 1032.64, 53.0474, 4.06429, 300, 0, 0, 1398, 0, 2, 0, 0, 0, NULL, 0);

INSERT INTO `creature_addon` VALUES (@CGUID, 446320, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL);
INSERT INTO `creature_addon` VALUES (@CGUID+1, 446321, 0, 0, 0, 2, 0, 0, 0, 0, 0, NULL);

 -- High Warlord Cromush
INSERT INTO `creature` VALUES (@CGUID+2, 44640, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1357.21, 1046.36, 52.7554, 4.46892, 300, 0, 0, 387450, 0, 2, 0, 0, 0, 'npc_high_warlord_cromush_44640', 0);

INSERT INTO `creature_addon` VALUES (@CGUID+2, 446400, 0, 0, 0, 1, 0, 0, 0, 0, 3, '80697');

 -- Worg
SET @ENTRY := 1765;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Ferocious Grizzled Bear
SET @ENTRY := 1778;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Giant Rabid Bear
SET @ENTRY := 1797;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Rabid Dog
SET @ENTRY := 1766;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Vile Fin Oracle
SET @ENTRY := 1908;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Vile Fin Tidehunter
SET @ENTRY := 1768;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Skitterweb Lurker
SET @ENTRY := 1781;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Skitterweb Lurker
SET @ENTRY := 1781;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Bloodfang Scavenger
SET @ENTRY := 44547;
UPDATE `creature` SET `MovementType`=1 WHERE `entry`=@ENTRY;
UPDATE `creature` SET `spawndist`=10 WHERE `entry`=@ENTRY;

 -- Bat Handler Maggotbreath
INSERT INTO `creature` VALUES (@CGUID+3, 44825, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1423.08, 1016.46, 52.544, 2.90902, 300, 0, 0, 77490, 0, 0, 0, 0, 0, NULL, 0);

 -- Deathstalker Commander Belmont
INSERT INTO `creature` VALUES (@CGUID+4, 44789, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1415.07, 1010.24, 52.7315, 2.09687, 300, 0, 0, 2085, 0, 0, 0, 0, 0, NULL, 0);

-- High Apothecary Shana T'veen
INSERT INTO `creature` VALUES (@CGUID+5, 44784, 0, 0, 0, 1, 0, 1, 169, 0, -1, 0, 1, 1431.55, 1086.89, 60.4768, 3.81387, 300, 0, 0, 276, 590, 0, 0, 0, 0, NULL, 0);

REPLACE INTO `creature_equip_template` VALUES (44365, 1, 2179, 0, 42775, 15595);
REPLACE INTO `creature_equip_template` VALUES (44640, 1, 47031, 0, 0, 18019);
REPLACE INTO `creature_equip_template` VALUES (44791, 1, 1481, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44791, 2, 11121, 9661, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44791, 3, 15273, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44791, 4, 36502, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44792, 1, 1481, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44792, 2, 11121, 9661, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44792, 3, 15273, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44792, 4, 36502, 0, 0, 0);
REPLACE INTO `creature_equip_template` VALUES (44632, 1, 0, 0, 34263, 18019);

REPLACE INTO `creature_template` VALUES (44608, 0, 0, 0, 0, 0, 33996, 0, 0, 0, 'Agatha', '', '', '', 0, 15, 15, 0, 0, 68, 2, 0.6, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 2, 33536, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 2.4, 30, 1, 20, 1, 1, 1, 1, 0, 868, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44609, 0, 0, 0, 0, 0, 33997, 0, 0, 0, 'Daschla', '', '', '', 0, 15, 15, 0, 0, 68, 0, 0.6, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 2, 33536, 2099200, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 2.8, 30, 1, 20, 1, 1, 1, 1, 0, 868, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44610, 0, 0, 0, 0, 0, 33997, 0, 0, 0, 'Arthura', '', '', '', 0, 15, 15, 3, 0, 68, 2, 0.6, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 2, 33536, 2107392, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1109, 0, 0, '', 0, 2.4, 30, 1, 20, 1, 1, 1, 1, 0, 868, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44793, 0, 0, 0, 0, 0, 34078, 34079, 34080, 34081, 'Worgen Renegade', '', 'Gilneas Liberation Front', '', 0, 10, 11, 0, 0, 2207, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 44793, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_worgen_renegade_44793', 15595);
REPLACE INTO `creature_template` VALUES (44791, 0, 0, 0, 0, 0, 34050, 34051, 34052, 34053, 'Forsaken Trooper', '', '', '', 0, 10, 11, 0, 0, 2213, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19983, 11977, 8078, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_forsaken_trooper_44791', 15595);
REPLACE INTO `creature_template` VALUES (44792, 0, 0, 0, 0, 0, 34054, 34055, 34056, 34057, 'Forsaken Trooper', '', '', '', 0, 10, 11, 0, 0, 2213, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19983, 11977, 8078, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_forsaken_trooper_44791', 15595);
REPLACE INTO `creature_template` VALUES (44615, 0, 0, 0, 0, 0, 33999, 0, 0, 0, 'Grand Executor Mortuus', '', '', '', 0, 85, 85, 3, 0, 68, 2, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 2, 32768, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 5, 1, 15, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_grand_executor_mortuus_44615', 15595);
REPLACE INTO `creature_template` VALUES (44592, 0, 0, 0, 0, 0, 33978, 33979, 33980, 33981, 'Fallen Human', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2049, 0, 0, 0, 0, 0, 7, 16778240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_fallen_human_44592', 15595);
REPLACE INTO `creature_template` VALUES (44593, 0, 0, 0, 0, 0, 33982, 33983, 33984, 33985, 'Fallen Human', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2049, 32, 0, 0, 0, 0, 7, 16778240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_fallen_human_44592', 15595);
REPLACE INTO `creature_template` VALUES (44825, 0, 0, 0, 0, 0, 30840, 0, 0, 0, 'Bat Handler Maggotbreath', '', 'Flight Master', 'Taxi', 6944, 85, 85, 3, 0, 68, 8195, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_bat_handler_maggotbreath_44825', 15595);
REPLACE INTO `creature_template` VALUES (44821, 0, 0, 0, 0, 0, 1566, 0, 0, 0, 'Forsaken Bat', '', '', '', 0, 12, 12, 3, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83573, 0, 0, 0, 0, 0, 0, 0, 0, 1051, 0, 0, '', 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 212, 1, 0, 0, 8192, 'npc_forsaken_bat_44821', 15595);
REPLACE INTO `creature_template` VALUES (44882, 0, 0, 0, 0, 0, 3540, 0, 0, 0, 'Deathstalker Rane Yorick', '', '', '', 0, 15, 15, 0, 0, 68, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 4, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_deathstalker_rane_yorick_44882', 15595);
REPLACE INTO `creature_template` VALUES (44893, 0, 0, 0, 0, 0, 34164, 0, 0, 0, 'Armoire', '', '', '', 0, 1, 1, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 33554440, 2048, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1055, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 212, 1, 0, 0, 8192, 'npc_armoire_44893', 15595);
REPLACE INTO `creature_template` VALUES (44894, 0, 0, 0, 0, 0, 34165, 0, 0, 0, 'Armoire', '', '', 'Inspect', 0, 1, 1, 0, 0, 35, 16777216, 1.6, 2.57143, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 212, 1, 0, 0, 8192, 'npc_armoire_44894', 15595);
REPLACE INTO `creature_template` VALUES (44884, 0, 0, 0, 0, 0, 34162, 0, 0, 0, 'Packleader Ivar Bloodfang', '', '', '', 0, 20, 20, 0, 0, 2173, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 2, 16384, 0, 0, 0, 0, 0, 7, 262148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1059, 0, 0, '', 0, 1, 20, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_packleader_ivar_bloodfang_44884', 15595);
REPLACE INTO `creature_template` VALUES (44883, 0, 0, 0, 0, 0, 34161, 0, 0, 0, 'Lord Darius Crowley', '', 'Leader of the Gilneas Liberation Front', '', 0, 20, 20, 0, 0, 2173, 0, 1, 1.14286, 1, 1, 0, 2000, 2000, 1, 1, 1, 2, 16384, 0, 0, 0, 0, 0, 7, 262148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 20, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_lord_darius_crowley_44883', 15595);
REPLACE INTO `creature_template` VALUES (44825, 0, 0, 0, 0, 0, 30840, 0, 0, 0, 'Bat Handler Maggotbreath', '', 'Flight Master', 'Taxi', 6944, 85, 85, 3, 0, 68, 8195, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 'npc_bat_handler_maggotbreath_44825', 15595);
REPLACE INTO `creature_template` VALUES (44632, 0, 0, 0, 0, 0, 30072, 30071, 30073, 0, 'Dark Ranger', '', '', '', 0, 25, 25, 0, 0, 71, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44764, 0, 0, 0, 0, 0, 31290, 0, 0, 0, 'Horde Coffin Hauler', '', '', 'vehichleCursor', 0, 20, 20, 0, 0, 1735, 16777216, 1.2, 2, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2048, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1049, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 182, 1, 0, 0, 8192, 'npc_horde_coffin_hauler', 15595);
REPLACE INTO `creature_template` VALUES (44731, 0, 0, 0, 0, 0, 31290, 0, 0, 0, 'Horde Hauler', '', '', 'vehichleCursor', 0, 20, 20, 0, 0, 1735, 16777216, 1.2, 2, 1, 0, 0, 2000, 2000, 1, 1, 1, 768, 2048, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1048, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 182, 1, 0, 0, 8192, 'npc_horde_hauler', 15595);
REPLACE INTO `creature_template` VALUES (44734, 0, 0, 0, 0, 0, 32658, 32656, 0, 0, 'Horde Engineer', '', '', '', 0, 15, 15, 0, 0, 1735, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 33536, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44596, 0, 0, 0, 0, 0, 33986, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44597, 0, 0, 0, 0, 0, 33991, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44601, 0, 0, 0, 0, 0, 33988, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44602, 0, 0, 0, 0, 0, 33987, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44603, 0, 0, 0, 0, 0, 33989, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44604, 0, 0, 0, 0, 0, 33992, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44605, 0, 0, 0, 0, 0, 33993, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);
REPLACE INTO `creature_template` VALUES (44606, 0, 0, 0, 0, 0, 33994, 0, 0, 0, 'Forsaken Trooper', '', '', '', 0, 13, 13, 0, 0, 68, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, '', 15595);

REPLACE INTO `creature_template_addon` VALUES (44608, 0, 0, 0, 50331648, 1, 3, '');
REPLACE INTO `creature_template_addon` VALUES (44609, 0, 0, 0, 50331648, 1, 3, '');
REPLACE INTO `creature_template_addon` VALUES (44610, 0, 0, 0, 50331648, 1, 3, '');
REPLACE INTO `creature_template_addon` VALUES (44615, 0, 0, 0, 0, 257, 3, '83231');
REPLACE INTO `creature_template_addon` VALUES (44592, 0, 0, 0, 0, 1, 0, '80636');
REPLACE INTO `creature_template_addon` VALUES (44593, 0, 0, 0, 0, 1, 0, '80636');
REPLACE INTO `creature_template_addon` VALUES (44825, 0, 0, 0, 0, 257, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44821, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44882, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44893, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44894, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44884, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44883, 0, 0, 0, 0, 1, 0, '');
REPLACE INTO `creature_template_addon` VALUES (44632, 0, 0, 0, 0, 2, 3, '');
REPLACE INTO `creature_template_addon` VALUES (44764, 0, 0, 0, 0, 1, 4, '');
REPLACE INTO `creature_template_addon` VALUES (44731, 0, 0, 0, 0, 1, 4, '');

REPLACE INTO `creature_template_movement` VALUES (44610, 1, 0, 1, 0, 0, NULL);
REPLACE INTO `creature_template_movement` VALUES (44608, 1, 0, 1, 0, 0, NULL);
REPLACE INTO `creature_template_movement` VALUES (44609, 1, 0, 1, 0, 0, NULL);

INSERT INTO `creature_text` VALUES (44365, 0, 0, 'Where is that ogre-headed buffoon?', 12, 0, 100, 6, 0, 20459, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent01');
INSERT INTO `creature_text` VALUES (44365, 1, 0, 'Ah, speak of the devil...', 12, 0, 100, 1, 0, 20460, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent02');
INSERT INTO `creature_text` VALUES (44365, 2, 0, 'Warchief, so glad you could make it.', 12, 0, 100, 1, 0, 20461, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent03');
INSERT INTO `creature_text` VALUES (44365, 3, 0, 'With the death of the Lich King, many of the more intelligent Scourge became... unemployed. Those \'fiends,\' as you so delicately put it, are called val\'kyr. They are under my command now...', 12, 0, 100, 1, 0, 20462, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent04');
INSERT INTO `creature_text` VALUES (44365, 4, 0, '...and they are part of the reason that I asked to see you.', 12, 0, 100, 1, 0, 20463, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent05');
INSERT INTO `creature_text` VALUES (44365, 5, 0, 'Very well, Warchief. I have solved the plight of the Forsaken!', 12, 0, 100, 5, 0, 20464, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent06');
INSERT INTO `creature_text` VALUES (44365, 6, 0, 'As a race, we Forsaken are unable to procreate.', 12, 0, 100, 274, 0, 20465, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent07');
INSERT INTO `creature_text` VALUES (44365, 7, 0, 'With the aid of the val\'kyr, we are now able to take the corpses of the fallen and create new Forsaken.', 12, 0, 100, 1, 0, 20466, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent08');
INSERT INTO `creature_text` VALUES (44365, 8, 0, 'Agatha, show the Warchief!', 12, 0, 100, 22, 0, 20467, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent09');
INSERT INTO `creature_text` VALUES (44365, 9, 0, 'Warchief, without these new Forsaken my people would die out... Our hold upon Gilneas and northern Lordaeron would crumble.', 12, 0, 100, 1, 0, 20468, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent10');
INSERT INTO `creature_text` VALUES (44365, 10, 0, 'Isn\'t it obvious, Warchief? I serve the Horde.', 12, 0, 100, 66, 0, 20469, 0, 0, 0, 'VO_QE_SP_Sylvanas_SPEvent11');
INSERT INTO `creature_text` VALUES (44615, 0, 0, 'Move, Forsaken!', 12, 0, 100, 25, 2, 0, 0, 0, 0, '');
INSERT INTO `creature_text` VALUES (44640, 0, 0, 'ABBERATION!', 12, 0, 100, 5, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01');
INSERT INTO `creature_text` VALUES (44640, 1, 0, 'As you command, Warchief.', 12, 0, 100, 66, 0, 0, 0, 0, 0, 'VO_QE_SP_Crommush_SPEvent01');
INSERT INTO `creature_text` VALUES (44629, 0, 0, 'This better be important, Sylvanas. You know how I detest this place and its foul stench. Why have you called for me?', 12, 0, 100, 1, 0, 20496, 0, 0, 0, 'VO_QE_Garrosh_SPEvent01');
INSERT INTO `creature_text` VALUES (44629, 1, 0, 'And more importantly, what are those Scourge fiends doing here?', 12, 0, 100, 25, 0, 20497, 0, 0, 0, 'VO_QE_Garrosh_SPEvent02');
INSERT INTO `creature_text` VALUES (44629, 2, 0, 'Get on with it, Sylvanas.', 12, 0, 100, 1, 0, 20498, 0, 0, 0, 'VO_QE_Garrosh_SPEvent03');
INSERT INTO `creature_text` VALUES (44629, 3, 0, 'What you have done here, Sylvanas... it goes against the laws of nature. Disgusting is the only word I have to describe it.', 12, 0, 100, 1, 0, 20499, 0, 0, 0, 'VO_QE_Garrosh_SPEvent04');
INSERT INTO `creature_text` VALUES (44629, 4, 0, 'Have you given any thought to what this means, Sylvanas?', 12, 0, 100, 6, 0, 20500, 0, 0, 0, 'VO_QE_Garrosh_SPEvent05');
INSERT INTO `creature_text` VALUES (44629, 5, 0, 'What difference is there between you and the Lich King now?', 12, 0, 100, 6, 0, 20501, 0, 0, 0, 'VO_QE_Garrosh_SPEvent06');
INSERT INTO `creature_text` VALUES (44629, 6, 0, 'Watch your clever mouth, bitch.', 12, 0, 100, 397, 0, 20502, 0, 0, 0, 'VO_QE_Garrosh_SPEvent07');
INSERT INTO `creature_text` VALUES (44629, 7, 0, 'Cromush, you stay behind and make sure the Banshee Queen is well "guarded." I will be expecting a full report when next we meet.', 12, 0, 100, 1, 0, 20503, 0, 0, 0, 'VO_QE_Garrosh_SPEvent08');
INSERT INTO `creature_text` VALUES (44629, 8, 0, 'Remember, Sylvanas, eventually we all have to stand before our maker and face judgment. Your day may come sooner than others...', 12, 0, 100, 1, 0, 20504, 0, 0, 0, 'VO_QE_Garrosh_SPEvent09');
INSERT INTO `creature_text` VALUES (44791, 0, 0, 'Thank you, hero!', 12, 1, 100, 71, 44839, '');
INSERT INTO `creature_text` VALUES (44791, 0, 1, 'Thanks. That beast was a handful!', 12, 1, 100, 1, 44835, '');
INSERT INTO `creature_text` VALUES (44791, 0, 2, 'That beast came from nowhere!', 12, 1, 100, 5, 44836, '');
INSERT INTO `creature_text` VALUES (44791, 0, 3, 'They won\'t take our land without a fight!', 12, 1, 100, 15, 44838, '');
INSERT INTO `creature_text` VALUES (44791, 0, 4, 'Worgen filth! I spit on your corpse!', 12, 1, 100, 14, 44837, '');
INSERT INTO `creature_text` VALUES (44821, 0, 0, 'Eliminate all murlocs by using the Blight Concoction!$B$B|r|TInterface\\\\Icons\\\\INV_ALCHEMY_POTION_05.BLP:24|t', 42, 1, 100, 0, 44902, 'Forsaken Bat to Player');
INSERT INTO `creature_text` VALUES (44821, 1, 0, 'Returning to Forsaken High Command!$B$B|r|TInterface\\\\Icons\\\\ACHIEVEMENT_ZONE_SILVERPINE_01.BLP:24|t', 42, 1, 100, 0, 44903, 'Forsaken Bat to Player');
INSERT INTO `creature_text` VALUES (44882, 0, 0, 'We haven\'t got much time. Crowly will be arriving shortly.', 12, 0, 100, 5, 44951, '');
INSERT INTO `creature_text` VALUES (44882, 1, 0, 'THERE! Hide in the armoire! I\'ll hide in the shadow next to you.', 12, 0, 100, 25, 44955, '');
INSERT INTO `creature_text` VALUES (44882, 2, 0, 'I live... and die... for the Banshee Queen.', 12, 0, 100, 0, 44971, '');
INSERT INTO `creature_text` VALUES (44825, 0, 0, 'You better bring that bat back in one piece, $n!', 12, 0, 100, 25, 44906, '');
INSERT INTO `creature_text` VALUES (44883, 0, 0, 'Have you given any more thought to my proposal, Ivar?', 12, 0, 100, 6, 44957, '');
INSERT INTO `creature_text` VALUES (44883, 1, 0, 'If we\'re to win this war we will need your help. Our packs must unite! The Forsaken will destroy us otherwise.', 12, 0, 100, 396, 44960, '');
INSERT INTO `creature_text` VALUES (44883, 2, 0, 'You are the alpha male, Ivar. The rest of the ferals in Silverpine will do as you command.', 12, 0, 100, 397, 44961, '');
INSERT INTO `creature_text` VALUES (44883, 3, 0, 'So will you help?', 12, 0, 100, 6, 44965, '');
INSERT INTO `creature_text` VALUES (44884, 0, 0, 'Why trust you now, Crowley? You abandoned us. Left us to die.', 12, 0, 100, 6, 44959, '');
INSERT INTO `creature_text` VALUES (44884, 1, 0, 'I don\'t care about your war, Crowley.', 12, 0, 100, 274, 44962, '');
INSERT INTO `creature_text` VALUES (44884, 2, 0, 'But...', 12, 0, 100, 1, 0, '');
INSERT INTO `creature_text` VALUES (44884, 3, 0, 'I have seen firsthand what the Forsaken are capable of doing. It is true. They hunt us... slaughter the defenseless.', 12, 0, 100, 1, 44964, '');
INSERT INTO `creature_text` VALUES (44884, 4, 0, 'Aye, I will gather my pack... we...', 12, 0, 100, 1, 44966, '');
INSERT INTO `creature_text` VALUES (44884, 5, 0, 'Packleader Ivar Bloodfang sniffs the air.', 16, 0, 100, 479, 44967, '');
INSERT INTO `creature_text` VALUES (44884, 6, 0, 'It would appear that we are being watched, Crowley. You have grown soft... Likely Greymane\'s fault.', 12, 0, 100, 1, 44968, '');
INSERT INTO `creature_text` VALUES (44884, 7, 0, 'Treacherous little pup!', 12, 0, 100, 15, 44969, '');
INSERT INTO `creature_text` VALUES (44884, 8, 0, 'What say you now, spy?', 12, 0, 100, 0, 44970, '');
INSERT INTO `creature_text` VALUES (44884, 9, 0, 'I was hoping you\'d say that...', 12, 0, 100, 0, 44972, '');
INSERT INTO `creature_text` VALUES (44884, 10, 0, 'I will prepare the pack. It will take some time to gather them all, but we will join... for now.', 12, 0, 100, 1, 44973, '');

REPLACE INTO `creature_sparring_template` VALUES (44793, 85);
REPLACE INTO `creature_sparring_template` VALUES (44791, 85);
REPLACE INTO `creature_sparring_template` VALUES (44792, 85);

REPLACE INTO `gameobject_template` VALUES (205143, 2, 3332, 'Abandoned Outhouse', '', '', '', 1.03418, 0, 15590, 0, 11897, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 'go_abandoned_outhouse_205143', 15595);

REPLACE INTO `gossip_menu` VALUES (6944, 7778, 15595);

REPLACE INTO `gossip_menu_option` VALUES (6944, 0, 2, 'Show me where I can fly.', 12271, 4, 8192, 15595);
REPLACE INTO `gossip_menu_option` VALUES (6944, 1, 2, 'I need to take a bat to the Dawning Isles.', 0, 1, 1, 15595);

REPLACE INTO `npc_spellclick_spells` VALUES (44894, 83756, 1, 0);

REPLACE INTO `creature_questender` VALUES (44615, 26964);
REPLACE INTO `creature_questender` VALUES (44615, 26965);
REPLACE INTO `creature_questender` VALUES (44778, 26992);
REPLACE INTO `creature_questender` VALUES (44784, 26995);
REPLACE INTO `creature_questender` VALUES (44615, 26989);
REPLACE INTO `creature_questender` VALUES (44784, 26998);
REPLACE INTO `creature_questender` VALUES (44789, 27045);
REPLACE INTO `creature_questender` VALUES (44365, 27056);
REPLACE INTO `creature_questender` VALUES (44916, 27065);

REPLACE INTO `creature_queststarter` VALUES (1515, 26964);
REPLACE INTO `creature_queststarter` VALUES (44615, 26965);
REPLACE INTO `creature_queststarter` VALUES (44778, 26992);
REPLACE INTO `creature_queststarter` VALUES (44784, 26995);
REPLACE INTO `creature_queststarter` VALUES (44615, 26989);
REPLACE INTO `creature_queststarter` VALUES (44784, 26998);
REPLACE INTO `creature_queststarter` VALUES (44789, 27056);
REPLACE INTO `creature_queststarter` VALUES (44365, 27065);

REPLACE INTO `quest_template` VALUES (26964, 2, 10, 9, 130, 0, 0, 0, 0, 0, 0, 26965, 5, 0, 510, 0, 0, 0, 0, 0, 262152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Warchief\'s Command: Silverpine Forest!', 'Report to Grand Executor Mortuus at Forsaken High Command in Silverpine Forest.', 'All able-bodied members of the Horde are hereby ordered to report to Grand Executor Mortuus at Forsaken High Command in Silverpine Forest.$B$BNow, more than ever, your help is needed in routing the enemy infestation of the region and conquering the port of Gilneas!$B$BTo reach Forsaken High Command you must travel south through Tirisfal Glades and enter Silverpine Forest. From there, follow the road south until you reach Forsaken High Command.$B$BOnward, $g brother:sister;! Destiny calls!', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (26965, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 0, 4, 0, 780, 0, 0, 0, 0, 0, 4194312, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Warchief Cometh', 'Stand by for Warchief Garrosh Hellscream\'s arrival.', 'You\'re just in time, $Nnub. It is $Nnub, right?$B$B<Grand Executor Mortuus looks at a sheet of paper.>$B$BYes, that\'s what the paperwork says: "$Nnub."$B$B<Grand Executor Mortuus points to your name on the sheet of paper.>$B$BWhere was I? Oh yes, you\'re just in time! The Warchief should be arriving at any moment to meet with our leader and savior, the Dark Lady herself. Just stand by and be at your best behavior. I expect that when this demonstration is over we will all receive commendations!', '', 'Speak with Grand Executor Mortuus at Forsaken High Command in Silverpine Forest.', 44629, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Stand by for Warchief Garrosh Hellscream\'s arrival', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (26992, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 0, 5, 400, 1080, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Agony Abounds', 'Gather 8 Ferocious Doomweed.', 'You have no doubt heard about the incident that transpired at the Wrath Gate in Northrend, yes? Due to those unfortunate events we are strictly forbidden from using the strain of life-shattering blight that we finally perfected. Now we have rules and regulations and other nonsense!$B$BThey\'ve given us some neutered version of the perfected blight and expect us to weaponize it.$B$BIf I\'m to get this crap functional I\'ll need several bags full of ferocious doomweed. You\'ll find it all over Silverpine.', '', 'Return to Apothecary Witherbloom at Forsaken High Command in Silverpine Forest.', 0, 0, 0, 0, 0, 0, 0, 0, 60741, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (26995, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 26998, 5, 400, 1080, 0, 0, 0, 0, 0, 8388616, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Guts and Gore', 'Recover 6 "Clean" Beast Guts.', 'If we\'re to get this new blight up and running we\'ll need clean beast guts. The problem is that clean beast guts are a rare commodity in Silverpine Forest. Most of the beasts here have succumbed to disease and thus only have diseased organs.$B$BDo a sweep of the forested area east of here for worgs and grizzled bears. Kill only the non-diseased variety and rip out their entrails. Bring me all of the clean guts that you can stuff into your backpack!', '', 'Return to High Apothecary Shana T\'Veen at Forsaken High Command in Silverpine Forest.', 0, 0, 0, 0, 0, 0, 0, 0, 60742, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (26989, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1080, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62839, 1, 62838, 1, 62837, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Gilneas Liberation Front', 'Kill 10 Worgen Renegades.', 'The battle over this region still rages, $Nnub. Although we soundly beat back the Gilnean defenses and took Gilneas for the Horde, a small group of rebels managed to escape. The "Gilneas Liberation Front" - as they call themselves - make attacks of opportunity against our forces in the field. The rebels have had a few minor victories and now grow brazen in their assaults! The time to strike back is now, $Nnub.$B$BVenture south into the wilds of Silverpine and kill any rebel that you cross.', '', 'Return to Grand Executor Mortuus at Forsaken High Command in Silverpine Forest.', 44793, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (26998, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 0, 6, 0, 1320, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62836, 1, 62835, 1, 62834, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Iterating Upon Success', 'Kill 50 Vile Fin Murlocs.', 'The concoction is finally ready, but before I can put my seal of approval on this strain, we\'ll need to run a field test.$B$BSpeak to Bat Handler Maggotbreath, here at High Command, and requisition a bat. The bat will take you to the Dawning Isles, east of here across Lordamere Lake, right into the heart of depravity itself - the Vile Fin murloc dens!$B$BUse the concoction against the murlocs and report the results back to me.$B$BThose results being that you killed them all, as commanded!', '', 'Return to High Apothecary Shana T\'Veen at Forsaken High Command in Silverpine Forest.', 44825, 1908, 0, 0, 1, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Bat requisitioned from Bat Handler Maggotbreath', 'Vile Fin Murloc slain', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (27039, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 27045, 4, 0, 780, 0, 0, 0, 0, 0, 262152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1136, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Dangerous Intentions', 'Find Deathstalker Yorick at the Ivar Patch in Silverpine Forest. ', 'Do you know why Mortuus appends \'nub\' to your name, $N? Because he is an incompetent sycophant that has trouble seeing through his oversized helmet. \'NUB\' is your classification. You are now part of the NEW UNDERCITY BATALLION.$B$BBut enough with the comedy! I sent Deathstalker Yorick on a shadow operation two days ago. Her goal was to get information on the Gilneas Liberation Front and return. Three days and no word from her.$B$BHead southwest to the Ivar Patch and look for Yorick, $N.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (27045, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 27056, 7, 1300, 1560, 0, 0, 0, 0, 0, 1048576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62833, 1, 62832, 1, 62831, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Waiting to Exsanguinate', 'Enter the Ivar Cottage and find a suitable hiding place upstairs. Then eavesdrop on Lord Darius Crowley.', 'The Gilneas Liberation Front leader, Darius Crowley, has been meeting with another worgen inside this cottage for the past three days. I\'m not sure what\'s going on, but we need to get inside the cottage and find a good hiding place from where we can eavesdrop.$B$BIt\'s now or never, $r. We may never get another shot.$B$BJust promise me that if I don\'t make it, you\'ll return to Forsaken High Command and inform Belmont of whatever we discover.', '', 'Return to Deathstalker Commander Belmont at Forsaken High Command in Silverpine Forest.', 44882, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Learn of Darius Crowley\'s Plans', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (27056, 2, 11, 9, 130, 0, 0, 0, 0, 0, 0, 27065, 1, 0, 120, 0, 0, 0, 0, 60766, 8388616, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Belmont\'s Report', 'Deliver Belmont\'s Report to Lady Sylvanas Windrunner at Forsaken High Command in Silverpine Forest.', '<Belmont hands you a sealed envelope.>$B$BDeliver this report to Lady Sylvanas Windrunner. It is my assessment of the situation in Silverpine based upon your and Yorick\'s field work.$B$BIf we do not stamp out the worgen insurgents, they will overrun us. The time to act is now!', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 60766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);
REPLACE INTO `quest_template` VALUES (27065, 2, 12, 10, 130, 0, 0, 0, 0, 0, 0, 27069, 1, 0, 210, 0, 0, 0, 0, 0, 262152, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 1, 0, 76, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Warchief\'s Fleet', 'Report to Admiral Hatchet at the Forsaken Rear Guard in Silverpine Forest.', 'Our reinforcements from Kalimdor arrived last night. Three orc gunships full of supplies, weapons and soldiers. Three orc gunships lost to Bloodfang\'s damned worgen.$B$BHow could an entire orc fleet lose to a small band of enemy combatants that lack any siege weaponry? An important question that you will answer for me.$B$BGather your gear and report to the orc admiral, Hatchet, at the Forsaken Rear Guard, southwest of here.$B$BGet those orcs operational, $N.$B$BI await your full report.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 890, 878, 15595);

REPLACE INTO `quest_template_addon` VALUES (26964, 0, 0, 0, 25012, 26965, 26964, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 946, 0);
REPLACE INTO `quest_template_addon` VALUES (26965, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (26992, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (26995, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (26989, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (26998, 0, 0, 0, 26995, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (27039, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (27045, 0, 0, 0, 27039, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (27056, 0, 0, 0, 27045, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 8388607, 0);
REPLACE INTO `quest_template_addon` VALUES (27065, 0, 0, 0, 27056, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388607, 0);

REPLACE INTO `quest_offer_reward` VALUES (26964, 5, 21, 0, 0, 0, 0, 0, 0, 'Most excellent! A new recruit has arrived. There is much to do... much to do I say.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (26965, 274, 1, 273, 0, 0, 0, 0, 0, 'Well that was unexpected. I am not one to doubt our Warchief, but...$B$BNevermind! We have work to do. I\'m sure the Dark Lady and the Warchief will get this all worked out.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (26992, 5, 1, 273, 0, 0, 0, 0, 0, 'Agony abounds, friend! The land itself threatens to devour us, and yet we persist. I wonder why that is? With this doomweed and the guts and gore that T\'Veen needs, we might have something suitable to send to the Forsaken front.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (26995, 4, 1, 0, 0, 0, 0, 0, 0, 'Excellent! With these guts and the ferocious doomweed we\'ll be able to test out our new strain of blight. Now to select a suitable target...', 15595);
REPLACE INTO `quest_offer_reward` VALUES (26989, 1, 5, 21, 21, 0, 0, 0, 0, '$Nnub has returned! And in one piece!$B$BDefender of Silverpine! Redeemer of the Horde! $Nnub, your bravery must be rewarded!', 15595);
REPLACE INTO `quest_offer_reward` VALUES (26998, 5, 1, 273, 0, 0, 0, 0, 0, 'Potent enough to exterminate a thousand murlocs! I think I\'ll put that on the label. You\'ve done well, $N, and great work around here gets rewarded.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (27039, 0, 0, 0, 0, 0, 0, 0, 0, 'Belmont gets a little nervous and sends some newbie to look for me? Listen, I\'ve been staking out the old Ivar cottage from inside this outhouse for three days. Your presence threatens the whole damn operation!$B$BThere\'s no time to explain. We have to act now or this mission will be compromised.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (27045, 5, 1, 0, 0, 0, 0, 0, 0, 'Alas, poor Yorick! I knew her, $N. An assassin of infinite courage...$B$BNow, tell me what\'s going on? What did you and Yorick discover?', 15595);
REPLACE INTO `quest_offer_reward` VALUES (27056, 1, 1, 274, 5, 0, 0, 0, 0, 'Crowley... A gnat. An insect, waiting to be crushed beneath my boot heel.$B$BSo he thinks to rouse the feral worgen of Silverpine? We shall show him and his allies the might of the Forsaken!$B$BBut first we must clean up the mess made by the Warchief\'s fleet. Without their aid we may lose this war.', 15595);
REPLACE INTO `quest_offer_reward` VALUES (27065, 1, 2, 0, 0, 0, 0, 0, 0, 'This fiasco may go down as one of the biggest embarrassments in the history of naval warfare. How did this happen? Drunk happened. That\'s how.', 15595);

INSERT INTO `spell_area` VALUES (83989, 5369, 26964, 27097, 0, 0, 2, 3, 74, 11);
INSERT INTO `spell_area` VALUES (83232, 5369, 26964, 27098, 0, 0, 2, 3, 66, 1);

INSERT INTO `spell_script_names` VALUES (83173, 'spell_silverpine_raise_forsaken_83173_spell');
INSERT INTO `spell_script_names` VALUES (83149, 'spell_silverpine_forsaken_trooper_master_script_83149');

INSERT INTO `spell_target_position` VALUES (83756, 0, 0, 1314.91, 1211.39, 58.4967, 5.8, 0);

INSERT INTO `vehicle_template_accessory` VALUES (44764, 44734, 0, 1, 'Sitting on top - Horde Engineer', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44764, 44737, 1, 1, 'Subdued Forest Ettin', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44764, 0, 2, 1, 'Sitting on floor (top left) - Player side', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44764, 44766, 3, 1, 'On the bottom - Coffin', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44764, 44766, 4, 1, 'On the top - Coffin', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44764, 44766, 5, 1, 'On the middle - Coffin', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44734, 0, 1, 'Sitting on top - Horde Engineer', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44737, 1, 1, 'Subdued Forest Ettin', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 0, 2, 1, 'Sitting on floor (top left) - Player side', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44733, 3, 1, 'Sitting on floor (top right) - Female Trooper', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44732, 4, 1, 'Sitting on floor (bottom left) - Male Trooper', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44733, 5, 1, 'Sitting on floor (bottom right) - Female Trooper', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44733, 6, 1, 'Sitting on shelf (bottom left) - Female trooper', 8, 30000);
INSERT INTO `vehicle_template_accessory` VALUES (44731, 44732, 7, 1, 'Sitting on shelf (bottom right) - Male trooper', 8, 30000);

INSERT INTO `waypoint_data` VALUES (448820, 1, 1299.87, 1189.73, 52.1171, 2.45043, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 2, 1290.53, 1196.47, 52.4176, 2.49362, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 3, 1289.97, 1200.79, 52.6583, 1.70037, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 4, 1290.12, 1205.71, 52.7164, 1.53936, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 5, 1293.6, 1207.07, 53.5343, 0.373047, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 6, 1299.12, 1207.07, 53.7466, 0.325923, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 7, 1303.74, 1210.4, 53.7466, 0.93068, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 8, 1304.21, 1212.67, 53.7466, 1.36265, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 9, 1303.62, 1219.87, 53.7466, 1.65325, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 10, 1300.44, 1222.61, 53.7466, 2.43864, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 11, 1297.95, 1220.37, 53.7466, 3.94268, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 12, 1297.62, 1212.44, 58.4808, 4.06835, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 13, 1297.64, 1210.12, 58.4751, 4.72023, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 14, 1301.66, 1206.27, 58.4941, 5.53704, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 15, 1311.44, 1206.49, 58.5103, 6.2596, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448820, 16, 1312.64, 1208.44, 58.5121, 1.01707, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 5, 1313.48, 1206.09, 58.5119, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 4, 1313.48, 1206.09, 58.5119, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 3, 1305.32, 1206.43, 58.5126, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 2, 1302.33, 1206.44, 58.499, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 1, 1300.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448830, 0, 1299.36, 1206.64, 58.5706, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448840, 4, 1308.4, 1206.28, 58.5109, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448840, 3, 1308.4, 1206.28, 58.5109, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448840, 2, 1302.31, 1206.61, 58.4984, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448840, 1, 1297.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448840, 0, 1296.2, 1210.54, 58.5533, 0, 3, 0, 1, 0, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 0, 1379.67, 1021.7, 59.9887, 0, 0, 0, 1, 3, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 1, 1249.74, 1001.91, 66.9924, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 2, 1212.13, 981.977, 65.3913, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 3, 1180.25, 927.461, 60.3111, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 4, 1149.33, 766.078, 57.3167, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 5, 1192.43, 475.569, 62.4255, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448210, 6, 1204.81, 329.134, 50.3452, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 0, 1197.2, 295.636, 47.8965, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 1, 1119.13, 234.839, 46.6181, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 2, 989.85, 180.368, 52.3217, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 3, 904.214, 168.247, 53.1545, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 4, 736.523, 178.213, 52.474, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 5, 721.224, 213.371, 53.8464, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 6, 720.123, 256.447, 55.9538, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 7, 790.195, 289.27, 52.1988, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 8, 1058.38, 312.523, 51.4779, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 9, 1078.93, 313.845, 51.2958, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 10, 1174.25, 330.96, 51.4075, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 11, 1186.85, 345.036, 50.4988, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 12, 1209.86, 406.546, 54.787, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 13, 1300.05, 659.114, 70.2942, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 14, 1361.85, 853.952, 70.2904, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 15, 1408.33, 1010.9, 60.1401, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 16, 1419.27, 1020.78, 54.8617, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448211, 17, 1009.7, 353.118, 57.484, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 0, 994.671, 465.943, 80.597, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 1, 972.181, 656.085, 119.203, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 2, 986.189, 699.04, 122.876, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 3, 1097.17, 790.282, 103.229, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 4, 1338.65, 979.091, 68.574, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (448212, 5, 1418.54, 1019.48, 53.918, 0, 0, 0, 1, 1, 0, 0, 0);
INSERT INTO `waypoint_data` VALUES (446320, 1, 1347.58, 1055.34, 52.4303, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 2, 1346.27, 1031.6, 52.4005, 0, 0, 8000, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 3, 1347.58, 1055.34, 52.4303, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 4, 1359.32, 1061.86, 52.9373, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 5, 1383.98, 1085.92, 53.1041, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 6, 1394.97, 1093.72, 57.4961, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 7, 1406.86, 1093.08, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 8, 1413.5, 1086.69, 60.477, 0, 0, 8000, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 9, 1406.86, 1093.08, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 10, 1394.97, 1093.72, 57.4961, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 11, 1383.98, 1085.92, 53.1041, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446320, 12, 1370.33, 1072.82, 53.2393, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 1, 1433.18, 1070.35, 60.4769, 0, 0, 8000, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 2, 1441.05, 1065.37, 60.4769, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 3, 1442.17, 1052.31, 59.632, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 4, 1425.12, 1028.87, 52.6475, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 5, 1410.54, 1022.41, 53.0542, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 6, 1400.68, 1018.82, 53.1551, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 7, 1387, 1008.78, 52.9742, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 8, 1374.11, 1008.62, 52.4603, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 9, 1356.44, 1018, 52.273, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 10, 1370.67, 1009.98, 52.3691, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 11, 1385.57, 1008.11, 52.8996, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 12, 1395.1, 1015.81, 53.1705, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 13, 1440.66, 1048.09, 58.4715, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 14, 1442.34, 1055.47, 60.1114, 0, 0, 0, 1, 0, 0, 100, 0);
INSERT INTO `waypoint_data` VALUES (446321, 15, 1442.73, 1063.33, 60.4767, 0, 0, 0, 1, 0, 0, 100, 0);