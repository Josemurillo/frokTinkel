DELETE FROM gossip_menu WHERE entry = 45066;
INSERT INTO gossip_menu VALUES (45066,921062);

DELETE FROM npc_text WHERE ID = 921062;
INSERT INTO `npc_text` VALUES (921062, '¿Que haces por aquí? No tienes una cuenta Vip! Informate en el foro www.wowaura.com', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 1);

DELETE FROM creature_template WHERE entry = 110119;
INSERT INTO `creature_template` VALUES (110119, 0, 0, 0, 0, 0, 27911, 0, 0, 0, 'Vendedor Vip', NULL, NULL, 0, 83, 83, 2, 35, 35, 129, 1, 1.14286, 2, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'VIP_NPC', 1);

DELETE FROM npc_vendor WHERE entry = 110119;
INSERT INTO `npc_vendor` VALUES (110119, 0, 7228, 0, 0, 0);

SET @guidn = ;

DELETE FROM creature WHERE id = 110119;
INSERT INTO `creature` VALUES (@guidn, 110119, 0, 1, 1, 0, 0, -8804.13, 652.155, 96.1989, 4.40408, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+1, 110119, 0, 1, 1, 0, 0, -4952.55, -925.99, 504.263, 5.352, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+2, 110119, 1, 1, 1, 0, 0, 9864.01, 2328.2, 1321.59, 3.88609, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+3, 110119, 530, 1, 1, 0, 0, -4010.28, -11719.9, -151.653, 0.0211995, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+4, 110119, 1, 1, 1, 0, 0, 1664.08, -4431.41, 17.7834, 1.13408, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+5, 110119, 1, 1, 1, 0, 0, -1200.64, 115.883, 134.999, 3.09116, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+6, 110119, 530, 1, 1, 0, 0, 9667.71, -7132.08, 14.3258, 6.21694, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+7, 110119, 530, 1, 1, 0, 0, 9689.11, -7492.89, 15.7782, 1.4912, 300, 0, 0, 13945, 0, 0, 0, 0, 0);
INSERT INTO `creature` VALUES (@guidn+8, 110119, 0, 1, 1, 0, 0, 1639.73, 195.642, -60.7697, 5.47517, 300, 0, 0, 13945, 0, 0, 0, 0, 0);

UPDATE item_template SET BuyPrice = 2000000,duration = 0 WHERE entry = 33184;

DELETE FROM npc_vendor WHERE entry = 110119;
INSERT INTO npc_vendor SELECT 110119,0,entry,5,10800,0 FROM item_template WHERE entry IN (40111,40112,40113,40114,
40115,40116,40117,40118,40119,40120,40121,40122,40123,40124,40125,40126,40127,40128,40129,40130,40131,40132,
40133,40134,40135,40136,40137,40138,40139,40140,40141,40164,40165,40166,40167,40168,40169,40170,40171,40172,
40173,40174,40175,40176,40177,40178,40179,40180,40181,40182,40142,40143,40144,40145,40146,40147,40148,40149,
40150,40151,40152,40153,40154,40155,40156,40157,40158,40159,40160,40161,40162,40163);
INSERT INTO npc_vendor VALUES (110119,0,33184,0,0,0);

