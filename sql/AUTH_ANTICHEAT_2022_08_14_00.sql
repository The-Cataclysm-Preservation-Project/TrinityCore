-- Apply to Auth
DELETE FROM `rbac_permissions` WHERE `id`=1434;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (1434, 'Command: anticheat');

DELETE FROM `rbac_linked_permissions` WHERE `id`=196 AND `linkedId`=1434;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (196, 1434);

DELETE FROM `rbac_linked_permissions` WHERE `id`=197 AND `linkedId`=1434;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (197, 1434);

DELETE FROM `rbac_linked_permissions` WHERE `id`=198 AND `linkedId`=1434;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (198, 1434);
