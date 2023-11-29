--
CREATE TABLE `creature_template_gossip` (
	`CreatureID` INT(10) UNSIGNED NOT NULL,
	`MenuID` INT(10) UNSIGNED NOT NULL,
	`VerifiedBuild` INT(11) NOT NULL DEFAULT '0',
	PRIMARY KEY (`CreatureID`, `MenuID`) USING BTREE
)
COLLATE='utf8mb4_unicode_ci'
ENGINE=InnoDB
;

INSERT INTO `creature_template_gossip` SELECT `entry`, `gossip_menu_id`, `VerifiedBuild` FROM `creature_template` WHERE `gossip_menu_id` <> 0;

ALTER TABLE `creature_template` DROP COLUMN `gossip_menu_id`;