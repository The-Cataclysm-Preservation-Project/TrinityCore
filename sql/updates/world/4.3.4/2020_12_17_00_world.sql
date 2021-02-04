--
ALTER TABLE `waypoints`
	ADD COLUMN `velocity` FLOAT NOT NULL DEFAULT '0' AFTER `position_z`;
	