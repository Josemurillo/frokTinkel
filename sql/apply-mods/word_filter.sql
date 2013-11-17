-- ----------------------------
-- Table structure for `letter_analogs`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `letter_analogs` (
  `letter` varchar(1) NOT NULL DEFAULT '',
  `analogs` varchar(128) DEFAULT '',
  PRIMARY KEY (`letter`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 MAX_ROWS=26;

-- ----------------------------
-- Records of letter_analogs
-- ----------------------------
DELETE FROM `letter_analogs` WHERE `letter` >= 'a' AND `letter` <= 'z';
INSERT INTO `letter_analogs` VALUES 
('a', '?@'),
('b', '???'),
('c', '?'),
('d', '?'),
('e', '??'),
('f', '?'),
('g', '?'),
('h', '??'),
('i', '??'),
('j', '??'),
('k', '?'),
('l', '?'),
('m', '?'),
('n', '?'),
('o', '?0'),
('p', '??'),
('q', '?'),
('r', '??'),
('s', '?$'),
('t', '?'),
('u', '??'),
('v', '?'),
('w', '???'),
('x', '?'),
('y', '??'),
('z', '?3');

-- ----------------------------
-- Table structure for `bad_word`
-- ----------------------------
DROP TABLE IF EXISTS `bad_word`;
CREATE TABLE `bad_word` (
  `bad_word` varchar(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`bad_word`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Bad word';

-- ----------------------------
-- Records of bad_word
-- ----------------------------

DELETE FROM `trinity_string` WHERE `entry` BETWEEN 20000 AND 20001;
INSERT INTO `trinity_string` VALUES (20000, 'Tu mensaje va a ser revisado por posible spam. \'%s\'.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '????????? ?? ???? ??????????, ?????? ??? ? ??? ??????? ??????????? ????? \'%s\'.');
INSERT INTO `trinity_string` VALUES (20001, 'Tu mail va a ser revisado por spam. \'%s', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '????? ?? ???? ??????????, ?????? ??? ? ??? ??????? ??????????? ????? \'%s\'.');

DELETE FROM `command` WHERE `name` IN ('wordfilter badword add', 'wordfilter badword remove', 'wordfilter badword list', 'wordfilter mod');
INSERT INTO `command` VALUES 
('reload bad_word', 3, 'Syntax: .reload bad_word\nReload bad_word table.'),
('reload letter_analogs', 3, 'Syntax: .reload letter_analogs\nReload letter_analogs table.'),
('wordfilter badword add', 3, 'Syntax: .wordfilter badword add #word'),
('wordfilter badword remove', 3, 'Syntax: .wordfilter badword remove #word'),
('wordfilter badword list', 3, 'Syntax: .wordfilter badword list'),
('wordfilter mod', 3, 'Syntax: .wordfilter mod on/off');
