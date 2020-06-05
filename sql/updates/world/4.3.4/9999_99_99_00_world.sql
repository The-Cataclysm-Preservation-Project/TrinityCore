DROP TABLE IF EXISTS warden_keys;
CREATE TABLE `warden_keys`  (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `platform` varchar(5) NOT NULL,
  `seed` text NOT NULL,
  `clientKey` text NOT NULL,
  `serverKey` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

INSERT INTO `warden_keys` (`platform`, `seed`, `clientKey`, `serverKey`) VALUES
("Win", "a390327515cec0bccb05618a2515d5a4", "6a1dd39730f319b0db64acd61ee96582", "128552d30cbbef058689a0e95c47cf52"),
("Win", "429059729f025d5f58725d39dbced1c5", "88066191bddbed717f7af58791528f08", "4f853408d8b6ce60e564cda6f089acbd"),
("Win", "189ebdcb9fde1a653cc0b27e5a4a46c5", "8c865d8c6180be9998313e3b8ea8af91", "80b24dceedbd6cb655a7c46905ceb034"),
("Win", "526e3538c2b2278cdfc019b320bb5471", "cdb5354a058de1f9c3d3a7f84aed91b9", "bb35ff3d5b341240415a62fae539bbef"),
("Win", "d0bfc32958ab623c168bbbaa93f2bce2", "35a68ad73d8aaf624698faf41f0fa473", "6397b6f72b544da284f8af0c8792f458"),
("Win", "bb18331d142164476cd835bab9b3623e", "075d383002d210b1dc41be75c07badab", "c13f5edc9cecb99fceecbe1e980f220e"),
("Win", "26f48d7896a294024a49235c3be40303", "dd0b3168a9b8753f1306bf855b5b1435", "cd51e950500d1252514cc2a06ef4e41e"),
("Win", "88eab2f08256db13862f5afa9ac8b3ae", "55d3fc60073a5765da10012e0cae0f44", "caf8b97b15958cda46c3ae1212747674"),
("Win", "2587f842eef8bc57a78ee6e002e36edb", "79412cab73fe8684e551e72a9bf47c38", "83d2a6f45d21c523c8f36060b06a10d7"),
("Win", "4f1dfb8d9a501e75c138bb8f42d3c71b", "330aaf0959829d398382f1f6fd50e1b9", "a3497d405cee3eb0dd9417346e35238b"),
("Win", "ff4af1e0eda051b903c520ec9b4fe1f5", "b53d4d93db413c9badef1a80facb5703", "d26f2495eeb3bba3a8a51a4760b20fb3"),
("Win", "093f3def01d311ca72cd534a97a5bf14", "b89c7279082e455deb34b2ab1200a068", "e0c3a667f6931d9252f9536f0068e02a"),
("Win", "ec6617dd1ea1af97c8d90a92888d84d9", "e327e12075c29c5e9a238688b3592c7d", "183dccbc9c0a91105959337f85549951"),
("Win", "d29b3b29768657967341a7b8bb8dbd92", "285598f344d7d9a45b883b789017973f", "06fa5715be206a92ca26380ec4dae26e"),
("Win", "ffba1fac21e7f840637dc9c89aa0b427", "261d5878a4983ab95dad71c7ca09adb5", "ae03e30e9bf22fd6459769917072fd37"),
("Win", "27cadad8809ad40b83ce345b03ba75b8", "113a1d9a77a187d471c6488cf3d1365a", "b7020e133e9282c31ed560b22be54880"),
("Win", "8b8f354fc20334e807f51e361b53f658", "b250eecb81563956dc8d9c8f0099977a", "7af5f1d27d7735c7c457d6e9fe47a06f"),
("Win", "4a70366f01cebe9833c4597970945f5b", "2c62b68a07bc1db4294b34c4ca633f3d", "f6032d87150b0293cc9176df6fe7e394"),
("Win", "51812b5bddf1f513bcc305591bf0f44a", "e556e838c247be7a7966c2aa02ce7c29", "ef05975f488e9f7836eae445274ed355"),
("Win", "04fab25fdfdeca051510621a8db38c30", "31f4664ee82efe11fcaf01cf34811098", "23eca60ac69e1caeae6d85ac25669be4");

DROP TABLE IF EXISTS warden_checks;
CREATE TABLE `warden_checks`  (
  `id` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `type` tinyint(3) UNSIGNED NULL DEFAULT NULL,
  `flags` int(10) NOT NULL DEFAULT 0,
  `enabled` tinyint(1) NOT NULL DEFAULT 0,
  `data0` varchar(255) DEFAULT NULL,
  `data1` varchar(255) DEFAULT NULL,
  `address` int(10) UNSIGNED NULL DEFAULT NULL,
  `length` tinyint(3) UNSIGNED NULL DEFAULT NULL,
  `result` varchar(40) DEFAULT NULL,
  `comment` text DEFAULT NULL,
  `VerifiedBuild` SMALLINT(5) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `warden_checks`(`VerifiedBuild`, `type`, `flags`, `enabled`, `data0`, `data1`, `address`, `length`, `result`, `comment`) VALUES
-- All scanned offsets by retail as of 4.3.4.15595
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004F6A04, 5,  '740E83F810', 'Universal language hack'), -- Universal language hack (foce language to universal by nulling check against boss message event types)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x001C8B4D, 7,  '75186848010000',           'Follow hack'), -- Follow any type of unit hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0003DDCB, 10, '76A5F30F118ED4010000',     'Camera zoom hack'), -- Camera zoom hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00332882, 12, '0F85D80100008D55DC528D45', 'No collision hack'), -- No collision hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x001B43B0, 10, '8B81B807000025000080',     'Fly hack'), -- Fly hack (MOVEMENTFLAG_CAN_FLY)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00205F89, 9,  'F74138000200207534',       'Fly hack'), -- Fly hack (MOVEMENTFLAG_DISABLE_COLLISION|MOVEMENTFLAG_DISABLE_GRAVITY)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00089770, 9,  '558BEC538B5D108B43',       'Network sniffer'), -- NetClient::WCMessageReady
(15595, 0, 0x00001000, 1, NULL, NULL, 0x007B0670, 8,  '2F549A4177BE7F3F',         'Gravity hack'), -- Gravity value
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00626A70, 12, '816638FFFF9FFF8B4638F6C1', 'Forever swimming (swimming fly hack?)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004DDDE5, 5,  '7734FF2485',               'FrameXML signature hack'), -- FrameXML signature hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004D20E1, 6,  '8BEC833D9480',             'Lua unlocking'), -- Lua unlock (LuaApi_G::isSecure)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE47, 8,  'F7C3000100007413',         'Terrain collision hack'), -- Terrain collision hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0016DDC0, 9,  '558BEC8B450C83EC08',       'Unknown (but still a hack).'), -- Unknown (CGPlayer_C prologue)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x006263E0, 5,  '753EF6463C',               'Disable jumping'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0020932D, 11, '017E748BCEE829A5FFFF8B',   'Fall damage hack'), -- Fall damage hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0017B272, 7,  'E8C9E42F008BF0',           'Speed hack (MoveIdle)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004F9C10, 12, '558BEC81EC180E00006A0AE8', 'Lua SendChatMessage detour'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x001DA011, 4,  '741C8B06',                 'No swim hack'), -- Walk in water
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0061FB16, 5,  '7465D94150',               'Fly hack'), -- Fly hack (disable falling)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x000041D6, 5,  'E855174000',               'Custom loader'), -- (Not calling GlueScriptEventsInitialize)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x000041E0, 6,  'E81BCEFFFFE8',             'Unknown. Useless, the function called is nulled out by preprocessor defines.'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x002018F1, 7,  '0F2F4408087206',           'Wallclimbing hack'), -- Wallclimbing hack (slope check)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004C5674, 4,  '7C4705C0',                 'Anti AFK hack'), -- Nullop CGGameUI::UpdatePlayerAFK (no AFK hack)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004FA305, 10, '52578B7DF8E831E5FEFF',     'Language hack'), -- Language hack (force language to universal)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004D20F2, 7,  '746583F91A7760',           'Lua unlocking'), -- Lua unlock (Nullop a lua_tainted check in CGGameUI::CanPerformAction)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x002052DA, 7,  '7426A900001000',           'Water walking hack'), -- Water walk hack (Force water facets)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE17, 8,  'F7C3F0000300741D',         'WMO collision hack'), -- WMO collision hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x000041C3, 5,  '6A00E8868F',               'Skip calling FrameScript_Initialize'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x000544B3, 4,  '7E0B8BCE',                 'Unit level hack'), -- Prevent showing boss mob icon (Script_UnitLevel)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004A1B34, 7,  'E8E75AC1FF8B0D',           'Unknown. Related to graphics. EndScene?'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0017B24A, 4,  '50518BCB',                 'Speed hack'), -- Speed hack (CMovement::MoveUnits)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004F9D57, 7,  '75166838020000',           'Language hack'), -- Speak all languages hack (avoid language skill checks)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0000E4D0, 6,  'E85B9FFFFF89',             'WinMain hook'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004F6A12, 9,  '750A8B4D10C7410400',       'Language hack'), -- Universal language hack (similar to 0x004F6A04)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CDE7, 8,  'F7C30000F0007428',         'Collision hack'), -- Collision related
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00321EB8, 5,  '742AF64034',               'Walk in water hack'), -- No swim hack (World::QueryObjectLiquid)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE65, 5,  '74218B4D18',               'Collision hack'), -- M2collision
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00907544, 8,  '0400000001000000',         'Unknown'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00087FA0, 9,  '558BEC83EC18568BF1',       'Network sniffer'), -- Incoming packet hook (NetClient::HandleData)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00089590, 9,  '558BEC83EC0853568B',       'Network sniffer'), -- Outgoing packet hook (NetClient::Send(CDataStore*, CONNECTION_ID)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0000119A, 6,  'CCCCCCCCCCCC',             'Unknown (in bytes between functions)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0017AB20, 10, '558BEC81ECA00000008B',     'Speed hack'), -- Speed hack (CMovement_C::ExecuteMovement(CMovement_C*, int, int))'
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0014EAE1, 1,  'FF',                       'Network sniffer'), -- Incoming packet hook (recv syscall)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004ED34E, 5,  '723B8B4D18',               'Language hack'), -- Understand all languages hack (CGChat::TranslateMessage, skip language skill value check)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x001DA00F, 4,  '85DB741C',                 'Walk in water hack'), -- No swim hack (CGUnit_C::UpdateSwimming)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE62, 5,  'F6C30F7421',               'Collision hack'), -- M2collision
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CDED, 5,  '74288B4D18',               'Collision hack'), -- Collision related
(15595, 0, 0x00001000, 1, NULL, NULL, 0x008185C4, 8,  'D893FEC0488C11C1',         'Speed hack'), -- Jump and swimming speed
(15595, 0, 0x00001000, 1, NULL, NULL, 0x006263A0, 7,  'A9000000107407',           'Always hover hack'), -- Always hover hack
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00703FD0, 9,  '558BEC5153568B7508',       'Network sniffer'), -- Grunt::ClientLink::PacketLogon (likely session key interception)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x002052D5, 7,  'A9000000047426',           'Water walk hack (facet query flags)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE1D, 5,  '741D8B4D18',               'Collision hack'), -- Collision related
(15595, 0, 0x00001000, 1, NULL, NULL, 0x00709FB0, 9,  '558BEC8B450C8B4D08',       'Checksum hook'), -- Checksum hook (doesn\'t work for Cata due to ASLR)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x007B6750, 8,  'BB8D243FD4D0313E',         'Wallclimbing hack'), -- Wallclimbing (Maximum climb angle override)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0043C24E, 5,  '8B4D10890D',               'Lua unlocking '), -- Lua unlock for /run (FrameScript_ExecuteBuffer)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004A240A, 7,  '83C40885C074EF',           'Unknown (Related to graphics, EndScene?)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0033285A, 4,  '750B5F5E',                 'Collision hack'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0014DFE0, 11, '8BC133C989088948048948',   'Network sniffer'), -- Outgoing packet hook (NETEVENTQUEUE)
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0061FB0F, 9,  'F74138000010017465',       'Fly hack (turn off fall direction math)'),
(15595, 0, 0x00001000, 1, NULL, NULL, 0x0038CE4D, 5,  '74138B4D18',               'Collision hack'), -- Terrrain collision
(15595, 0, 0x00001000, 1, NULL, NULL, 0x004F6560, 12, '558BECB8A0420000E8A380B1', 'ChatFrame::AddMessage hook'),
(15595, 9, 0x00001000, 1, NULL, NULL, NULL, NULL, NULL, 'Timing check'),
-- Scans below this point are entirely made up.
(0, 1, 0x00001000, 1, 'KERNEL32.DLL',         NULL, NULL, NULL, NULL, 'Sanity check'),
(0, 1, 0x00001002, 1, 'WPESPY.DLL',           NULL, NULL, NULL, NULL, 'WPE Pro'),
(0, 1, 0x00001002, 1, 'LPESPY.DLL',           NULL, NULL, NULL, NULL, 'LPE Spy'),
(0, 1, 0x00001002, 1, 'OHACK.DLL',            NULL, NULL, NULL, NULL, 'OHack'),
(0, 1, 0x00001002, 1, 'EWTDLL.DLL',           NULL, NULL, NULL, NULL, 'Easy WoW Toolbox'),
(0, 1, 0x00001002, 1, 'MOLTENTELEPORTER.DLL', NULL, NULL, NULL, NULL, 'Molten Teleporter'),
(0, 1, 0x00001002, 1, 'RPE.DLL',              NULL, NULL, NULL, NULL, 'rEdox Packet Editor'),
(0, 1, 0x00001002, 1, 'TNXYZ2.DLL',           NULL, NULL, NULL, NULL, 'tnxyz'),
-- MPQ Scans (disabled, causes crashes, also locale-specific for most files)
(15595, 7, 0x00002004, 1, 'DBFilesClient\\Item.db2', NULL, NULL, NULL, '4706FF83D9B611644A87DE79C244B414612EF4F2', 'MPQ Modification (enUS)'),
-- Custom scans from here on out.
(0, 0, 0x00001000, 1, NULL, NULL, 0x00918C40, 8,  '8E9770420000E040',           'Wow Admin Panel'), -- Slow fall - Terminal velocity
(0, 0, 0x00001000, 1, NULL, NULL, 0x00054489, 5,  '742A578BCE',                 'Wow Admin Panel'), -- Alwats show unit level
(0, 0, 0x00001000, 1, NULL, NULL, 0x004FA0EE, 6,  'FFD084C07452',               'Wow Admin Panel'), -- Talk while dead
(0, 0, 0x00001000, 1, NULL, NULL, 0x0002B3B9, 1,  '89',                         'Wow Admin Panel'), -- Lua unlocking
(0, 0, 0x00001000, 1, NULL, NULL, 0x0002B3A4, 1,  '89',                         'Wow Admin Panel'), -- Lua unlocking
(0, 0, 0x00001000, 1, NULL, NULL, 0x00050AD0, 13, '558BEC8B4D0883EC085633F656', 'Molten Teleporter'), --  GetUnitFromName
(0, 0, 0x00001000, 1, NULL, NULL, 0x004DDB50, 13, '558BEC81ECBC000000535633DB', 'Molten Teleporter'), -- CGGameUI::Initialize
(0, 0, 0x00001000, 1, NULL, NULL, 0x004E0DC0, 10, '558BEC83EC1C568BF1E8',       'Molten Teleporter'), -- CGWorldFrame::PerformDefaultAction
(0, 0, 0x00001000, 1, NULL, NULL, 0x00439E19, 3,  '00751B',                     'Lua unlocking'),
(0, 0, 0x00001000, 1, NULL, NULL, 0x00439F1C, 4,  '85FF7523',                   'Lua unlocking'),
(0, 2, 0x00001000, 1, NULL, NULL, 0x0047BF2B, 13, 'CCCCCCCCCC558BEC6AFF68F839', 'Easy Wow Toolbox'); -- Warden loader hook
