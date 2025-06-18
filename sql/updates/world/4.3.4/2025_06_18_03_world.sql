ALTER TABLE `quest_template_addon`
    ADD `QuestMaxScalingLevel` INT UNSIGNED NOT NULL DEFAULT 0,
    ADD `RewardMoneyDifficulty` INT UNSIGNED NOT NULL DEFAULT 0,
    ADD `RewardMoneyMultiplier` FLOAT NOT NULL DEFAULT 1,
    ADD `RequiredMoney` INT NOT NULL DEFAULT 0;

-- Convert negative reward money values to RequiredMoney
UPDATE `quest_template_addon` AS qta
INNER JOIN `quest_template` AS qt
    ON qt.`ID` = qta.`ID`
SET qta.`RequiredMoney` = qt.`RewardMoney` WHERE qt.`RewardMoney` < 0;
