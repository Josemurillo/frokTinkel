UPDATE creature_template SET unit_flags = 32836,mindmg = 1,maxdmg = 1, dmg_multiplier = 1,minrangedmg = 1, maxrangedmg = 1,AIName = "",InhabitType = 4 WHERE entry IN (30245,31750,30249,31751);
UPDATE creature_template SET ScriptName = "npc_lord_nexus" WHERE entry = 30245;
UPDATE creature_template SET ScriptName = "npc_scion_of_eternity" WHERE entry = 30249;
DELETE FROM smart_scripts WHERE entryorguid IN (30245,30249);

UPDATE creature_template SET spell1 = 56091, spell2 = 56092, spell3 = 57090, spell4 = 57143, spell5 = 57108, spell6 = 57092, spell7 = 60534, VehicleId = 220, InhabitType = 5 WHERE entry = 31752;
REPLACE INTO `gameobject` VALUES (361, 193960, 616, 2, 1, 754.255, 1301.72, 266.17, -1.6057, 0, 0, 0, 0, 120, 0, 1);

## Correccion del spam en modo 25
UPDATE creature_template SET difficulty_entry_1 = 31734 WHERE entry = 28859;

UPDATE creature_template SET flags_extra = 1, mechanic_immune_mask = 1071628287, InhabitType = 5 WHERE entry = 31734;

DELETE FROM spell_linked_spell WHERE spell_trigger = 56438;
INSERT INTO spell_linked_spell VALUES (56438,-56397,2,'Tromba Arcana inmune si tienes Sobrecarga Arcana');