-- Correct low level class trainers.

-- Insert gossip menu options to trainers with no options using generic text.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
values(14137,'0','3','I require training.', '2756', '5', '16', '0'),
	(14137,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14137,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(14138,'0','3','I require training.', '2756', '5', '16', '0'),
	(14138,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14138,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(14139,'0','3','I require training.', '2756', '5', '16', '0'),
	(14139,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14139,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(14140,'0','3','I require training.', '2756', '5', '16', '0'),
	(14140,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14140,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(14141,'0','3','I require training.', '2756', '5', '16', '0'),
	(14141,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14141,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10675,'0','3','I require training.', '2756', '5', '16', '0'),
	(10675,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10675,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10685,'0','3','I require training.', '2756', '5', '16', '0'),
	(10685,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10685,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10686,'0','3','I require training.', '2756', '5', '16', '0'),
	(10686,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10686,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10694,'0','3','I require training.', '2756', '5', '16', '0'),
	(10694,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10694,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10699,'0','3','I require training.', '2756', '5', '16', '0'),
	(10699,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10699,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10698,'0','3','I require training.', '2756', '5', '16', '0'),
	(10698,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10698,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10697,'0','3','I require training.', '2756', '5', '16', '0'),
	(10697,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10697,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10879,'0','3','I require training.', '2756', '5', '16', '0'),
	(10879,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10879,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10878,'0','3','I require training.', '2756', '5', '16', '0'),
	(10878,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10878,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10985,'0','3','I require training.', '2756', '5', '16', '0'),
	(10985,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10985,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11185,'0','3','I require training.', '2756', '5', '16', '0'),
	(11185,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11185,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11617,'0','3','I require training.', '2756', '5', '16', '0'),
	(11617,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11617,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11620,'0','3','I require training.', '2756', '5', '16', '0'),
	(11620,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11620,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11621,'0','3','I require training.', '2756', '5', '16', '0'),
	(11621,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11621,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11645,'0','3','I require training.', '2756', '5', '16', '0'),
	(11645,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11645,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11810,'0','3','I require training.', '2756', '5', '16', '0'),
	(11810,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11810,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11831,'0','3','I require training.', '2756', '5', '16', '0'),
	(11831,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11831,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0');

-- insert option_actions for talent and dual spec gossips.
insert into gossip_menu_option_action
(MenuId, OptionIndex, ActionMenuId, ActionPoiId)
values (14137,'1','4461','0'),
	(14137,'2','10371','0'),
	(14138,'1','4461','0'),
	(14138,'2','10371','0'),
	(14139,'1','4461','0'),
	(14139,'2','10371','0'),
	(14140,'1','4461','0'),
	(14140,'2','10371','0'),
	(14141,'1','4461','0'),
	(14141,'2','10371','0'),
	(10675,'1','4461','0'),
	(10675,'2','10371','0'),
	(10685,'1','4461','0'),
	(10685,'2','10371','0'),
	(10686,'1','4461','0'),
	(10686,'2','10371','0'),
	(10694,'1','4461','0'),
	(10694,'2','10371','0'),
	(10699,'1','4461','0'),
	(10699,'2','10371','0'),
	(10698,'1','4461','0'),
	(10698,'2','10371','0'),
	(10697,'1','4461','0'),
	(10697,'2','10371','0'),
	(10879,'1','4461','0'),
	(10879,'2','10371','0'),
	(10878,'1','4461','0'),
	(10878,'2','10371','0'),
	(10985,'1','4461','0'),
	(10985,'2','10371','0'),
	(11185,'1','4461','0'),
	(11185,'2','10371','0'),
	(11617,'1','4461','0'),
	(11617,'2','10371','0'),
	(11620,'1','4461','0'),
	(11620,'2','10371','0'),
	(11621,'1','4461','0'),
	(11621,'2','10371','0'),
	(11645,'1','4461','0'),
	(11645,'2','10371','0'),
	(11810,'1','4461','0'),
	(11810,'2','10371','0'),
	(11831,'1','4461','0'),
	(11831,'2','10371','0');
	
-- Correct one of the mage trainers.
update gossip_menu_option set OptionType = 5, OptionNpcflag = 16 where MenuId = 10683;

-- add gossip_menu_option_trainer records.
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4684, 0, 145),
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
update gossip_menu_option_trainer set TrainerId = 127 where MenuId = 7349;
update gossip_menu_option_trainer set TrainerId = 127 where MenuId = 13921;
update gossip_menu_option_trainer set TrainerId = 127 where MenuId = 10700;

-- give Drill Sergeant Steamcrank a low level gossip
update creature_template set gossip_menu_id = 4645 where entry = 42324;
-- give Nekali a low level gossip
update creature_template set gossip_menu_id = 10684 where entry = 38242;
-- mage wotlk gossip
update creature_template set gossip_menu_id = 6648 where entry = 15279;


-- Correct remaining class trainers

-- Insert gossip menu options to trainers with no options using generic text.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
values (14136,'0','3','I require training.', '2756', '5', '16', '0'),
	(14136,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14136,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11824,'0','3','I require training.', '2756', '5', '16', '0'),
	(11824,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11824,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11906,'0','3','I require training.', '2756', '5', '16', '0'),
	(11906,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11906,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(4608,'0','3','I require training.', '2756', '5', '16', '0'),
	(4608,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(4608,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(14144,'0','3','I require training.', '2756', '5', '16', '0'),
	(14144,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(14144,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10555,'0','3','I require training.', '2756', '5', '16', '0'),
	(10555,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10555,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11052,'0','3','I require training.', '2756', '5', '16', '0'),
	(11052,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11052,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11045,'0','3','I require training.', '2756', '5', '16', '0'),
	(11045,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11045,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10814,'0','3','I require training.', '2756', '5', '16', '0'),
	(10814,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10814,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10549,'0','3','I require training.', '2756', '5', '16', '0'),
	(10549,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10549,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10554,'0','3','I require training.', '2756', '5', '16', '0'),
	(10554,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10554,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11051,'0','3','I require training.', '2756', '5', '16', '0'),
	(11051,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11051,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10553,'0','3','I require training.', '2756', '5', '16', '0'),
	(10553,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10553,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10819,'0','3','I require training.', '2756', '5', '16', '0'),
	(10819,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10819,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10818,'0','3','I require training.', '2756', '5', '16', '0'),
	(10818,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10818,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11050,'0','3','I require training.', '2756', '5', '16', '0'),
	(11050,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11050,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11049,'0','3','I require training.', '2756', '5', '16', '0'),
	(11049,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11049,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10551,'0','3','I require training.', '2756', '5', '16', '0'),
	(10551,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10551,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10816,'0','3','I require training.', '2756', '5', '16', '0'),
	(10816,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10816,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12572,'0','3','I require training.', '2756', '5', '16', '0'),
	(12572,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12572,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10815,'0','3','I require training.', '2756', '5', '16', '0'),
	(10815,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10815,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12576,'0','3','I require training.', '2756', '5', '16', '0'),
	(12576,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12576,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12513,'0','3','I require training.', '2756', '5', '16', '0'),
	(12513,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12513,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12523,'0','3','I require training.', '2756', '5', '16', '0'),
	(12523,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12523,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12532,'0','3','I require training.', '2756', '5', '16', '0'),
	(12532,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12532,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12533,'0','3','I require training.', '2756', '5', '16', '0'),
	(12533,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12533,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12925,'0','3','I require training.', '2756', '5', '16', '0'),
	(12925,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12925,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12926,'0','3','I require training.', '2756', '5', '16', '0'),
	(12926,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12926,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12835,'0','3','I require training.', '2756', '5', '16', '0'),
	(12835,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12835,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10835,'0','3','I require training.', '2756', '5', '16', '0'),
	(10835,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10835,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12528,'0','3','I require training.', '2756', '5', '16', '0'),
	(12528,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12528,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12535,'0','3','I require training.', '2756', '5', '16', '0'),
	(12535,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12535,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12517,'0','3','I require training.', '2756', '5', '16', '0'),
	(12517,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12517,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10843,'0','3','I require training.', '2756', '5', '16', '0'),
	(10843,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10843,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12051,'0','3','I require training.', '2756', '5', '16', '0'),
	(12051,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12051,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12524,'0','3','I require training.', '2756', '5', '16', '0'),
	(12524,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12524,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12531,'0','3','I require training.', '2756', '5', '16', '0'),
	(12531,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12531,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12536,'0','3','I require training.', '2756', '5', '16', '0'),
	(12536,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12536,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12928,'0','3','I require training.', '2756', '5', '16', '0'),
	(12928,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12928,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12543,'0','3','I require training.', '2756', '5', '16', '0'),
	(12543,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12543,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12564,'0','3','I require training.', '2756', '5', '16', '0'),
	(12564,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12564,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12924,'0','3','I require training.', '2756', '5', '16', '0'),
	(12924,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12924,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(4560,'0','3','I require training.', '2756', '5', '16', '0'),
	(4560,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(4560,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(4583,'0','3','I require training.', '2756', '5', '16', '0'),
	(4583,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(4583,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12004,'0','3','I require training.', '2756', '5', '16', '0'),
	(12004,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12004,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12841,'0','3','I require training.', '2756', '5', '16', '0'),
	(12841,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12841,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12049,'0','3','I require training.', '2756', '5', '16', '0'),
	(12049,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12049,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12525,'0','3','I require training.', '2756', '5', '16', '0'),
	(12525,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12525,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10702,'0','3','I require training.', '2756', '5', '16', '0'),
	(10702,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10702,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12053,'0','3','I require training.', '2756', '5', '16', '0'),
	(12053,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12053,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10840,'0','3','I require training.', '2756', '5', '16', '0'),
	(10840,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10840,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12526,'0','3','I require training.', '2756', '5', '16', '0'),
	(12526,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12526,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12921,'0','3','I require training.', '2756', '5', '16', '0'),
	(12921,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12921,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(4602,'0','3','I require training.', '2756', '5', '16', '0'),
	(4602,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(4602,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12882,'0','3','I require training.', '2756', '5', '16', '0'),
	(12882,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12882,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(2522,'0','3','I require training.', '2756', '5', '16', '0'),
	(2522,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(2522,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(12845,'0','3','I require training.', '2756', '5', '16', '0'),
	(12845,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(12845,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(10373,'0','3','I require training.', '2756', '5', '16', '0'),
	(10373,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(10373,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11878,'0','3','I require training.', '2756', '5', '16', '0'),
	(11878,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11878,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11972,'0','3','I require training.', '2756', '5', '16', '0'),
	(11972,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11972,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0'),
	(11997,'0','3','I require training.', '2756', '5', '16', '0'),
	(11997,'1','0','I wish to unlearn my talents.','62295','16','16','0'),
	(11997,'2','0','I wish to know about Dual Talent Specialization.','33762','20','1','0');

-- insert option_actions for talent and dual spec gossips.
insert into gossip_menu_option_action
(MenuId, OptionIndex, ActionMenuId, ActionPoiId)
values (14136,'1','4461','0'),
	(14136,'2','10371','0'),
	(11824,'1','4461','0'),
	(11824,'2','10371','0'),
	(11906,'1','4461','0'),
	(11906,'2','10371','0'),
	(4608,'1','4461','0'),
	(4608,'2','10371','0'),
	(14144,'1','4461','0'),
	(14144,'2','10371','0'),
	(10555,'1','4461','0'),
	(10555,'2','10371','0'),
	(11052,'1','4461','0'),
	(11052,'2','10371','0'),
	(11045,'1','4461','0'),
	(11045,'2','10371','0'),
	(10814,'1','4461','0'),
	(10814,'2','10371','0'),
	(10549,'1','4461','0'),
	(10549,'2','10371','0'),
	(10554,'1','4461','0'),
	(10554,'2','10371','0'),
	(11051,'1','4461','0'),
	(11051,'2','10371','0'),
	(10553,'1','4461','0'),
	(10553,'2','10371','0'),
	(10819,'1','4461','0'),
	(10819,'2','10371','0'),
	(10818,'1','4461','0'),
	(10818,'2','10371','0'),
	(11050,'1','4461','0'),
	(11050,'2','10371','0'),
	(11049,'1','4461','0'),
	(11049,'2','10371','0'),
	(10551,'1','4461','0'),
	(10551,'2','10371','0'),
	(10816,'1','4461','0'),
	(10816,'2','10371','0'),
	(12572,'1','4461','0'),
	(12572,'2','10371','0'),
	(10815,'1','4461','0'),
	(10815,'2','10371','0'),
	(12576,'1','4461','0'),
	(12576,'2','10371','0'),
	(12513,'1','4461','0'),
	(12513,'2','10371','0'),
	(12523,'1','4461','0'),
	(12523,'2','10371','0'),
	(12532,'1','4461','0'),
	(12532,'2','10371','0'),
	(12533,'1','4461','0'),
	(12533,'2','10371','0'),
	(12925,'1','4461','0'),
	(12925,'2','10371','0'),
	(12926,'1','4461','0'),
	(12926,'2','10371','0'),
	(12835,'1','4461','0'),
	(12835,'2','10371','0'),
	(10835,'1','4461','0'),
	(10835,'2','10371','0'),
	(12528,'1','4461','0'),
	(12528,'2','10371','0'),
	(12535,'1','4461','0'),
	(12535,'2','10371','0'),
	(12517,'1','4461','0'),
	(12517,'2','10371','0'),
	(10843,'1','4461','0'),
	(10843,'2','10371','0'),
	(12051,'1','4461','0'),
	(12051,'2','10371','0'),
	(12524,'1','4461','0'),
	(12524,'2','10371','0'),
	(12531,'1','4461','0'),
	(12531,'2','10371','0'),
	(12536,'1','4461','0'),
	(12536,'2','10371','0'),
	(12928,'1','4461','0'),
	(12928,'2','10371','0'),
	(12543,'1','4461','0'),
	(12543,'2','10371','0'),
	(12564,'1','4461','0'),
	(12564,'2','10371','0'),
	(12924,'1','4461','0'),
	(12924,'2','10371','0'),
	(4560,'1','4461','0'),
	(4560,'2','10371','0'),
	(4583,'1','4461','0'),
	(4583,'2','10371','0'),
	(12004,'1','4461','0'),
	(12004,'2','10371','0'),
	(12841,'1','4461','0'),
	(12841,'2','10371','0'),
	(12049,'1','4461','0'),
	(12049,'2','10371','0'),
	(12525,'1','4461','0'),
	(12525,'2','10371','0'),
	(10702,'1','4461','0'),
	(10702,'2','10371','0'),
	(12053,'1','4461','0'),
	(12053,'2','10371','0'),
	(10840,'1','4461','0'),
	(10840,'2','10371','0'),
	(12526,'1','4461','0'),
	(12526,'2','10371','0'),
	(12921,'1','4461','0'),
	(12921,'2','10371','0'),
	(4602,'1','4461','0'),
	(4602,'2','10371','0'),
	(12882,'1','4461','0'),
	(12882,'2','10371','0'),
	(2522,'1','4461','0'),
	(2522,'2','10371','0'),
	(12845,'1','4461','0'),
	(12845,'2','10371','0'),
	(10373,'1','4461','0'),
	(10373,'2','10371','0'),
	(11878,'1','4461','0'),
	(11878,'2','10371','0'),
	(11972,'1','4461','0'),
	(11972,'2','10371','0'),
	(11997,'1','4461','0'),
	(11997,'2','10371','0');
	
-- Correct flags on numerous trainer gossip_options.
update gossip_menu_option set OptionType = 5, OptionNpcflag = 16 where OptionIcon = 3
and MenuId in (141, 11912, 11913, 12050, 4105, 11932, 12606, 10552, 10817, 11766, 11875, 12344, 12151,
	11879, 12048, 12341, 10838, 12010, 12537, 12714, 10684, 12052, 4555, 4553, 10837, 12918, 12716, 12521,
	12821, 12522, 12746, 12927, 12755, 12920);

-- add gossip_menu_option_trainer records.
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4683, 0, 16),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4824, 0, 130),
	(11972, 0, 149),
	(4827, 0, 149);
	
-- change low level gossip_menu_option_trainer to standard class trainer.
update gossip_menu_option_trainer set TrainerId = 16 where MenuId = 523;
update gossip_menu_option_trainer set TrainerId = 164 where MenuId in (6647,11767);
update gossip_menu_option_trainer set TrainerId = 40 where MenuId = 4647;
update gossip_menu_option_trainer set TrainerId = 33 where MenuId = 6650;

-- change trainer type
update creature_template set trainer_class = 4 where entry = 16279;

-- correct creature_template npcflag for class trainer
update creature_template set npcflag = 49 where entry = 50716;
	
-- switch numerous trainers to correct gossip menus.
-- gnome mage trainers
update creature_template set gossip_menu_id = 4554 where entry = 1228;
update creature_template set gossip_menu_id = 4554 where entry = 5144;
update creature_template set gossip_menu_id = 4554 where entry = 5145;

-- Undead rogue trainer
update creature_template set gossip_menu_id = 4540 where entry = 2130;

-- update Gilneans to standard class trainers.
update creature_template set gossip_menu_Id = 10835 where entry = 38467;
update creature_template set gossip_menu_id = 10834 where entry = 35873;

-- switch trainers who do nothing to either wotlk or generic gossips
-- warrior wotlk gossips
update creature_template set gossip_menu_id = 523 where entry = 3169;
update creature_template set gossip_menu_id = 4548 where entry = 4593;
update creature_template set gossip_menu_id = 4546 where entry = 4594;
update creature_template set gossip_menu_id = 4547 where entry = 4595;
-- warrior generic gossips
update creature_template set gossip_menu_id = 14199 where entry = 17480;
update creature_template set gossip_menu_id = 4548 where entry = 49955;
-- paladin generic gossips
update creature_template set gossip_menu_id = 6647 where entry = 49954;
-- hunter generic gossips
update creature_template set gossip_menu_id = 4647 where entry = 39116;
update creature_template set gossip_menu_id = 4647 where entry = 49946;
update creature_template set gossip_menu_id = 4647 where entry = 39100;
update creature_template set gossip_menu_id = 4647 where entry = 50609;
-- rogue wotlk gossips
update creature_template set gossip_menu_id = 4542 where entry = 4582;
update creature_template set gossip_menu_id = 4540 where entry = 4583;
update creature_template set gossip_menu_id = 4541 where entry = 4584;
update creature_template set gossip_menu_id = 4562 where entry = 5166;
update creature_template set gossip_menu_id = 85 where entry = 2130;
-- rogue generic gossips
update creature_template set gossip_menu_id = 436 where entry = 49909;
update creature_template set gossip_menu_id = 436 where entry = 49949;
update creature_template set gossip_menu_id = 436 where entry = 51998;
update creature_template set gossip_menu_id = 436 where entry = 48615;
-- priest wotlk gossips
update creature_template set gossip_menu_id = 4544 where entry = 4606;
update creature_template set gossip_menu_id = 4545 where entry = 4607;
update creature_template set gossip_menu_id = 4543 where entry = 4608;
-- Sister Almyra gossips
update creature_template set gossip_menu_id = 10838 where entry = 38466;
-- priest generic gossips
update creature_template set gossip_menu_id = 4680 where entry = 49901;
update creature_template set gossip_menu_id = 4680 where entry = 49950;
update creature_template set gossip_menu_id = 4680 where entry = 48614;
-- mage wotlk gossips
update creature_template set gossip_menu_id = 4583 where entry = 4567;
update creature_template set gossip_menu_id = 4537 where entry = 4568;
update creature_template set gossip_menu_id = 4485 where entry = 5497;
update creature_template set gossip_menu_id = 63 where entry = 	23103;
-- mage generic gossips
update creature_template set gossip_menu_id = 14136 where entry = 49952;
update creature_template set gossip_menu_id = 14136 where entry = 50714;
update creature_template set gossip_menu_id = 14136 where entry = 50715;
update creature_template set gossip_menu_id = 14136 where entry = 50717;
update creature_template set gossip_menu_id = 14136 where entry = 50720;
-- warlock wotlk gossips
update creature_template set gossip_menu_id = 2383 where entry = 4563;
update creature_template set gossip_menu_id = 4609 where entry = 4564;
-- warlock generic gossips
update creature_template set gossip_menu_id = 4610 where entry = 49945;
update creature_template set gossip_menu_id = 4610 where entry = 50723;
update creature_template set gossip_menu_id = 4610 where entry = 50729;
-- tauren druid generic gossip
update creature_template set gossip_menu_id = 4605 where entry = 44726;

-- correct low level class trainers who share gossips with standard class trainers.
set @Frang := 3153;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Frang);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 145);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Frang, 5, 0, 0, 'Show gossip when Frang is present'),
	(15, @MenuId, 0, 0, 29, 0, @Frang, 5, 0, 1, 'Show gossip when Frang is not present');
	
set @DeliosSilverBlade := 43010;
set @MenuId := (select gossip_menu_id from creature_template where entry = @DeliosSilverBlade);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 145);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DeliosSilverBlade, 5, 0, 0, 'Show gossip when Delios Silver Blade is present'),
	(15, @MenuId, 0, 0, 29, 0, @DeliosSilverBlade, 5, 0, 1, 'Show gossip when Delios Silver Blade is not present');

set @Kore := 16503;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Kore);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 145);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Kore, 5, 0, 0, 'Show gossip when Kore is present'),
	(15, @MenuId, 0, 0, 29, 0, @Kore, 5, 0, 1, 'Show gossip when Kore is not present');
	
set @SunwalkerHelaku := 37737;
set @MenuId := (select gossip_menu_id from creature_template where entry = @SunwalkerHelaku);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 168);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @SunwalkerHelaku, 5, 0, 0, 'Show gossip when Sunwalker Helaku is present'),
	(15, @MenuId, 0, 0, 29, 0, @SunwalkerHelaku, 5, 0, 1, 'Show gossip when Sunwalker Helaku is not present');
	
set @Aurelon := 16501;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Aurelon);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 168);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Aurelon, 5, 0, 0, 'Show gossip when Aurelon is present'),
	(15, @MenuId, 0, 0, 29, 0, @Aurelon, 5, 0, 1, 'Show gossip when Aurelon is not present');
	
set @Keilnei := 16499;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Keilnei);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 15);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Keilnei, 5, 0, 0, 'Show gossip when Keilnei is present'),
	(15, @MenuId, 0, 0, 29, 0, @Keilnei, 5, 0, 1, 'Show gossip when Keilnei is not present');
	
set @LankaFarshot := 3061;
set @MenuId := (select gossip_menu_id from creature_template where entry = @LankaFarshot);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 15);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @LankaFarshot, 5, 0, 0, 'Show gossip when Lanka Farshot is present'),
	(15, @MenuId, 0, 0, 29, 0, @LankaFarshot, 5, 0, 1, 'Show gossip when Lanka Farshot is not present');
	
set @Rwag := 3155;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Rwag);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 17);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Rwag, 5, 0, 0, 'Show gossip when Rwag is present'),
	(15, @MenuId, 0, 0, 29, 0, @Rwag, 5, 0, 1, 'Show gossip when Rwag is not present');

set @DavidTrias := 2122;
set @MenuId := (select gossip_menu_id from creature_template where entry = @DavidTrias);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 17);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DavidTrias, 5, 0, 0, 'Show gossip when David Trias is present'),
	(15, @MenuId, 0, 0, 29, 0, @DavidTrias, 5, 0, 1, 'Show gossip when David Trias is not present');
	
set @DarkClericDuesten := 2123;
set @MenuId := (select gossip_menu_id from creature_template where entry = @DarkClericDuesten);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 127);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @DarkClericDuesten, 5, 0, 0, 'Show gossip when Dark Cleric Duesten is present'),
	(15, @MenuId, 0, 0, 29, 0, @DarkClericDuesten, 5, 0, 1, 'Show gossip when Dark Cleric Duesten is not present');

set @SeerRavenfeather := 37724;
set @MenuId := (select gossip_menu_id from creature_template where entry = @SeerRavenfeather);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 127);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @SeerRavenfeather, 5, 0, 0, 'Show gossip when Seer Ravenfeather is present'),
	(15, @MenuId, 0, 0, 29, 0, @SeerRavenfeather, 5, 0, 1, 'Show gossip when Seer Ravenfeather is not present');
	
set @MeelaDawnstrider := 3062;
set @MenuId := (select gossip_menu_id from creature_template where entry = @MeelaDawnstrider);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 134);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @MeelaDawnstrider, 5, 0, 0, 'Show gossip when Meela Dawnstrider is present'),
	(15, @MenuId, 0, 0, 29, 0, @MeelaDawnstrider, 5, 0, 1, 'Show gossip when Meela Dawnstrider is not present');

set @JuliaSunstriker := 15279;
set @MenuId := (select gossip_menu_id from creature_template where entry = @JuliaSunstriker);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 44);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @JuliaSunstriker, 5, 0, 0, 'Show gossip when Julia Sunstriker is present'),
	(15, @MenuId, 0, 0, 29, 0, @JuliaSunstriker, 5, 0, 1, 'Show gossip when Julia Sunstriker is not present');

set @Valaatu := 16500;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Valaatu);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 44);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Valaatu, 5, 0, 0, 'Show gossip when Valaatu is present'),
	(15, @MenuId, 0, 0, 29, 0, @Valaatu, 5, 0, 1, 'Show gossip when Valaatu is not present');

-- Update a Horde Portal trainer who shares a gossip with an Alliance Portal trainer.
set @Narinth := 16654;
set @MenuId := (select gossip_menu_id from creature_template where entry = @Narinth);
set @trainingGossipOptionIndex := (select OptionIndex from gossip_menu_option where OptionIcon = 3 and MenuId = @MenuId and OptionType = 5 and OptionNpcFlag = 16);
set @newGossipMenuOptionIndex := (select max(OptionIndex) + 1 from gossip_menu_option where MenuId = @MenuId);
insert into gossip_menu_option (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag, VerifiedBuild)
select @MenuId, @newGossipMenuOptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild from gossip_menu_option where MenuId = @MenuId and OptionIndex = @trainingGossipOptionIndex;
insert into gossip_menu_option_trainer (MenuId, OptionIndex, TrainerId) values (@MenuId, @newGossipMenuOptionIndex, 149);
insert into conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, Comment)
values (15, @MenuId, @newGossipMenuOptionIndex, 0, 29, 0, @Narinth, 5, 0, 0, 'Show gossip when Narinth is present'),
	(15, @MenuId, 0, 0, 29, 0, @Narinth, 5, 0, 1, 'Show gossip when Narinth is not present');


-- Correct Profession Trainers

-- add missing gossip_menu_option for Alchemists.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (4111, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
	(4110, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
	(4117, 0, 3, 'Train me in Alchemy.', 0, 5, 16),
	(8863, 0, 3, 'Train me in Alchemy.', 0, 5, 16);

-- add missing gossip_menu_option for Blacksmiths.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (2741, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
	(2749, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
	(1043, 0, 3, 'Train me in Blacksmithing.', 0, 5, 16),
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
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (4169, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
	(4156, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
	(4171, 0, 3, 'Train me in Enchanting.', 0, 5, 16),
	(4170, 0, 3, 'Train me in Enchanting.', 0, 5, 16);

-- add missing gossip_menu_option for Engineers.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (4136, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(1469, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(1465, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(7512, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(7513, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(7820, 0, 3, 'Train me in Engineering.', 0, 5, 16),
	(10363, 0, 3, 'Train me in Engineering.', 0, 5, 16);

-- add missing gossip_menu_option for Leatherworkers.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (4208, 0, 3, 'Train me in Leatherworking.', 0, 5, 16),
	(4842, 0, 3, 'Train me in Leatherworking.', 0, 5, 16),
	(4244, 0, 3, 'Train me in Leatherworking.', 0, 5, 16);

-- add missing gossip_menu_option for Tailors.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (4356, 0, 3, 'Train me in Tailoring.', 0, 5, 16),
	(10138, 0, 3, 'Train me in Tailoring.', 0, 5, 16);

-- add missing gossip_menu_option for First Aid Trainers.
insert into gossip_menu_option
(MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcflag)
values (8522, 0, 3, 'Train me in First Aide.', 0, 5, 16);

-- correct trainer flags in gossip menu options
update gossip_menu_option set OptionType = 5, OptionNpcflag = 16 where OptionIcon = 3
and MenuId in (4361, 4134, 4129, 4164, 7455, 7817, 9084, 12846, 8460, 6087, 10437, 2021, 51997);

-- add missing gossip_menu_option_trainer for Apprentice Blacksmiths
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (2746, 0, 80),
	(2741, 0, 80);

-- add missing gossip_menu_option_trainer for Apprentice Engineers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4142, 0, 102),
	(7383, 0, 102);

-- add missing gossip_menu_option_trainer for Apprentice Leatherworkers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (7430, 0, 103);

-- add missing gossip_menu_option_trainer for Herbalists.
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (7524, 1, 101),
	(7459, 0, 101),
	(10350, 0, 101);

-- add missing gossip_menu_option_trainer for Miners
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4361, 0, 49),
	(10351, 0, 49);

-- add missing gossip_menu_option_trainer for Skinners
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (7429, 0, 83),
	(10360, 0, 83);
	
-- add missing gossip_menu_option_trainer for Alchemists
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4111, 0, 122),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (2781, 0, 58),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4169, 0, 62),
	(4166, 0, 62),
	(4171, 0, 62),
	(4170, 0, 62),
	(8731, 0, 62),
	(8866, 0, 62),
	(10365, 0, 62),
	(33676, 0, 62);

-- add missing gossip_menu_option_trainer for Engineers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4145, 0, 407),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (9879, 0, 63);

-- add missing gossip_menu_option_trainer for Jewelcrafters
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (12848, 0, 29),
	(8376, 0, 29),
	(8380, 0, 29),
	(8382, 0, 29),
	(9892, 0, 29),
	(9894, 0, 29),
	(9895, 0, 29),
	(9873, 0, 29);

-- add missing gossip_menu_option_trainer for Leatherworkers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4108, 0, 56),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4261, 0, 163),
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
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (4761, 0, 107),
	(12939, 0, 107),
	(8522, 0, 107),
	(8502, 0, 107);

-- add missing gossip_menu_option_trainer for Cooking Trainers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (9986, 0, 136),
	(9985, 0, 136),
	(9987, 0, 136);

-- add missing gossip_menu_option_trainer for Fishing Trainers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (12887, 0, 10),
	(6087, 0, 10),
	(8826, 0, 10);

-- add missing gossip_menu_option_trainer for Archaeology Trainers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (12850, 0, 373);

-- add missing gossip_menu_option_trainer for Riding Trainers
insert into gossip_menu_option_trainer
(MenuId, OptionIndex, TrainerId)
values (8553, 0, 46),
	(8275, 0, 46),
	(4019, 0, 46);

-- correct trainer npcFlags
update creature_template set npcflag = 81 where entry = 30711;
update creature_template set npcflag = 81 where entry = 4578;
update creature_template set npcflag = 81 where entry = 7087;
update creature_template set npcflag = 81 where entry = 16742;
update creature_template set npcflag = 81 where entry = 30716;
update creature_template set npcflag = 81 where entry = 19775;
update creature_template set npcflag = 81 where entry = 26956;
update creature_template set npcflag = 81 where entry = 33587;
update creature_template set npcflag = 81 where entry = 34785;

-- give trainers who do nothing generic gossips
-- herbalism generic gossips
update creature_template set gossip_menu_Id = 7691 where entry = 908;
update creature_template set gossip_menu_Id = 7691 where entry = 1473;
update creature_template set gossip_menu_Id = 7691 where entry = 2114;
update creature_template set gossip_menu_Id = 7691 where entry = 2390;
update creature_template set gossip_menu_Id = 7691 where entry = 2856;
update creature_template set gossip_menu_Id = 7691 where entry = 12025;
update creature_template set gossip_menu_Id = 7691 where entry = 26910;
update creature_template set gossip_menu_Id = 7691 where entry = 26958;
update creature_template set gossip_menu_Id = 7691 where entry = 26974;
update creature_template set gossip_menu_Id = 7691 where entry = 26994;
-- mining wotlk gossips
update creature_template set gossip_menu_Id = 7690 where entry = 4598;
-- mining generic gossips
update creature_template set gossip_menu_Id = 7690 where entry = 3175;
update creature_template set gossip_menu_Id = 7690 where entry = 3555;
update creature_template set gossip_menu_Id = 7690 where entry = 8128;
update creature_template set gossip_menu_Id = 7690 where entry = 26912;
update creature_template set gossip_menu_Id = 7690 where entry = 26962;
update creature_template set gossip_menu_Id = 7690 where entry = 26976;
update creature_template set gossip_menu_Id = 7690 where entry = 28698;
update creature_template set gossip_menu_Id = 7690 where entry = 52170;
update creature_template set gossip_menu_Id = 7690 where entry = 53409;
-- skinning generic gossips
update creature_template set gossip_menu_Id = 7842 where entry = 6295;
update creature_template set gossip_menu_Id = 7842 where entry = 6306;
update creature_template set gossip_menu_Id = 7842 where entry = 7087;
update creature_template set gossip_menu_Id = 7842 where entry = 8144;
update creature_template set gossip_menu_Id = 7842 where entry = 12030;
update creature_template set gossip_menu_Id = 7842 where entry = 27000;
update creature_template set gossip_menu_Id = 7842 where entry = 26913;
update creature_template set gossip_menu_Id = 7842 where entry = 26986;
update creature_template set gossip_menu_Id = 7842 where entry = 28696;
update creature_template set gossip_menu_Id = 7842 where entry = 53437;
-- alchemy generic gossips
update creature_template set gossip_menu_Id = 10359 where entry = 26987;
update creature_template set gossip_menu_Id = 10359 where entry = 26903;
update creature_template set gossip_menu_Id = 10359 where entry = 26951;
update creature_template set gossip_menu_Id = 10359 where entry = 26975;
update creature_template set gossip_menu_Id = 10359 where entry = 27023;
update creature_template set gossip_menu_Id = 10359 where entry = 27029;
update creature_template set gossip_menu_Id = 10359 where entry = 33588;
-- blacksmithing wotlk gossips
update creature_template set gossip_menu_Id = 1022 where entry = 4596;
update creature_template set gossip_menu_Id = 1022 where entry = 26952;
-- blacksmithing generic gossips
update creature_template set gossip_menu_Id = 8521 where entry = 16724;
update creature_template set gossip_menu_Id = 8521 where entry = 26988;
update creature_template set gossip_menu_Id = 8521 where entry = 26904;
update creature_template set gossip_menu_Id = 8521 where entry = 26924;
update creature_template set gossip_menu_Id = 8521 where entry = 27034;
update creature_template set gossip_menu_Id = 8521 where entry = 28694;
update creature_template set gossip_menu_Id = 8521 where entry = 29505;
update creature_template set gossip_menu_Id = 8521 where entry = 29506;
update creature_template set gossip_menu_Id = 8521 where entry = 33591;
-- enchanting generic gossips
update creature_template set gossip_menu_Id = 8866 where entry = 19251;
update creature_template set gossip_menu_Id = 8866 where entry = 26990;
update creature_template set gossip_menu_Id = 8866 where entry = 26906;
update creature_template set gossip_menu_Id = 8866 where entry = 26954;
update creature_template set gossip_menu_Id = 8866 where entry = 26980;
update creature_template set gossip_menu_Id = 8866 where entry = 33583;
-- engineering wotlk gossips
update creature_template set gossip_menu_Id = 4149 where entry = 11031;
-- engineering generic gossips
update creature_template set gossip_menu_Id = 8867 where entry = 10993;
update creature_template set gossip_menu_Id = 8867 where entry = 26991;
update creature_template set gossip_menu_Id = 8867 where entry = 25277;
update creature_template set gossip_menu_Id = 8867 where entry = 26907;
update creature_template set gossip_menu_Id = 8867 where entry = 33586;
-- inscription generic gossips
update creature_template set gossip_menu_Id = 9879 where entry = 30721;
update creature_template set gossip_menu_Id = 9879 where entry = 30710;
update creature_template set gossip_menu_Id = 9879 where entry = 30711;
update creature_template set gossip_menu_Id = 9879 where entry = 30715;
update creature_template set gossip_menu_Id = 9879 where entry = 30716;
-- jewelcrafting generic gossips
update creature_template set gossip_menu_Id = 11861 where entry = 33590;
update creature_template set gossip_menu_Id = 11861 where entry = 52586;
update creature_template set gossip_menu_Id = 11861 where entry = 52587;
-- leatherworking wotlk gossips
update creature_template set gossip_menu_Id = 4210 where entry = 4588;
update creature_template set gossip_menu_Id = 4242 where entry = 16278;
-- leatherworking generic gossips
update creature_template set gossip_menu_Id = 10361 where entry = 7869;
update creature_template set gossip_menu_Id = 10361 where entry = 16728;
update creature_template set gossip_menu_Id = 10361 where entry = 26998;
update creature_template set gossip_menu_Id = 10361 where entry = 26911;
update creature_template set gossip_menu_Id = 10361 where entry = 26996;
update creature_template set gossip_menu_Id = 10361 where entry = 29508;
update creature_template set gossip_menu_Id = 10361 where entry = 29509;
update creature_template set gossip_menu_Id = 10361 where entry = 33581;
-- tailoring wotlk gossips
update creature_template set gossip_menu_Id = 4267 where entry = 3523;
update creature_template set gossip_menu_Id = 4354 where entry = 4576;
-- talioring generic gossips
update creature_template set gossip_menu_Id = 8519 where entry = 4578;
update creature_template set gossip_menu_Id = 8519 where entry = 33580;
-- first aid wotlk gossips
update creature_template set gossip_menu_Id = 5856 where entry = 4591;
-- first aid generic gossips
update creature_template set gossip_menu_Id = 5856 where entry = 26956;
-- cooking trainer generic gossips
update creature_template set gossip_menu_Id = 5853 where entry = 19369;
update creature_template set gossip_menu_Id = 5853 where entry = 33587;
update creature_template set gossip_menu_Id = 5853 where entry = 34785;
update creature_template set gossip_menu_Id = 5853 where entry = 34786;
-- fishing trainer generic gossips
update creature_template set gossip_menu_Id = 5665 where entry = 26993;
update creature_template set gossip_menu_Id = 5665 where entry = 26909;
update creature_template set gossip_menu_Id = 5665 where entry = 26957;

-- remove the repairer mouseover
update creature_template set npcflag = 80 where entry = 16278;
-- correct mount trainer npcflags
update creature_template set npcflag = 81 where entry = 3175;
update creature_template set npcflag = 83 where entry = 16280;
	
-- correct a few innkeepers
update gossip_menu_option set OptionType = 8, OptionNpcflag = 65536 where MenuId = 1297 and OptionIndex = 1;
update gossip_menu_option set OptionType = 3, OptionNpcflag = 128 where MenuId = 1297 and OptionIndex = 2;
insert into gossip_menu_option_action (MenuId, OptionIndex, ActionMenuId, ActionPoiId) values (1297, 0, 1221, 0);
update gossip_menu_option set OptionType = 1, OptionNpcflag = 1 where MenuId = 1293 and OptionIndex = 0;
update gossip_menu_option set OptionType = 8, OptionNpcflag = 65536 where MenuId = 1293 and OptionIndex = 1;
update gossip_menu_option set OptionType = 3, OptionNpcflag = 128 where MenuId = 1293 and OptionIndex = 2;
insert into gossip_menu_option_action (MenuId, OptionIndex, ActionMenuId, ActionPoiId) values (1293, 0, 1221, 0);
update gossip_menu_option set OptionIcon = 0, OptionText = 'What can I do at an inn?', OptionBroadcastTextId = 4308
where MenuId = 1294 and OptionIndex = 2;
update gossip_menu_option set OptionIcon = 0, OptionText = 'What can I do at an inn?', OptionBroadcastTextId = 4308
where MenuId = 7468 and OptionIndex = 2;