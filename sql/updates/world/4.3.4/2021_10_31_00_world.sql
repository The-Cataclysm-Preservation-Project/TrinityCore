ALTER TABLE `quest_greeting_locale`
	DROP PRIMARY KEY,
	ADD PRIMARY KEY (`ID`, `Type`, `locale`) USING BTREE;