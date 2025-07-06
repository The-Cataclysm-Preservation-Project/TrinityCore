--

START TRANSACTION; -- we're messing with the accounts table here, let's play it safe

DROP TABLE IF EXISTS `secret_digest`;
CREATE TABLE `secret_digest` (
  `id` int(10) unsigned not null,
  `digest` varchar(100) not null,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB;

COMMIT; -- safety gloves off
