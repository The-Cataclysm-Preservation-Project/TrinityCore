-- Adding columns
ALTER TABLE `characters`
	ADD COLUMN `characterFlags` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `playerFlags`,
	ADD COLUMN `characterFlags2` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `characterFlags`;

-- Migrate data
-- Ghost
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00002000 WHERE (`playerFlags` & 0x00000010) != 0;
-- Hide Cloak
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00000800 WHERE (`playerFlags` & 0x00000800) != 0;
-- Hide Helm
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00000400 WHERE (`playerFlags` & 0x00000400) != 0;
-- Game Master
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00020000 WHERE (`playerFlags` & 0x00000008) != 0;
-- Resting
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00000002 WHERE (`playerFlags` & 0x00000020) != 0;
-- PvP Enabled
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00000040 WHERE (`playerFlags` & 0x00000100) != 0;
-- PvP Desired
UPDATE `characters` SET `characterFlags`= `characterFlags` | 0x00010000 WHERE (`playerFlags` & 0x00000200) != 0;
-- No XP Gain
UPDATE `characters` SET `characterFlags2`= `characterFlags2` | 0x00040000 WHERE (`playerFlags` & 0x02000000) != 0;
-- Can Use Void Storage Feature
UPDATE `characters` SET `characterFlags2`= `characterFlags2` | 0x04000000 WHERE (`playerFlags` & 0x20000000) != 0;
-- Low Level Raids enabled
UPDATE `characters` SET `characterFlags2`= `characterFlags2` | 0x20000000 WHERE (`playerFlags` & 0x00010000) != 0;
-- Auto Decline Guild Invites
UPDATE `characters` SET `characterFlags2`= `characterFlags2` | 0x40000000 WHERE (`playerFlags` & 0x08000000) != 0;

-- Conversion done, drop no longer needed column
ALTER TABLE `characters` DROP COLUMN `playerFlags`;
