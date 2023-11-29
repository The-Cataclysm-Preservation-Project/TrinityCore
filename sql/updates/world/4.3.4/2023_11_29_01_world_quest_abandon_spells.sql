--
CREATE TABLE `quest_abandon_spells` (
	`QuestID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`SpellID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`Description` TEXT NULL DEFAULT NULL COLLATE 'utf8mb4_unicode_ci',
	PRIMARY KEY (`QuestID`) USING BTREE
)
COLLATE='utf8mb4_unicode_ci'
ENGINE=InnoDB
;
