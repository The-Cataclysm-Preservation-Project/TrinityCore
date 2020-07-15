-- Correct low level class trainers.

-- Insert gossip menu options to trainers with no options using generic text.
DELETE FROM `gossip_menu_option` WHERE `OptionIndex` IN (0,1,2) AND `MenuId` IN (14137,14137,14138,14139,14140,14141,10675,10685,10686,10694,10699,10698,10697,10879,10878,10985,11185,11617,11620,11621,11645,11810,11831);
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) VALUES
(14137,0,3,'I require training.', 2756, 5, 16, 0),
(14137,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14137,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(14138,0,3,'I require training.', 2756, 5, 16, 0),
(14138,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14138,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(14139,0,3,'I require training.', 2756, 5, 16, 0),
(14139,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14139,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(14140,0,3,'I require training.', 2756, 5, 16, 0),
(14140,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14140,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(14141,0,3,'I require training.', 2756, 5, 16, 0),
(14141,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14141,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10675,0,3,'I require training.', 2756, 5, 16, 0),
(10675,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10675,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10685,0,3,'I require training.', 2756, 5, 16, 0),
(10685,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10685,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10686,0,3,'I require training.', 2756, 5, 16, 0),
(10686,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10686,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10694,0,3,'I require training.', 2756, 5, 16, 0),
(10694,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10694,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10699,0,3,'I require training.', 2756, 5, 16, 0),
(10699,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10699,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10698,0,3,'I require training.', 2756, 5, 16, 0),
(10698,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10698,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10697,0,3,'I require training.', 2756, 5, 16, 0),
(10697,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10697,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10879,0,3,'I require training.', 2756, 5, 16, 0),
(10879,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10879,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10878,0,3,'I require training.', 2756, 5, 16, 0),
(10878,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10878,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10985,0,3,'I require training.', 2756, 5, 16, 0),
(10985,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10985,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11185,0,3,'I require training.', 2756, 5, 16, 0),
(11185,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11185,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11617,0,3,'I require training.', 2756, 5, 16, 0),
(11617,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11617,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11620,0,3,'I require training.', 2756, 5, 16, 0),
(11620,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11620,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11621,0,3,'I require training.', 2756, 5, 16, 0),
(11621,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11621,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11645,0,3,'I require training.', 2756, 5, 16, 0),
(11645,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11645,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11810,0,3,'I require training.', 2756, 5, 16, 0),
(11810,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11810,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11831,0,3,'I require training.', 2756, 5, 16, 0),
(11831,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11831,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0);

-- insert option_actions for talent and dual spec gossips.
DELETE FROM `gossip_menu_option_action` WHERE `OptionIndex` IN (1,2) AND `MenuId` IN (14137,14137,14138,14139,14140,14141,10675,10685,10686,10694,10699,10698,10697,10879,10878,10985,11185,11617,11620,11621,11645,11810,11831);
INSERT INTO `gossip_menu_option_action` (`MenuId`, `OptionIndex`, `ActionMenuId`, `ActionPoiId`) VALUES
(14137,1,4461,0),
(14137,2,10371,0),
(14138,1,4461,0),
(14138,2,10371,0),
(14139,1,4461,0),
(14139,2,10371,0),
(14140,1,4461,0),
(14140,2,10371,0),
(14141,1,4461,0),
(14141,2,10371,0),
(10675,1,4461,0),
(10675,2,10371,0),
(10685,1,4461,0),
(10685,2,10371,0),
(10686,1,4461,0),
(10686,2,10371,0),
(10694,1,4461,0),
(10694,2,10371,0),
(10699,1,4461,0),
(10699,2,10371,0),
(10698,1,4461,0),
(10698,2,10371,0),
(10697,1,4461,0),
(10697,2,10371,0),
(10879,1,4461,0),
(10879,2,10371,0),
(10878,1,4461,0),
(10878,2,10371,0),
(10985,1,4461,0),
(10985,2,10371,0),
(11185,1,4461,0),
(11185,2,10371,0),
(11617,1,4461,0),
(11617,2,10371,0),
(11620,1,4461,0),
(11620,2,10371,0),
(11621,1,4461,0),
(11621,2,10371,0),
(11645,1,4461,0),
(11645,2,10371,0),
(11810,1,4461,0),
(11810,2,10371,0),
(11831,1,4461,0),
(11831,2,10371,0);
	
-- Correct one of the mage trainers.
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `MenuId` = 10683;

-- add gossip_menu_option_trainer records.
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4684,14137,14141,10675,10697,11185,4676,10686,10699,10985,14140,11621,4692,4679,14139,11617,7522,4652,10879,10878,11620,11810,4655,4643,14138,11645,11831,10702);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4684, 0, 145),
(14137, 0, 168),
(14141, 0, 15),
(10675, 0, 15),
(10697, 0, 15),
(11185, 0, 15),
(4676, 0, 17),
(10686, 0, 17),
(10699, 0, 17),
(10985, 0, 17),
(14140, 0, 17),
(11621, 0, 17),
(4692, 0, 127),
(4679, 0, 127),
(14139, 0, 127),
(11617, 0, 127),
(7522, 0, 134),
(4652, 0, 134),
(10879, 0, 134),
(10878, 0, 44),
(11620, 0, 44),
(11810, 0, 44),
(4655, 0, 32),
(4643, 0, 32),
(14138, 0, 32),
(11645, 0, 32),
(11831, 0, 32),
(10702, 0, 32);
	
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
-- Insert gossip menu options to trainers with no options using generic text.
DELETE FROM `gossip_menu_option` WHERE `OptionIndex` IN (0,1,2) AND `MenuId` IN (14136,11824,11906,4608,14144,10555,11052,11045,10814,10549,10554,11051,10553,10819,10818,11050,11049,10551,10816,12572,10815,12576,12513,12523,12532,12533,12925,12926,12835,10835,12528,12535,12517,10843,12051,12524,12531,12536,12928,12543,12564,12924,4560,4583,12004,12841,12049,12525,10702,12053,10840,12526,12921,4602,12882,2522,12845,10373,11878,11972,11997);
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) VALUES
(14136,0,3,'I require training.', 2756, 5, 16, 0),
(14136,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14136,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11824,0,3,'I require training.', 2756, 5, 16, 0),
(11824,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11824,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11906,0,3,'I require training.', 2756, 5, 16, 0),
(11906,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11906,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(4608,0,3,'I require training.', 2756, 5, 16, 0),
(4608,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(4608,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(14144,0,3,'I require training.', 2756, 5, 16, 0),
(14144,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(14144,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10555,0,3,'I require training.', 2756, 5, 16, 0),
(10555,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10555,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11052,0,3,'I require training.', 2756, 5, 16, 0),
(11052,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11052,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11045,0,3,'I require training.', 2756, 5, 16, 0),
(11045,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11045,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10814,0,3,'I require training.', 2756, 5, 16, 0),
(10814,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10814,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10549,0,3,'I require training.', 2756, 5, 16, 0),
(10549,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10549,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10554,0,3,'I require training.', 2756, 5, 16, 0),
(10554,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10554,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11051,0,3,'I require training.', 2756, 5, 16, 0),
(11051,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11051,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10553,0,3,'I require training.', 2756, 5, 16, 0),
(10553,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10553,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10819,0,3,'I require training.', 2756, 5, 16, 0),
(10819,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10819,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10818,0,3,'I require training.', 2756, 5, 16, 0),
(10818,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10818,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11050,0,3,'I require training.', 2756, 5, 16, 0),
(11050,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11050,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11049,0,3,'I require training.', 2756, 5, 16, 0),
(11049,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11049,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10551,0,3,'I require training.', 2756, 5, 16, 0),
(10551,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10551,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10816,0,3,'I require training.', 2756, 5, 16, 0),
(10816,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10816,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12572,0,3,'I require training.', 2756, 5, 16, 0),
(12572,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12572,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10815,0,3,'I require training.', 2756, 5, 16, 0),
(10815,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10815,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12576,0,3,'I require training.', 2756, 5, 16, 0),
(12576,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12576,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12513,0,3,'I require training.', 2756, 5, 16, 0),
(12513,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12513,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12523,0,3,'I require training.', 2756, 5, 16, 0),
(12523,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12523,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12532,0,3,'I require training.', 2756, 5, 16, 0),
(12532,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12532,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12533,0,3,'I require training.', 2756, 5, 16, 0),
(12533,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12533,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12925,0,3,'I require training.', 2756, 5, 16, 0),
(12925,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12925,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12926,0,3,'I require training.', 2756, 5, 16, 0),
(12926,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12926,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12835,0,3,'I require training.', 2756, 5, 16, 0),
(12835,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12835,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10835,0,3,'I require training.', 2756, 5, 16, 0),
(10835,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10835,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12528,0,3,'I require training.', 2756, 5, 16, 0),
(12528,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12528,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12535,0,3,'I require training.', 2756, 5, 16, 0),
(12535,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12535,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12517,0,3,'I require training.', 2756, 5, 16, 0),
(12517,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12517,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10843,0,3,'I require training.', 2756, 5, 16, 0),
(10843,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10843,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12051,0,3,'I require training.', 2756, 5, 16, 0),
(12051,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12051,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12524,0,3,'I require training.', 2756, 5, 16, 0),
(12524,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12524,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12531,0,3,'I require training.', 2756, 5, 16, 0),
(12531,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12531,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12536,0,3,'I require training.', 2756, 5, 16, 0),
(12536,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12536,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12928,0,3,'I require training.', 2756, 5, 16, 0),
(12928,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12928,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12543,0,3,'I require training.', 2756, 5, 16, 0),
(12543,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12543,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12564,0,3,'I require training.', 2756, 5, 16, 0),
(12564,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12564,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12924,0,3,'I require training.', 2756, 5, 16, 0),
(12924,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12924,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(4560,0,3,'I require training.', 2756, 5, 16, 0),
(4560,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(4560,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(4583,0,3,'I require training.', 2756, 5, 16, 0),
(4583,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(4583,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12004,0,3,'I require training.', 2756, 5, 16, 0),
(12004,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12004,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12841,0,3,'I require training.', 2756, 5, 16, 0),
(12841,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12841,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12049,0,3,'I require training.', 2756, 5, 16, 0),
(12049,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12049,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12525,0,3,'I require training.', 2756, 5, 16, 0),
(12525,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12525,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10702,0,3,'I require training.', 2756, 5, 16, 0),
(10702,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10702,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12053,0,3,'I require training.', 2756, 5, 16, 0),
(12053,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12053,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10840,0,3,'I require training.', 2756, 5, 16, 0),
(10840,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10840,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12526,0,3,'I require training.', 2756, 5, 16, 0),
(12526,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12526,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12921,0,3,'I require training.', 2756, 5, 16, 0),
(12921,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12921,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(4602,0,3,'I require training.', 2756, 5, 16, 0),
(4602,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(4602,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12882,0,3,'I require training.', 2756, 5, 16, 0),
(12882,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12882,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(2522,0,3,'I require training.', 2756, 5, 16, 0),
(2522,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(2522,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(12845,0,3,'I require training.', 2756, 5, 16, 0),
(12845,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(12845,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(10373,0,3,'I require training.', 2756, 5, 16, 0),
(10373,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(10373,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11878,0,3,'I require training.', 2756, 5, 16, 0),
(11878,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11878,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11972,0,3,'I require training.', 2756, 5, 16, 0),
(11972,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11972,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0),
(11997,0,3,'I require training.', 2756, 5, 16, 0),
(11997,1,0,'I wish to unlearn my talents.',62295,16,16,0),
(11997,2,0,'I wish to know about Dual Talent Specialization.',33762,20,1,0);

-- insert option_actions for talent and dual spec gossips.
DELETE FROM `gossip_menu_option_action` WHERE `OptionIndex` IN (1,2) AND `MenuId` IN (14136,11824,11906,4608,14144,10555,11052,11045,10814,10549,10554,11051,10553,10819,10818,11050,11049,10551,10816,12572,10815,12576,12513,12523,12532,12533,12925,12926,12835,10835,12528,12535,12517,10843,12051,12524,12531,12536,12928,12543,12564,12924,4560,4583,12004,12841,12049,12525,10702,12053,10840,12526,12921,4602,12882,2522,12845,10373,11878,11972,11997);
INSERT INTO `gossip_menu_option_action` (`MenuId`, `OptionIndex`, `ActionMenuId`, `ActionPoiId`) VALUES
(14136,1,4461,0),
(14136,2,10371,0),
(11824,1,4461,0),
(11824,2,10371,0),
(11906,1,4461,0),
(11906,2,10371,0),
(4608,1,4461,0),
(4608,2,10371,0),
(14144,1,4461,0),
(14144,2,10371,0),
(10555,1,4461,0),
(10555,2,10371,0),
(11052,1,4461,0),
(11052,2,10371,0),
(11045,1,4461,0),
(11045,2,10371,0),
(10814,1,4461,0),
(10814,2,10371,0),
(10549,1,4461,0),
(10549,2,10371,0),
(10554,1,4461,0),
(10554,2,10371,0),
(11051,1,4461,0),
(11051,2,10371,0),
(10553,1,4461,0),
(10553,2,10371,0),
(10819,1,4461,0),
(10819,2,10371,0),
(10818,1,4461,0),
(10818,2,10371,0),
(11050,1,4461,0),
(11050,2,10371,0),
(11049,1,4461,0),
(11049,2,10371,0),
(10551,1,4461,0),
(10551,2,10371,0),
(10816,1,4461,0),
(10816,2,10371,0),
(12572,1,4461,0),
(12572,2,10371,0),
(10815,1,4461,0),
(10815,2,10371,0),
(12576,1,4461,0),
(12576,2,10371,0),
(12513,1,4461,0),
(12513,2,10371,0),
(12523,1,4461,0),
(12523,2,10371,0),
(12532,1,4461,0),
(12532,2,10371,0),
(12533,1,4461,0),
(12533,2,10371,0),
(12925,1,4461,0),
(12925,2,10371,0),
(12926,1,4461,0),
(12926,2,10371,0),
(12835,1,4461,0),
(12835,2,10371,0),
(10835,1,4461,0),
(10835,2,10371,0),
(12528,1,4461,0),
(12528,2,10371,0),
(12535,1,4461,0),
(12535,2,10371,0),
(12517,1,4461,0),
(12517,2,10371,0),
(10843,1,4461,0),
(10843,2,10371,0),
(12051,1,4461,0),
(12051,2,10371,0),
(12524,1,4461,0),
(12524,2,10371,0),
(12531,1,4461,0),
(12531,2,10371,0),
(12536,1,4461,0),
(12536,2,10371,0),
(12928,1,4461,0),
(12928,2,10371,0),
(12543,1,4461,0),
(12543,2,10371,0),
(12564,1,4461,0),
(12564,2,10371,0),
(12924,1,4461,0),
(12924,2,10371,0),
(4560,1,4461,0),
(4560,2,10371,0),
(4583,1,4461,0),
(4583,2,10371,0),
(12004,1,4461,0),
(12004,2,10371,0),
(12841,1,4461,0),
(12841,2,10371,0),
(12049,1,4461,0),
(12049,2,10371,0),
(12525,1,4461,0),
(12525,2,10371,0),
(10702,1,4461,0),
(10702,2,10371,0),
(12053,1,4461,0),
(12053,2,10371,0),
(10840,1,4461,0),
(10840,2,10371,0),
(12526,1,4461,0),
(12526,2,10371,0),
(12921,1,4461,0),
(12921,2,10371,0),
(4602,1,4461,0),
(4602,2,10371,0),
(12882,1,4461,0),
(12882,2,10371,0),
(2522,1,4461,0),
(2522,2,10371,0),
(12845,1,4461,0),
(12845,2,10371,0),
(10373,1,4461,0),
(10373,2,10371,0),
(11878,1,4461,0),
(11878,2,10371,0),
(11972,1,4461,0),
(11972,2,10371,0),
(11997,1,4461,0),
(11997,2,10371,0);

-- Correct flags on numerous trainer gossip_options.
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `OptionIcon` = 3 AND `MenuId` IN (141, 11912, 11913, 12050, 4105, 11932, 12606, 10552, 10817, 11766, 11875, 12344, 12151, 11879, 12048, 12341, 10838, 12010, 12537, 12714, 10684, 12052, 4555, 4553, 10837, 12918, 12716, 12521, 12821, 12522, 12746, 12927, 12755, 12920);

-- add `gossip_menu_option_trainer` records.
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4683,11052,4526,4525,4527,7263,12513,12532,12533,12925,4662,12516,12527,12926,14144,11045,10814,10549,12835,4694,4674,4621,12528,12535,12924,4092,410,10843,10554,11051,10819,12517,12051,12524,12531,12536,12928,12543,12564,7265,4680,10553,4691,11050,10818,4574,4573,4572,4559,4558,4560,3643,4801,3642,12004,12841,4105,7467,11049,4516,4515,5123,11878,12052,64,6648,10551,10816,12572,4534,4536,4535,4539,4538,4537,4553,4583,14136,12049,12233,12525,4641,11932,4608,4656,10840,4603,4604,4609,4610,4504,1522,12053,12821,12526,12921,4602,12882,50723,50729,2522,12845,10373,4605,4646,4687,11997,12755);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4683, 0, 16),
(11052, 0, 16),
(4526, 0, 16),
(4525, 0, 16),
(4527, 0, 16),
(7263, 0, 16),
(12513, 0, 16),
(12532, 0, 16),
(12533, 0, 16),
(12925, 0, 16),
(4662, 0, 164),
(12516, 0, 164),
(12527, 0, 164),
(12926, 0, 164),
(14144, 0, 40),
(11045, 0, 40),
(10814, 0, 40),
(10549, 0, 40),
(12835, 0, 40),
(4694, 0, 40),
(4674, 0, 40),
(4621, 0, 40),
(12528, 0, 40),
(12535, 0, 40),
(12924, 0, 40),
(4092, 0, 40),
(410, 0, 33),
(10843, 0, 33),
(10554, 0, 33),
(11051, 0, 33),
(10819, 0, 33),
(12517, 0, 33),
(12051, 0, 33),
(12524, 0, 33),
(12531, 0, 33),
(12536, 0, 33),
(12928, 0, 33),
(12543, 0, 33),
(12564, 0, 33),
(7265, 0, 3),
(4680, 0, 3),
(10553, 0, 3),
(4691, 0, 3),
(11050, 0, 3),
(10818, 0, 3),
(4574, 0, 3),
(4573, 0, 3),
(4572, 0, 3),
(4559, 0, 3),
(4558, 0, 3),
(4560, 0, 3),
(3643, 0, 3),
(4801, 0, 3),
(3642, 0, 3),
(12004, 0, 3),
(12841, 0, 3),
(4105, 0, 124),
(7467, 0, 124),
(11049, 0, 124),
(4516, 0, 124),
(4515, 0, 124),
(5123, 0, 124),
(11878, 0, 124),
(12052, 0, 124),
(64, 0, 135),
(6648, 0, 135),
(10551, 0, 135),
(10816, 0, 135),
(12572, 0, 135),
(4534, 0, 135),
(4536, 0, 135),
(4535, 0, 135),
(4539, 0, 135),
(4538, 0, 135),
(4537, 0, 135),
(4553, 0, 135),
(4583, 0, 135),
(14136, 0, 135),
(12049, 0, 135),
(12233, 0, 135),
(12525, 0, 135),
(4641, 0, 154),
(11932, 0, 154),
(4608, 0, 154),
(4656, 0, 154),
(10840, 0, 154),
(4603, 0, 154),
(4604, 0, 154),
(4609, 0, 154),
(4610, 0, 154),
(4504, 0, 154),
(1522, 0, 154),
(12053, 0, 154),
(12821, 0, 154),
(12526, 0, 154),
(12921, 0, 154),
(4602, 0, 154),
(12882, 0, 154),
(50723, 3, 154),
(50729, 3, 154),
(2522, 0, 154),
(12845, 0, 154),
(10373, 0, 39),
(4605, 0, 39),
(4646, 0, 39),
(4687, 0, 39),
(11997, 0, 39),
(12755, 0, 39);

-- add missing gossip_menu_option_trainer for Portal Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4824,11972,4827);
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4824, 0, 130),
(11972, 0, 149),
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
UPDATE `creature_template` SET `gossip_menu_Id` = 10835 WHERE `entry` = 38467;
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
UPDATE `creature_template` SET `gossip_menu_id` = 4543 WHERE `entry` = 4608;
-- Sister Almyra gossips
UPDATE `creature_template` SET `gossip_menu_id` = 10838 WHERE `entry` = 38466;
-- priest generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 49901;
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 49950;
UPDATE `creature_template` SET `gossip_menu_id` = 4680 WHERE `entry` = 48614;
-- mage wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4583 WHERE `entry` = 4567;
UPDATE `creature_template` SET `gossip_menu_id` = 4537 WHERE `entry` = 4568;
UPDATE `creature_template` SET `gossip_menu_id` = 4485 WHERE `entry` = 5497;
UPDATE `creature_template` SET `gossip_menu_id` = 63 WHERE `entry` = 	23103;
-- mage generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 14136 WHERE `entry` = 49952;
UPDATE `creature_template` SET `gossip_menu_id` = 14136 WHERE `entry` = 50714;
UPDATE `creature_template` SET `gossip_menu_id` = 14136 WHERE `entry` = 50715;
UPDATE `creature_template` SET `gossip_menu_id` = 14136 WHERE `entry` = 50717;
UPDATE `creature_template` SET `gossip_menu_id` = 14136 WHERE `entry` = 50720;
-- warlock wotlk gossips
UPDATE `creature_template` SET `gossip_menu_id` = 2383 WHERE `entry` = 4563;
UPDATE `creature_template` SET `gossip_menu_id` = 4609 WHERE `entry` = 4564;
-- warlock generic gossips
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 49945;
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 50723;
UPDATE `creature_template` SET `gossip_menu_id` = 4610 WHERE `entry` = 50729;
-- tauren druid generic gossip
UPDATE `creature_template` SET `gossip_menu_id` = 4605 WHERE `entry` = 44726;

-- correct low level class trainers who share gossips with standard class trainers.
SET @Frang := 3153;
SET @MenuId := 523;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId, @newGossipMenuOptionIndex, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 145);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Frang, 5, 0, 0, 'Show gossip when Frang is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Frang, 5, 0, 1, 'Show gossip when Frang is not present');
	
SET @DeliosSilverBlade := 43010;
SET @MenuId := 11793;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 145);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DeliosSilverBlade, 5, 0, 0, 'Show gossip when Delios Silver Blade is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @DeliosSilverBlade, 5, 0, 1, 'Show gossip when Delios Silver Blade is not present');

SET @Kore := 16503;
SET @MenuId := 7263;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 145);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Kore, 5, 0, 0, 'Show gossip when Kore is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Kore, 5, 0, 1, 'Show gossip when Kore is not present');
	
SET @SunwalkerHelaku := 37737;
SET @MenuId := 11767;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 168);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @SunwalkerHelaku, 5, 0, 0, 'Show gossip when Sunwalker Helaku is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @SunwalkerHelaku, 5, 0, 1, 'Show gossip when Sunwalker Helaku is not present');
	
SET @Aurelon := 16501;
SET @MenuId := 7260;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 168);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Aurelon, 5, 0, 0, 'Show gossip when Aurelon is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Aurelon, 5, 0, 1, 'Show gossip when Aurelon is not present');
	
SET @Keilnei := 16499;
SET @MenuId := 7262;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 15);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Keilnei, 5, 0, 0, 'Show gossip when Keilnei is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Keilnei, 5, 0, 1, 'Show gossip when Keilnei is not present');
	
SET @LankaFarshot := 3061;
SET @MenuId := 4647;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 15);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @LankaFarshot, 5, 0, 0, 'Show gossip when Lanka Farshot is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @LankaFarshot, 5, 0, 1, 'Show gossip when Lanka Farshot is not present');
	
SET @Rwag := 3155;
SET @MenuId := 141;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 10;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 17);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES 
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Rwag, 5, 0, 0, 'Show gossip when Rwag is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Rwag, 5, 0, 1, 'Show gossip when Rwag is not present');

SET @DavidTrias := 2122;
SET @MenuId := 85;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 5;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 17);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DavidTrias, 5, 0, 0, 'Show gossip when David Trias is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @DavidTrias, 5, 0, 1, 'Show gossip when David Trias is not present');
	
SET @DarkClericDuesten := 2123;
SET @MenuId := 3645;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 127);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DarkClericDuesten, 5, 0, 0, 'Show gossip when Dark Cleric Duesten is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @DarkClericDuesten, 5, 0, 1, 'Show gossip when Dark Cleric Duesten is not present');

SET @SeerRavenfeather := 37724;
SET @MenuId := 11766;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 1;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 127);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @SeerRavenfeather, 5, 0, 0, 'Show gossip when Seer Ravenfeather is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @SeerRavenfeather, 5, 0, 1, 'Show gossip when Seer Ravenfeather is not present');
	
SET @MeelaDawnstrider := 3062;
SET @MenuId := 4103;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 134);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @MeelaDawnstrider, 5, 0, 0, 'Show gossip when Meela Dawnstrider is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @MeelaDawnstrider, 5, 0, 1, 'Show gossip when Meela Dawnstrider is not present');

SET @JuliaSunstriker := 15279;
SET @MenuId := 6648;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 44);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @JuliaSunstriker, 5, 0, 0, 'Show gossip when Julia Sunstriker is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @JuliaSunstriker, 5, 0, 1, 'Show gossip when Julia Sunstriker is not present');

SET @Valaatu := 16500;
SET @MenuId := 7264;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 3;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 44);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Valaatu, 5, 0, 0, 'Show gossip when Valaatu is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Valaatu, 5, 0, 1, 'Show gossip when Valaatu is not present');

-- Update a Horde Portal trainer who shares a gossip with an Alliance Portal trainer.
SET @Narinth := 16654;
SET @MenuId := 4824;
SET @trainingGossipOptionIndex := 0;
SET @newGossipMenuOptionIndex := 1;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`, `VerifiedBuild`) SELECT @MenuId AS `MenuId`, @newGossipMenuOptionIndex AS `OptionIndex`, `OptionIcon`, `OptionText`,`OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild` FROM `gossip_menu_option` WHERE `MenuId` = @MenuId AND `OptionIndex` = @trainingGossipOptionIndex;
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` = @MenuId AND `OptionIndex` = @newGossipMenuOptionIndex;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES (@MenuId, @newGossipMenuOptionIndex, 149);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @MenuId AND `SourceEntry` IN (@newGossipMenuOptionIndex,@trainingGossipOptionIndex) AND `ConditionTypeOrReference` = 29;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Narinth, 5, 0, 0, 'Show gossip when Narinth is present'),
(15, @MenuId, @trainingGossipOptionIndex, 0, 29, 0, @Narinth, 5, 0, 1, 'Show gossip when Narinth is not present');


-- Correct Profession Trainers
-- add missing gossip_menu_option for Alchemists.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4111,4110,4117,8863) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES 
(4111, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
(4110, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
(4117, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
(8863, 0, 3, 'Train me in Alchemy.', 0, 5, 16);

-- add gossip data from wotlk for Blacksmiths.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (1043) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(1043, 0, 3, 'Train me.', 3266, 5, 16);

-- add missing gossip_menu_option for Blacksmiths.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (2741,2749,1042,1041,3203,3202,8760,7809,9131,9132,8254) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(2741, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(2749, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(1042, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(1041, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(3203, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(3202, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(8760, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(7809, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(9131, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(9132, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
(8254, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16);

-- add missing gossip_menu_option for Enchanters.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4169,4156,4171,4170) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(4169, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
(4156, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
(4171, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
(4170, 0, 3, 'Train me in Enchanting.', 0, 5, 16);

-- add missing gossip_menu_option for Engineers.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4136,1469,1465,7512,7513,7820,10363) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(4136, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(1469, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(1465, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(7512, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(7513, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(7820, 0, 3, 'Train me in Engineering.', 0, 5, 16),
(10363, 0, 3, 'Train me in Engineering.', 0, 5, 16);

-- add missing gossip_menu_option for Leatherworkers.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4208,4842,4244) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(4208, 0, 3, 'Train me in Leatherworking.', 0, 5, 16),
(4842, 0, 3, 'Train me in Leatherworking.', 0, 5, 16),
(4244, 0, 3, 'Train me in Leatherworking.', 0, 5, 16);

-- add missing gossip_menu_option for Tailors.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4356,10138) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(4356, 0, 3, 'Train me in Tailoring.', 0, 5, 16),
(10138, 0, 3, 'Train me in Tailoring.', 0, 5, 16);

-- add missing gossip_menu_option for First Aid Trainers.
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (8522) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcflag`) VALUES
(8522, 0, 3, 'Train me in First Aide.', 0, 5, 16);

-- correct trainer flags in gossip menu options
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `OptionIcon` = 3 AND `MenuId` IN (4361, 4134, 4129, 4164, 7455, 7817, 9084, 12846, 8460, 6087, 10437, 2021, 51997);

-- add missing gossip_menu_option_trainer for Apprentice Blacksmiths
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (2746,2741) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(2746, 0, 80),
(2741, 0, 80);

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
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4111,7384,4110,4115,4127,4129,2391,2837,4117,16161,8540,8733,8863,10627) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4111, 0, 122),
(7384, 0, 122),
(4110, 0, 122),
(4115, 0, 122),
(4127, 0, 122),
(4129, 0, 122),
(2391, 0, 122),
(2837, 0, 122),
(4117, 0, 122),
(16161, 0, 122),
(8540, 0, 122),
(8733, 0, 122),
(8863, 0, 122),
(10627, 0, 122);

-- add missing gossip_menu_option_trainer for Blacksmiths
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (2781,2783,2749,593,597,1022,1043,1042,1041,3203,3202,7256,8760,7809,9131,9132,8254,9459) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(2781, 0, 58),
(2783, 0, 58),
(2749, 0, 58),
(593, 0, 58),
(597, 0, 58),
(1022, 0, 58),
(1043, 0, 58),
(1042, 0, 58),
(1041, 0, 58),
(3203, 0, 58),
(3202, 0, 58),
(7256, 0, 58),
(8760, 0, 58),
(7809, 0, 58),
(9131, 0, 58),
(9132, 0, 58),
(8254, 0, 58),
(9459, 0, 58);

-- add missing gossip_menu_option_trainer for Enchanters
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4169,4166,4171,4170,8731,8866,10365,33676) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4169, 0, 62),
(4166, 0, 62),
(4171, 0, 62),
(4170, 0, 62),
(8731, 0, 62),
(8866, 0, 62),
(10365, 0, 62),
(33676, 0, 62);

-- add missing gossip_menu_option_trainer for Engineers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4145,4136,1469,1465,4149,8656,7512,7513,7820,9084,10363) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4145, 0, 407),
(4136, 0, 407),
(1469, 0, 407),
(1465, 0, 407),
(4149, 0, 407),
(8656, 0, 407),
(7512, 0, 407),
(7513, 0, 407),
(7820, 0, 407),
(9084, 0, 407),
(10363, 0, 407);

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
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4108,4172,4208,4210,4243,7866,10361,4842,4843,4244,8732,7816,12852) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4108, 0, 56),
(4172, 0, 56),
(4208, 0, 56),
(4210, 0, 56),
(4243, 0, 56),
(7866, 0, 56),
(10361, 0, 56),
(4842, 0, 56),
(4843, 0, 56),
(4244, 0, 56),
(8732, 0, 56),
(7816, 0, 56),
(12852, 0, 56);

-- add missing gossip_menu_option_trainer for Tailors
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4261,4356,4348,4270,4267,4354,4844,7524,8868,10114,10116,10117,10138,10364) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES 
(4261, 0, 163),
(4356, 0, 163),
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
(10138, 0, 163),
(10364, 0, 163);

-- add missing gossip_menu_option_trainer for First Aid Trainers
DELETE FROM `gossip_menu_option_trainer` WHERE `MenuId` IN (4761,12939,8522,8502) AND `OptionIndex` = 0;
INSERT INTO `gossip_menu_option_trainer` (`MenuId`, `OptionIndex`, `TrainerId`) VALUES
(4761, 0, 107),
(12939, 0, 107),
(8522, 0, 107),
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