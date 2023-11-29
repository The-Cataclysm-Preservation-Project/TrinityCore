--
ALTER TABLE `quest_template_addon`
	ADD COLUMN `AbandonSpellID` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `TimeAllowed`;
	
-- Example
UPDATE `quest_template_addon` SET `AbandonSpellID` = 74113 WHERE `ID` = 24958; -- Volcanoth!