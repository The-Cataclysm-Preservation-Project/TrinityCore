-- Correct low level class trainers.

-- Correct one of the mage trainers.
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `MenuId` = 10683;

-- add gossip_menu_option_trainer records.
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4684,4676,4692,4679,7522,4652,4655,4643);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4684, 0, 145),
(4676, 0, 17),
(4692, 0, 127),
(4679, 0, 127),
(7522, 0, 134),
(4652, 0, 134),
(4655, 0, 32),
(4643, 0, 32);
	
-- change three trainers to low level trainers.
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 127 WHERE `MenuId` = 7349;
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 127 WHERE `MenuId` = 13921;
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 127 WHERE `MenuId` = 10700;

-- give Drill Sergeant Steamcrank a low level gossip
UPDATE `creature_template` SET `gossip_menu_id` = 4645 WHERE `entry` = 42324;
-- give Nekali a low level gossip
UPDATE `creature_template` SET `gossip_menu_id` = 10684 WHERE `entry` = 38242;
-- mage wotlk gossip
UPDATE `creature_template` SET `gossip_menu_id` = 6648 WHERE `entry` = 15279;


-- Correct remaining class trainers

-- Correct flags on numerous trainer gossip_options.
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `OptionIcon` = 3 AND `MenuId` IN (141, 11912, 11913, 12050, 4105, 11932, 12606, 10552, 10817, 11766, 11875, 12344, 12151, 11879, 12048, 12341, 10838, 12010, 12537, 12714, 10684, 12052, 4555, 4553, 10837, 12918, 12716, 12521, 12821, 12522, 12746, 12927, 12755, 12920);

-- add `gossip_menu_option_trainer` records.
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4683,4526,4525,4527,7263,4662,12516,12527,4694,4674,4621,4092,410,7265,4680,4691,4574,4573,4572,4559,4558,3643,4801,3642,4105,7467,4516,4515,5123,12052,64,6648,4534,4536,4535,4539,4538,4537,4553,12233,4641,11932,4656,4603,4604,4609,4610,4504,1522,12821,50723,50729,4605,4646,4687,12755);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4683, 0, 16),
(4526, 0, 16),
(4525, 0, 16),
(4527, 0, 16),
(7263, 0, 16),
(4662, 0, 164),
(12516, 0, 164),
(12527, 0, 164),
(4694, 0, 40),
(4674, 0, 40),
(4621, 0, 40),
(4092, 0, 40),
(410, 0, 33),
(7265, 0, 3),
(4680, 0, 3),
(4691, 0, 3),
(4574, 0, 3),
(4573, 0, 3),
(4572, 0, 3),
(4559, 0, 3),
(4558, 0, 3),
(3643, 0, 3),
(4801, 0, 3),
(3642, 0, 3),
(4105, 0, 124),
(7467, 0, 124),
(4516, 0, 124),
(4515, 0, 124),
(5123, 0, 124),
(12052, 0, 124),
(64, 0, 135),
(6648, 0, 135),
(4534, 0, 135),
(4536, 0, 135),
(4535, 0, 135),
(4539, 0, 135),
(4538, 0, 135),
(4537, 0, 135),
(4553, 0, 135),
(12233, 0, 135),
(4641, 0, 154),
(11932, 0, 154),
(4656, 0, 154),
(4603, 0, 154),
(4604, 0, 154),
(4609, 0, 154),
(4610, 0, 154),
(4504, 0, 154),
(1522, 0, 154),
(12821, 0, 154),
(50723, 3, 154),
(50729, 3, 154),
(4605, 0, 39),
(4646, 0, 39),
(4687, 0, 39),
(12755, 0, 39);

-- add missing gossip_menu_option_trainer for Portal Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4824,4827);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4824, 0, 130),
(4827, 0, 149);

-- change low level `gossip_menu_option_trainer` to standard class trainer.
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 16 WHERE `MenuId` = 523;
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 164 WHERE `MenuId` IN (6647,11767);
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 40 WHERE `MenuId` = 4647;
UPDATE `gossip_menu_option_trainer` SET `TrainerId` = 33 WHERE `MenuId` = 6650;

-- change trainer type
UPDATE `creature_template` SET `trainer_class` = 4 WHERE `entry` = 16279;

-- correct creature_template npcflag for class trainer
UPDATE `creature_template` SET `npcflag` = 49 WHERE `entry` = 50716;
	
-- switch numerous trainers to correct gossip menus.
-- gnome mage trainers
UPDATE `creature_template` SET `gossip_menu_id` = 4554 WHERE `entry` = 1228;
UPDATE `creature_template` SET `gossip_menu_id` = 4554 WHERE `entry` = 5144;
UPDATE `creature_template` SET `gossip_menu_id` = 4554 WHERE `entry` = 5145;

-- Undead rogue trainer
UPDATE `creature_template` SET `gossip_menu_id` = 4540 WHERE `entry` = 2130;

-- update Gilneans to standard class trainers.
UPDATE `creature_template` SET `gossip_menu_id` = 10834 WHERE `entry` = 35873;

-- switch trainers who do nothing to either wotlk or generic gossips
-- warrior wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 523 WHERE `entry` = 3169;
UPDATE `creature_template` SET `gossip_menu_id` = 4548 WHERE `entry` = 4593;
UPDATE `creature_template` SET `gossip_menu_id` = 4546 WHERE `entry` = 4594;
UPDATE `creature_template` SET `gossip_menu_id` = 4547 WHERE `entry` = 4595;
-- warrior generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 14199 WHERE `entry` = 17480;
UPDATE `creature_template` SET `gossip_menu_id` = 4548 WHERE `entry` = 49955;
-- paladin generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 6647 WHERE `entry` = 49954;
-- hunter generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4647 WHERE `entry` = 39116;
UPDATE `creature_template` SET `gossip_menu_id` = 4647 WHERE `entry` = 49946;
UPDATE `creature_template` SET `gossip_menu_id` = 4647 WHERE `entry` = 39100;
UPDATE `creature_template` SET `gossip_menu_id` = 4647 WHERE `entry` = 50609;
-- rogue wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4542 WHERE `entry` = 4582;
UPDATE `creature_template` SET `gossip_menu_id` = 4540 WHERE `entry` = 4583;
UPDATE `creature_template` SET `gossip_menu_id` = 4541 WHERE `entry` = 4584;
UPDATE `creature_template` SET `gossip_menu_id` = 4562 WHERE `entry` = 5166;
UPDATE `creature_template` SET `gossip_menu_id` = 85 WHERE `entry` = 2130;
-- rogue generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 436 WHERE `entry` = 49909;
UPDATE `creature_template` SET `gossip_menu_id` = 436 WHERE `entry` = 49949;
UPDATE `creature_template` SET `gossip_menu_id` = 436 WHERE `entry` = 51998;
UPDATE `creature_template` SET `gossip_menu_id` = 436 WHERE `entry` = 48615;
-- priest wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4544 WHERE `entry` = 4606;
UPDATE `creature_template` SET `gossip_menu_id` = 4545 WHERE `entry` = 4607;
-- Sister Almyra gossips
UPDATE `creature_template` SET `gossip_menu_id` = 10838 WHERE `entry` = 38466;
-- priest generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 4608;
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 49901;
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 49950;
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 48614;
-- mage wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4537 WHERE `entry` = 4568;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 5497;
UPDATE `creature_template` SET `gossip_menu_id` = 63 WHERE `entry` = 	23103;
-- mage generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 4567;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 49952;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 50714;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 50715;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 50717;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 50720;
-- warlock wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 2383 WHERE `entry` = 4563;
UPDATE `creature_template` SET `gossip_menu_id` = 4609 WHERE `entry` = 4564;
-- warlock generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 49945;
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 50723;
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 50729;
-- tauren druid generic gossip
UPDATE `creature_template` SET `gossip_menu_id` = 4605 WHERE `entry` = 44726;


-- Correct Profession Trainers

-- correct trainer flags in gossip menu options
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `OptionIcon` = 3 AND `MenuId` IN (4361, 4134, 4129, 4164, 7455, 7817, 9084, 12846, 8460, 6087, 10437, 2021, 51997);

-- add missing gossip_menu_option_trainer for Apprentice Blacksmiths
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (2746) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(2746, 0, 80);

-- add missing gossip_menu_option_trainer for Apprentice Engineers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4142,7383) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4142, 0, 102),
(7383, 0, 102);

-- add missing gossip_menu_option_trainer for Apprentice Leatherworkers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (7430) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(7430, 0, 103);

-- add missing gossip_menu_option_trainer for Herbalists.
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (7524,7459,10350) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(7524, 1, 101),
(7459, 0, 101),
(10350, 0, 101);

-- add missing gossip_menu_option_traine` for Miners
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4361,10351) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4361, 0, 49),
(10351, 0, 49);

-- add missing gossip_menu_option_trainer for Skinners
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (7429,10360) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(7429, 0, 83),
(10360, 0, 83);
	
-- add missing gossip_menu_option_trainer for Alchemists
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (7384,4115,4127,4129,2391,2837,16161,8540,8733,10627) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(7384, 0, 122),
(4115, 0, 122),
(4127, 0, 122),
(4129, 0, 122),
(2391, 0, 122),
(2837, 0, 122),
(16161, 0, 122),
(8540, 0, 122),
(8733, 0, 122),
(10627, 0, 122);

-- add missing gossip_menu_option_trainer for Blacksmiths
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (2781,2783,593,597,1022,7256,9459) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(2781, 0, 58),
(2783, 0, 58),
(593, 0, 58),
(597, 0, 58),
(1022, 0, 58),
(7256, 0, 58),
(9459, 0, 58);

-- add missing gossip_menu_option_trainer for Enchanters
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4166,8731,8866,10365,33676) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4166, 0, 62),
(8731, 0, 62),
(8866, 0, 62),
(10365, 0, 62),
(33676, 0, 62);

-- add missing gossip_menu_option_trainer for Engineers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4145,4149,8656,9084) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4145, 0, 407),
(4149, 0, 407),
(8656, 0, 407),
(9084, 0, 407);

-- add missing gossip_menu_option_trainer for Scribes
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (9879) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(9879, 0, 63);

-- add missing gossip_menu_option_trainer for Jewelcrafters
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (12848,8376,8380,8382,9892,9894,9895,9873) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(12848, 0, 29),
(8376, 0, 29),
(8380, 0, 29),
(8382, 0, 29),
(9892, 0, 29),
(9894, 0, 29),
(9895, 0, 29),
(9873, 0, 29);

-- add missing gossip_menu_option_trainer for Leatherworkers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4108,4172,4210,4243,7866,10361,4843,8732,7816,12852) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4108, 0, 56),
(4172, 0, 56),
(4210, 0, 56),
(4243, 0, 56),
(7866, 0, 56),
(10361, 0, 56),
(4843, 0, 56),
(8732, 0, 56),
(7816, 0, 56),
(12852, 0, 56);

-- add missing gossip_menu_option_trainer for Tailors
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4261,4348,4270,4267,4354,4844,7524,8868,10114,10116,10117,10364) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4261, 0, 163),
(4348, 0, 163),
(4270, 0, 163),
(4267, 0, 163),
(4354, 0, 163),
(4844, 0, 163),
(7524, 0, 163),
(8868, 0, 163),
(10114, 0, 163),
(10116, 0, 163),
(10117, 0, 163),
(10364, 0, 163);

-- add missing gossip_menu_option_trainer for First Aid Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4761,12939,8502) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4761, 0, 107),
(12939, 0, 107),
(8502, 0, 107);

-- add missing gossip_menu_option_trainer for Cooking Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (9986,9985,9987) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(9986, 0, 136),
(9985, 0, 136),
(9987, 0, 136);

-- add missing gossip_menu_option_trainer for Fishing Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (12887,6087,8826) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(12887, 0, 10),
(6087, 0, 10),
(8826, 0, 10);

-- add missing gossip_menu_option_trainer for Archaeology Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (12850) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(12850, 0, 373);

-- add missing gossip_menu_option_trainer for Riding Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (8553,8275,4019) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(8553, 0, 46),
(8275, 0, 46),
(4019, 0, 46);

-- correct trainer npcFlags
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 30711;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 4578;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 7087;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 16742;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 30716;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 19775;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 26956;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 33587;
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 34785;

-- give trainers who do nothing generic gossips
-- herbalism generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 908;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 1473;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 2114;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 2390;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 2856;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 12025;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 26910;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 26958;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 26974;
UPDATE `creature_template` SET `gossip_menu_Id` = 7691 WHERE `entry` = 26994;
-- mining wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 4598;
-- mining generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 3175;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 3555;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 8128;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 26912;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 26962;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 26976;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 28698;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 52170;
UPDATE `creature_template` SET `gossip_menu_Id` = 7690 WHERE `entry` = 53409;
-- skinning generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 6295;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 6306;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 7087;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 8144;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 12030;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 27000;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 26913;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 26986;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 28696;
UPDATE `creature_template` SET `gossip_menu_Id` = 7842 WHERE `entry` = 53437;
-- alchemy generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 26987;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 26903;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 26951;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 26975;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 27023;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 27029;
UPDATE `creature_template` SET `gossip_menu_Id` = 10359 WHERE `entry` = 33588;
-- blacksmithing wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 1022 WHERE `entry` = 4596;
UPDATE `creature_template` SET `gossip_menu_Id` = 1022 WHERE `entry` = 26952;
-- blacksmithing generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 16724;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 26988;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 26904;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 26924;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 27034;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 28694;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 29505;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 29506;
UPDATE `creature_template` SET `gossip_menu_Id` = 8521 WHERE `entry` = 33591;
-- enchanting generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 19251;
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 26990;
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 26906;
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 26954;
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 26980;
UPDATE `creature_template` SET `gossip_menu_Id` = 8866 WHERE `entry` = 33583;
-- engineering wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 4149 WHERE `entry` = 11031;
-- engineering generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 8867 WHERE `entry` = 10993;
UPDATE `creature_template` SET `gossip_menu_Id` = 8867 WHERE `entry` = 26991;
UPDATE `creature_template` SET `gossip_menu_Id` = 8867 WHERE `entry` = 25277;
UPDATE `creature_template` SET `gossip_menu_Id` = 8867 WHERE `entry` = 26907;
UPDATE `creature_template` SET `gossip_menu_Id` = 8867 WHERE `entry` = 33586;
-- inscription generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 9879 WHERE `entry` = 30721;
UPDATE `creature_template` SET `gossip_menu_Id` = 9879 WHERE `entry` = 30710;
UPDATE `creature_template` SET `gossip_menu_Id` = 9879 WHERE `entry` = 30711;
UPDATE `creature_template` SET `gossip_menu_Id` = 9879 WHERE `entry` = 30715;
UPDATE `creature_template` SET `gossip_menu_Id` = 9879 WHERE `entry` = 30716;
-- jewelcrafting generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 11861 WHERE `entry` = 33590;
UPDATE `creature_template` SET `gossip_menu_Id` = 11861 WHERE `entry` = 52586;
UPDATE `creature_template` SET `gossip_menu_Id` = 11861 WHERE `entry` = 52587;
-- leatherworking wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 4210 WHERE `entry` = 4588;
UPDATE `creature_template` SET `gossip_menu_Id` = 4242 WHERE `entry` = 16278;
-- leatherworking generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 7869;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 16728;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 26998;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 26911;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 26996;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 29508;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 29509;
UPDATE `creature_template` SET `gossip_menu_Id` = 10361 WHERE `entry` = 33581;
-- tailoring wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 4267 WHERE `entry` = 3523;
UPDATE `creature_template` SET `gossip_menu_Id` = 4354 WHERE `entry` = 4576;
-- talioring generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 8519 WHERE `entry` = 4578;
UPDATE `creature_template` SET `gossip_menu_Id` = 8519 WHERE `entry` = 33580;
-- first aid wotlk gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 5856 WHERE `entry` = 4591;
-- first aid generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 5856 WHERE `entry` = 26956;
-- cooking trainer generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 5853 WHERE `entry` = 19369;
UPDATE `creature_template` SET `gossip_menu_Id` = 5853 WHERE `entry` = 33587;
UPDATE `creature_template` SET `gossip_menu_Id` = 5853 WHERE `entry` = 34785;
UPDATE `creature_template` SET `gossip_menu_Id` = 5853 WHERE `entry` = 34786;
-- fishing trainer generic gossips
UPDATE `creature_template` SET `gossip_menu_Id` = 5665 WHERE `entry` = 26993;
UPDATE `creature_template` SET `gossip_menu_Id` = 5665 WHERE `entry` = 26909;
UPDATE `creature_template` SET `gossip_menu_Id` = 5665 WHERE `entry` = 26957;

-- remove the repairer mouseover
UPDATE `creature_template` SET `npcflag` = 80 WHERE `entry` = 16278;
-- correct mount trainer npcflags
UPDATE `creature_template` SET `npcflag` = 81 WHERE `entry` = 3175;
UPDATE `creature_template` SET `npcflag` = 83 WHERE `entry` = 16280;

-- correct a few innkeepers
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536 WHERE `MenuId` = 1297 AND `OptionIndex` = 1;
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuId` = 1297 AND `OptionIndex` = 2;
DELETE FROM `gossip_menu_option_action` WHERE `MenuId` = 1297 AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_action` (`MenuId`, `OptionIndex`, `ActionMenuId`, `ActionPoiId`) VALUES (1297, 0, 1221, 0);
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuId` = 1293 AND `OptionIndex` = 0;
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536 WHERE `MenuId` = 1293 AND `OptionIndex` = 1;
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuId` = 1293 AND `OptionIndex` = 2;
DELETE FROM `gossip_menu_option_action` WHERE `MenuId` = 1293 AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_action` (`MenuId`, `OptionIndex`, `ActionMenuId`, `ActionPoiId`) VALUES (1293, 0, 1221, 0);
UPDATE `gossip_menu_option` SET `OptionIcon` = 0, `OptionText` = 'What can I do at an inn?', `OptionBroadcastTextId` = 4308 WHERE `MenuId` = 1294 AND `OptionIndex` = 2;
UPDATE `gossip_menu_option` SET `OptionIcon` = 0, `OptionText` = 'What can I do at an inn?', `OptionBroadcastTextId` = 4308 WHERE `MenuId` = 7468 AND `OptionIndex` = 2;