-- Alliance Trainers Gossip Corrections
DELETE FROM `creature_trainer` WHERE `CreatureId` IN (37115, 37121, 42331, 42323, 42366, 43006, 43455);
insert into `creature_trainer` values 
('43455', '32', '11831', '0'),
('43006', '44', '11810', '0'),
('42331', '44', '11620', '0'),
('37121', '44', '10878', '0'),
('37115', '134', '10879', '0'),
('42323', '127', '11617', '0'),
('42366', '17', '11621', '0');

DELETE FROM `gossip_menu_option` WHERE  `MenuId` IN ( 11620, 10878, 10879, 11617, 11621, 11810, 11831);

Insert into `gossip_menu_option` values
('11617','0','3', 'I seek more training in the priestly ways.','3266','5','16','15600'),
('11617','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11617','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('11621','0','3', 'I would like training.','3266','5','16','15600'),
('11621','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11621','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('11831','0','3', 'I am interested in warlock training.','3266','5','16','15600'),
('11831','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11831','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('10879','0','3', 'Teach me the ways of the spirits.','3266','5','16','15600'),
('10879','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('10879','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('10878','0','3', 'I am interested in mage training.','3266','5','16','15600'),
('10878','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('10878','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('11620','0','3', 'I am interested in mage training.','3266','5','16','15600'),
('11620','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11620','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('11810','0','3', 'I am interested in mage training.','3266','5','16','15600'),
('11810','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11810','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600');

-- Horde Trainers Gossip Corrections
DELETE FROM `creature_trainer` WHERE `CreatureId` IN (15279, 15284, 15280, 39214, 42618, 15285, 38244, 15513, 15283);
insert into `creature_trainer` values 
('15279', '44', '14136', '0'),
('15284', '127', '14139', '0'),
('15280', '168', '14137', '0'),
('42618', '32', '11645', '0'),
('15285', '17', '14140', '0'),
('38244', '17', '10985', '0'),
('39214', '15', '11185', '0'),
('15513', '15', '14141', '0'),
('15283', '32', '14138', '0');

DELETE FROM `gossip_menu_option` WHERE  `MenuId` IN (11185, 14136, 14139, 14137, 11645, 14140, 10985, 14141, 14138);

Insert into `gossip_menu_option` values
('11185','0','3', 'I seek training in the ways of the Hunter.','3266','5','16','15600'),
('11185','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11185','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('14139','0','3', 'I seek more training in the priestly ways.','3266','5','16','15600'),
('14141','0','3', 'I seek training in the ways of the Hunter.','3266','5','16','15600'),
('14137','0','3', 'I require paladin training.','3266','5','16','15600'),
('14136','0','3', 'I am interested in mage training.','3266','5','16','15600'),
('14138','0','3', 'I am interested in warlock training.','3266','5','16','15600'),
('11645','0','3', 'I am interested in warlock training.','3266','5','16','15600'),
('10985','0','3', 'I would like training.','3266','5','16','15600'),
('14140','0','3', 'I would like training.','3266','5','16','15600'),
('14139','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('10985','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14140','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14137','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14136','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14138','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('11645','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14141','1','0','I wish to unlearn my talents.','62295','16','16','15600'),
('14139','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('14137','2','0','I wish to know about Dual Talent Specialization.','33762','20','1','15600'),
('14136','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('14138','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('10985','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('14140','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('14141','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600'),
('11645','2','0','Learn about Dual Talent Specialization.','33762','20','1','15600');
