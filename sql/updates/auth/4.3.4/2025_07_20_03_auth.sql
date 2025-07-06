ALTER TABLE `account`
    DROP FOREIGN KEY `fk_bnet_acc`,
    DROP CONSTRAINT `uk_bnet_acc`,
    DROP COLUMN `sha_pass_hash`,
    DROP COLUMN `sessionKey`,
    DROP COLUMN `battlenet_account`,
    DROP COLUMN `battlenet_index`,
    ADD `salt` binary(32) NOT NULL AFTER `username`,
    ADD `verifier` binary(32) NOT NULL AFTER `salt`,
    ADD `session_key_auth` binary(40) DEFAULT NULL AFTER `verifier`,
    ADD `session_key_bnet` varbinary(64) DEFAULT NULL AFTER `session_key_auth`,
    ADD `totp_secret` varbinary(128) DEFAULT NULL AFTER `session_key_bnet`,
    ADD `timezone_offset` smallint NOT NULL DEFAULT '0' AFTER `os`;

ALTER TABLE `account_access`
    CHANGE `id` `AccountID` int unsigned NOT NULL,
    CHANGE `gmlevel` `SecurityLevel` tinyint unsigned NOT NULL;

-- Wipe all remaining traces of bnet
DROP TABLE IF EXISTS `battlenet_account_bans`;
DROP TABLE IF EXISTS `battlenet_components`;
DROP TABLE IF EXISTS `battlenet_modules`;
DROP TABLE IF EXISTS `battlenet_accounts`;
