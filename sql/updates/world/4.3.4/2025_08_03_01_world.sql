ALTER TABLE `creature_template_movement`
	ADD COLUMN `GravityInitiallyDisabled` TINYINT(1) NULL DEFAULT NULL AFTER `HoverInitiallyEnabled`;

ALTER TABLE `creature_movement_override`
	ADD COLUMN `GravityInitiallyDisabled` TINYINT(1) NULL DEFAULT NULL AFTER `HoverInitiallyEnabled`;
