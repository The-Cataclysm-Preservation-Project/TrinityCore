ALTER TABLE `areatrigger_teleport`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `target_orientation`;

ALTER TABLE `broadcast_text`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Flags`;

ALTER TABLE `broadcast_text_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Text1`;

ALTER TABLE `creature`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ScriptName`;

ALTER TABLE `creature_equip_template`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ItemID3`;

ALTER TABLE `creature_questitem`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `ItemId`;

ALTER TABLE `creature_template`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ScriptName`;

ALTER TABLE `creature_template_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Title`;

ALTER TABLE `gameobject`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ScriptName`;

ALTER TABLE `gameobject_questitem`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `ItemId`;
	
ALTER TABLE `gameobject_template`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ScriptName`;
	
ALTER TABLE `gameobject_template_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `castBarCaption`;

ALTER TABLE `gossip_menu`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `TextID`;

ALTER TABLE `gossip_menu_option`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `OptionNpcflag`;
	
ALTER TABLE `lfg_dungeon_template`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `requiredItemLevel`;

ALTER TABLE `npc_text`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Emote7_2`;

ALTER TABLE `npc_vendor`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `PlayerConditionID`;

ALTER TABLE `page_text`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `NextPageID`;

ALTER TABLE `page_text_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Text`;

ALTER TABLE `points_of_interest`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Name`;

ALTER TABLE `points_of_interest_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Name`;

ALTER TABLE `quest_details`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `EmoteDelay4`;

ALTER TABLE `quest_greeting`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `Greeting`;

ALTER TABLE `quest_greeting_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Greeting`;

ALTER TABLE `quest_offer_reward`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `RewardText`;
	
ALTER TABLE `quest_poi`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Flags`;

ALTER TABLE `quest_poi_points`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Y`;

ALTER TABLE `quest_request_items`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NOT NULL DEFAULT 0 AFTER `CompletionText`;

ALTER TABLE `quest_template`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `SoundTurnIn`;

ALTER TABLE `quest_template_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `QuestTurnTargetName`;

ALTER TABLE `spell_target_position`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Orientation`;

ALTER TABLE `trainer`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Greeting`;

ALTER TABLE `trainer_locale`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `Greeting_lang`;

ALTER TABLE `trainer_spell`
	CHANGE COLUMN `VerifiedBuild` `VerifiedBuild` MEDIUMINT NULL DEFAULT NULL AFTER `ReqLevel`;




