DELETE FROM creature_template WHERE entry IN (130189,130190);
INSERT INTO `creature_template` VALUES (130190, 0, 0, 0, 0, 0, 20365, 0, 0, 0, 'Alma danzante', NULL, NULL, 0, 80, 80, 2, 14, 14, 0, 1, 1.14286, 2, 3, 509, 683, 0, 805, 15, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 8, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1071628287, 0, 'summon_esquele', 1);
INSERT INTO `creature_template` VALUES (130189, 0, 0, 0, 0, 0, 7550, 0, 0, 0, 'Esqueletron', 'Puro Hueso', NULL, 0, 83, 83, 2, 14, 14, 0, 1, 1.14286, 10, 3, 509, 683, 0, 805, 35, 2000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 130189, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250000, 300000, '', 0, 3, 1, 1700, 11111, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 30841, 1071628287, 1, 'boss_esquele', 1);

DELETE FROM creature_text WHERE entry = 130189;
INSERT INTO `creature_text` VALUES (130189, 0, 0, 'Iniciamos la cuenta atras para vuestra muerte', 14, 0, 100, 0, 0, 0, 'Esqueletron - Aggro');
INSERT INTO `creature_text` VALUES (130189, 1, 0, '!Ohh.. si...¡ Uno de mi lista', 14, 0, 100, 0, 0, 0, 'Esqueletron - Kill');
INSERT INTO `creature_text` VALUES (130189, 2, 0, 'No puede ser...,esto es ilogico', 14, 0, 100, 0, 0, 0, 'Esqueletron - Dead');
INSERT INTO `creature_text` VALUES (130189, 3, 0, 'Se os acabo el tiempo, todos morireis', 14, 0, 100, 0, 0, 0, 'Esqueletron - Rage');

DELETE FROM creature_equip_template WHERE entry = 30841;
INSERT INTO creature_equip_template VALUES (30841,39769,0,0);

DELETE FROM reference_loot_template WHERE entry = 130189;
INSERT INTO reference_loot_template SELECT 130189,entry,0,1,1,1,1 FROM item_template WHERE entry IN (46312,46051,46038,46021,45931,45929,45286,45866,45703,45522,45535,45609,45507,45263);
DELETE FROM creature_loot_template WHERE entry = 130189;
INSERT INTO creature_loot_template VALUES (130189,49426,100,1,0,2,2);
INSERT INTO creature_loot_template VALUES (130189,37742,100,1,0,3,3);
INSERT INTO creature_loot_template VALUES (130189,1,100,1,0,-130189,2);

SET @guidn = ;
DELETE FROM creature WHERE id = 130189;
INSERT INTO `creature` VALUES (@guidn, 130189, 534, 1, 1, 0, 0, 5258.57, -2095.13, 1277.7, 0.806455, 82800, 0, 0, 1547895, 472638, 0, 0, 0, 0);
