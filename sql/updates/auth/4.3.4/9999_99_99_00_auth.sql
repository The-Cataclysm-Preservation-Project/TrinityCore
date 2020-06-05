DROP TABLE IF EXISTS `account_warden_flags`;
CREATE TABLE `account_warden_flags`  (
  `account_id` int(10) UNSIGNED NOT NULL,
  `timestamp` int(10) UNSIGNED NOT NULL,
  `check_id` int(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`account_id`),
  FOREIGN KEY (`account_id`) REFERENCES `account` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
);
