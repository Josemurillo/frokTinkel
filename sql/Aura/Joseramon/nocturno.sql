UPDATE gameobject SET position_x = -11164.4, position_y = -1906.2, position_z = 91.47 WHERE id = 194092;
UPDATE creature_template SET unit_flags = 0, ScriptName = "boss_nightbane" WHERE entry = 17225;
DELETE FROM creature WHERE id = 17225;
UPDATE event_scripts SET X =-11130.68 ,Y= -1883.95 ,z=91.47 ,o=3.46  WHERE id = 10591; 
UPDATE gameobject_template SET TYPE = 10, data0 = 0,data1 = 0,data2 = 0, data6 =0, data5 = 1, data10 = 31116 WHERE entry = 194092;
UPDATE gameobject SET spawntimesecs = 604800 WHERE id = 194092;
