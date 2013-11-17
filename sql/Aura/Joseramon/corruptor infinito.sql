SET @guidn = ;
DELETE FROM creature WHERE id = 32273;
INSERT INTO `creature` VALUES (@guidn, 32273, 595, 2, 1, 0, 0, 2334.97, 1269.46, 132.882, 3.23105, 86400, 0, 0, 1, 0, 0, 0, 0, 0);
DELETE FROM game_event_creature WHERE guid = @guidn;