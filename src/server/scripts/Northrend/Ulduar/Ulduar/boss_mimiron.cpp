/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "ulduar.h"

Position const ini = {2782.27f,2569.78f,364.31f,3.09f};
Position const summon = {2744.89f,2569.24f,364.31f,3.11f};
Position const home = {2742.53f,2560.99f,364.397f,1.39626f};

uint32 phas = 0;
uint32 tanklive;
uint32 bodylive;
uint32 headlive;
uint32 exclu;


enum Yells
{
    SAY_AGGRO                                   = 0,
    SAY_HARDMODE_ON                             = 1,
    SAY_MKII_ACTIVATE                           = 2,
    SAY_MKII_SLAY                               = 3,
    SAY_MKII_DEATH                              = 4,
    SAY_VX001_ACTIVATE                          = 5,
    SAY_VX001_SLAY                              = 6,
    SAY_VX001_DEATH                             = 7,
    SAY_AERIAL_ACTIVATE                         = 8,
    SAY_AERIAL_SLAY                             = 9,
    SAY_AERIAL_DEATH                            = 10,
    SAY_V07TRON_ACTIVATE                        = 11,
    SAY_V07TRON_SLAY                            = 12,
    SAY_V07TRON_DEATH                           = 13,
    SAY_BERSERK                                 = 14,
    SAY_YS_HELP                                 = 15,
};

enum Spells
{
   SPELL_EMERGENCY_MODE                        = 64582,
   SPELL_SELF_REPAIR                           = 64383,
   // Leviathan MK II
   SPELL_EXPLOSION							   = 63009,
   SPELL_NAPALM_SHELL                          = 63666,
   SPELL_PLASMA_BLAST                          = 62977,
   SPELL_PROXIMITY_MINES                       = 63027,
   SPELL_SHOCK_BLAST                           = 63631,
   // VX 001
   SPELL_HAND_PULSE                            = 64348,
   //SPELL_SPINNING_UP                           = 63414,
   SPELL_RAPID_BURST                           = 63387,
   SPELL_P3WX2_LASER_BARRAGE                   = 63293,
   // Aerial Command Unit
   SPELL_PLASMA_BALL                           = 63689,
   // Additonal spells
   SPELL_MAGNETIC_FIELD                        = 64668,
   SPELL_WATER_SPRAY                           = 64619,
   SPELL_TRIGGER_MISSILE                       = 65347,
};

enum Npc
{
   NPC_ASSAULT_BOT                             = 34057,
   NPC_BOMB_BOT                                = 33836,
   NPC_JUNK_BOT                                = 33855,
   NPC_EMERGENCE_FIRE_BOT                      = 34147,
   NPC_FROST_BOMB                              = 34149,
   
   NPC_TANK = 33432,
   NPC_BODY = 33651,
   NPC_HEAD = 33670,
};

class boss_mimiron : public CreatureScript
{
public:
	boss_mimiron() : CreatureScript("boss_mimiron") { }
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return GetUlduarAI<boss_mimironAI>(creature);
    }
	
	struct boss_mimironAI : public BossAI
    {
        boss_mimironAI(Creature* creature) : BossAI(creature, BOSS_MIMIRON)
        {
        }

		uint32 live;
		uint32 tempProximimines;
		uint32 tempRapidbust;
		uint32 tempMagnetidfield;
		uint32 tempWaterspryy;
		
		void Reset(){

			tempMagnetidfield = 10000;
			tempWaterspryy = 11000;
			tempRapidbust = 15000;
			tempProximimines = 35000;
			live = me->GetMaxHealth()/4;
			tanklive = 0;
			bodylive = 0;
			headlive = 0;
			phas = 1;
			exclu = 0;
			me->SetReactState(REACT_PASSIVE);
			_Reset();
		}
		
		void EnterEvadeMode(){
			boss_mimironAI::Reset();
			_EnterEvadeMode();
			me->GetMotionMaster()->MovePoint(0,home);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

		}
		
		void KilledUnit(Unit* /*victim*/){
		
		}
		
		void JustDied(Unit* /*killer*/){
			me->SetReactState(REACT_PASSIVE);
			_JustDied();
			me->RemoveAllAuras();
			me->AttackStop();
			me->setFaction(35);
			me->DeleteThreatList();
			me->CombatStop(true);
			if(GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL){
				me->SummonGameObject(194958,2784.19f,2579.60f,364.31f,2.99f,0, 0, 0, 0, 90000000);
			}
			if(GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL){
				me->SummonGameObject(194957,2784.19f,2579.60f,364.31f,2.99f,0, 0, 0, 0, 90000000);
			}
			me->DespawnOrUnsummon();
			
		}
		
		void EnterCombat(Unit* /*who*/){
			Talk(SAY_AGGRO);
			me->GetMotionMaster()->MovePoint(0,ini);
			_EnterCombat();
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		}

		void DamageTaken(Unit* /*attacker*/, uint32& damage){
			
			if(HealthBelowPct(3)){
				DoCast(me,SPELL_SELF_REPAIR);
			}

		}
		
		void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			if(tanklive == 0 && phas == 1 && exclu == 0){
				exclu = 1;
				Talk(SAY_MKII_ACTIVATE);
				me->SummonCreature(NPC_TANK,2744.89f,2569.24f,364.31f,3.11f,TEMPSUMMON_CORPSE_DESPAWN);
			}

			if(bodylive == 0 && phas == 2 && exclu == 0){
				exclu = 1;
				Talk(SAY_VX001_ACTIVATE);
				me->SummonCreature(NPC_BODY,2744.89f,2569.24f,364.31f,3.11f,TEMPSUMMON_CORPSE_DESPAWN);
				me->SetHealth(me->GetHealth()-live);
			}

			if(headlive == 0 && phas == 3 && exclu == 0){
				exclu = 1;
				Talk(SAY_AERIAL_ACTIVATE);
				me->SummonCreature(NPC_HEAD,2744.89f,2569.24f,364.31f,3.11f,TEMPSUMMON_CORPSE_DESPAWN);
				me->SetHealth(me->GetHealth()-live);
			}
			
			if(phas == 4 && exclu == 0){
				exclu = 1;
				me->SetHealth(me->GetHealth()-live);
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			}

			if(phas == 4){
				me->SetReactState(REACT_AGGRESSIVE);
				
				if(tempProximimines <= diff){
					if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
						DoCast(target,SPELL_PROXIMITY_MINES);
					}
					tempProximimines = 35000;
				}
				else tempProximimines -= diff;

				if(tempRapidbust <= diff){
					if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
						DoCast(target,SPELL_RAPID_BURST);
					}
					tempRapidbust = 15000;
				}
				else tempRapidbust -= diff;

				if(tempMagnetidfield<= diff){
					if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,1,30.0f,true)){
						DoCast(target,SPELL_MAGNETIC_FIELD);
					}
					tempMagnetidfield = 10000;
				}
				else tempMagnetidfield -= diff;
				DoMeleeAttackIfReady();

				if(tempWaterspryy<= diff){
					if(Unit* target = me->getVictim()){
						DoCast(target,SPELL_WATER_SPRAY);
					}
					tempWaterspryy = 11000;
				}
				else tempWaterspryy -= diff;

				DoMeleeAttackIfReady();
			}
		}
	
	};
};

class boss_leviathan_mkII : public CreatureScript
{
public:
	boss_leviathan_mkII() : CreatureScript("boss_leviathan_mkII") { }
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_leviathan_mkIIAI (creature);
    }
	
	struct boss_leviathan_mkIIAI : ScriptedAI
    {
        boss_leviathan_mkIIAI(Creature* creature) : ScriptedAI(creature)
        {
        }
		
		uint32 tempNapalmshel;
		uint32 tempPlamabast;
		uint32 tempProximimines;
		uint32 tempBlast;

		void Reset(){
			tempNapalmshel = 10000;
			tempPlamabast = 22000;
			tempProximimines = 35000;
			tempBlast = 45000;
		}
		
		void EnterEvadeMode(){
			me->DespawnOrUnsummon();
		}
		
		void KilledUnit(Unit* /*victim*/){
		
		}
		
		void JustDied(Unit* /*killer*/){
			phas = 2;
			tanklive = 1;
			exclu = 0;
			Talk(SAY_MKII_DEATH);
		}
		
		void EnterCombat(Unit* /*who*/){
			
		}

		void DamageTaken(Unit* /*attacker*/, uint32& damage){
			
			if(HealthBelowPct(15)){
				DoCast(me,SPELL_EMERGENCY_MODE);
			}

		}
		
		void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			if(tempNapalmshel<= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_NAPALM_SHELL);
				}
				tempNapalmshel = 10000;
			}
			else tempNapalmshel -= diff;

			if(tempPlamabast<= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 40.0f, true)){
					DoCast(target,SPELL_PLASMA_BLAST);
				}
				tempPlamabast = 22000;
			}
			else tempPlamabast -= diff;

			if(tempProximimines <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_PROXIMITY_MINES);
				}
				tempProximimines = 35000;
			}
			else tempProximimines -= diff;

			if(tempBlast <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_SHOCK_BLAST);
				}
				tempBlast = 45000;
			}
			else tempBlast -= diff;

			DoMeleeAttackIfReady();
		}
	
	};
};

class boss_assault_cannon : public CreatureScript
{
public:
	boss_assault_cannon() : CreatureScript("boss_assault_cannon") { }
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_assault_cannonAI (creature);
    }
	
	struct boss_assault_cannonAI : ScriptedAI
    {
        boss_assault_cannonAI(Creature* creature) : ScriptedAI(creature)
        {
        }
		
		uint32 tempHandpulse;
		uint32 tempRapidbust;
		uint32 tempLasebarrage;

		void Reset(){
			
			tempHandpulse = 10000;
			tempRapidbust = 15000;
			tempLasebarrage = 25000;
		}
		
		void EnterEvadeMode(){
			me->DespawnOrUnsummon();
		}
		
		void KilledUnit(Unit* /*victim*/){
		
		}
		
		void JustDied(Unit* /*killer*/){
			phas = 3;
			bodylive = 1;
			exclu = 0;
			Talk(SAY_VX001_DEATH);
		}
		
		void EnterCombat(Unit* /*who*/){
			
		}

		void DamageTaken(Unit* /*attacker*/, uint32& damage){
			
			if(HealthBelowPct(15)){
				DoCast(me,SPELL_EMERGENCY_MODE);
			}

		}
		
		void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			if(tempHandpulse <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_HAND_PULSE);
				}
				tempHandpulse = 10000;
			}
			else tempHandpulse -= diff;

			if(tempRapidbust <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_RAPID_BURST);
				}
				tempRapidbust = 15000;
			}
			else tempRapidbust -= diff;

			if(tempLasebarrage <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_P3WX2_LASER_BARRAGE);
				}
				tempLasebarrage = 25000;
			}
			else tempLasebarrage -= diff;

			DoMeleeAttackIfReady();
		}
	
	};
};

class boss_aerial_unit : public CreatureScript
{
public:
	boss_aerial_unit() : CreatureScript("boss_aerial_unit") { }
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_aerial_unitAI (creature);
    }
	
	struct boss_aerial_unitAI : ScriptedAI
    {
        boss_aerial_unitAI(Creature* creature) : ScriptedAI(creature)
        {
        }
		
		uint32 tempPlasmaball;

		void Reset(){
			tempPlasmaball = 3000;
		}
		
		void EnterEvadeMode(){
			me->DespawnOrUnsummon();
		}
		
		void KilledUnit(Unit* /*victim*/){
		
		}
		
		void JustDied(Unit* /*killer*/){
			phas = 4;
			headlive = 1;
			exclu = 0;
			Talk(SAY_AERIAL_DEATH);
		}
		
		void EnterCombat(Unit* /*who*/){
			
		}

		void DamageTaken(Unit* /*attacker*/, uint32& damage){
			
			if(HealthBelowPct(15)){
				DoCast(me,SPELL_EMERGENCY_MODE);
			}

		}
		
		void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			if(tempPlasmaball <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_PLASMA_BALL);
				}
				tempPlasmaball = 3000;
			}
			else tempPlasmaball -= diff;

			DoMeleeAttackIfReady();
		}
	
	};
};

class proximity_mine : public CreatureScript
{
public:
	proximity_mine() : CreatureScript("proximity_mine") { }
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new proximity_mineAI (creature);
    }
	
	struct proximity_mineAI : ScriptedAI
    {
        proximity_mineAI(Creature* creature) : ScriptedAI(creature)
        {
        }

		uint32 tempExploit;

		void Reset(){
				
			tempExploit = 2000;
		}

		void UpdateAI(const uint32 diff){

			if (!UpdateVictim())
                return;

			if(tempExploit <= diff){
				DoCast(me,SPELL_EXPLOSION);
				tempExploit = 2000;
				me->DespawnOrUnsummon();
			}
			else tempExploit -= diff;


		}
	};
};

class spell_ulduar_proximity_mines : public SpellScriptLoader
{
   public:
       spell_ulduar_proximity_mines() : SpellScriptLoader("spell_ulduar_proximity_mines") { }

       class spell_ulduar_proximity_minesSpellScript : public SpellScript
       {
           PrepareSpellScript(spell_ulduar_proximity_minesSpellScript)

           void HandleScript(SpellEffIndex effIndex)
           {
               PreventHitDefaultEffect(effIndex);
               for (uint8 i = 0; i < 10; ++i)
                   GetCaster()->CastSpell(GetCaster(), SPELL_TRIGGER_MISSILE, true);
           }

           void Register()
           {
               OnEffectHitTarget += SpellEffectFn(spell_ulduar_proximity_minesSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_ulduar_proximity_minesSpellScript();
       }
};

void AddSC_boss_mimiron()
{
	new boss_mimiron();
	new boss_leviathan_mkII();
	new boss_aerial_unit();
	new boss_assault_cannon();
	new proximity_mine();
    new spell_ulduar_proximity_mines();
}
