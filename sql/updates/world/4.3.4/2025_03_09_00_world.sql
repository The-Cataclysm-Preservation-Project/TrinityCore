-- Add new movement settings
ALTER TABLE `creature_template_movement`   
	ADD COLUMN `HoverInitiallyEnabled` TINYINT UNSIGNED NULL AFTER `CreatureId`;

UPDATE `creature_template_movement` SET `HoverInitiallyEnabled`= 1 WHERE `Ground` = 2;

ALTER TABLE `creature_movement_override`   
	ADD COLUMN `HoverInitiallyEnabled` TINYINT UNSIGNED NULL AFTER `SpawnId`;

UPDATE `creature_movement_override` SET `HoverInitiallyEnabled`= 1 WHERE `Ground` = 2;

-- Remove deprecated CREATURE_FLAG_EXTRA_NO_MOVE_FLAGS_UPDATE
UPDATE `creature_template` SET `flags_extra`= `flags_extra` &~ 0x200;

-- Conversion done. Drop old columns
ALTER TABLE `creature_movement_override`   
	DROP COLUMN `Ground`, 
	DROP COLUMN `Swim`, 
	DROP COLUMN `Flight`, 
	DROP COLUMN `Rooted`;

ALTER TABLE `creature_template_movement`   
	DROP COLUMN `Ground`, 
	DROP COLUMN `Swim`, 
	DROP COLUMN `Flight`, 
	DROP COLUMN `Rooted`;
