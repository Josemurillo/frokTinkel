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
#include "ScriptedCreature.h"
#include "ulduar.h"

enum Yells
{
    SAY_AGGRO_1                                 = -1603270,
    SAY_AGGRO_2                                 = -1603271,
    SAY_SPECIAL_1                               = -1603272,
    SAY_SPECIAL_2                               = -1603273,
    SAY_SPECIAL_3                               = -1603274,
    SAY_JUMPDOWN                                = -1603275,
    SAY_SLAY_1                                  = -1603276,
    SAY_SLAY_2                                  = -1603277,
    SAY_BERSERK                                 = -1603278,
    SAY_WIPE                                    = -1603279,
    SAY_DEATH                                   = -1603280,
    SAY_END_NORMAL_1                            = -1603281,
    SAY_END_NORMAL_2                            = -1603282,
    SAY_END_NORMAL_3                            = -1603283,
    SAY_END_HARD_1                              = -1603284,
    SAY_END_HARD_2                              = -1603285,
    SAY_END_HARD_3                              = -1603286,
    SAY_YS_HELP                                 = -1603287,
};

enum spell
{
	SPELL_STORMHAMMER = 62042,
	SPELL_CHAIN_LIGHTNING = 64390,
	SPELL_LIGHTNING_ORB = 62391,
	SPELL_UNBALANCING_STRIKE = 62130,
	SPELL_LIGHT_BOLT_VOLLEY = 36740,
	SPELL_SMASH = 62339,
    SPELL_RUNIC_BARRIER = 62338,
    SPELL_CHARGE_10 = 62613,
    SPELL_CHARGE_25 = 62614,
	SPELL_RUNIC_FORTIFICATION = 62942,
    SPELL_RUNE_DETONATION = 62526,
    SPELL_STOMP_10 = 62411,
    SPELL_STOMP_25 = 62413,

};

enum npcobject
{
	NPC_COLOSSUS = 32872,
	NPC_GIGANT = 32873,
	NPC_LINGT_ORB = 33138,

};

#define EMOTE_MIGHT        "Ancient Rune Giant fortifies nearby allies with runic might!"

Position const home = {2135.0f,-288.93f,438.25f,1.64f};

class boss_thorim : public CreatureScript
{
public:
    boss_thorim() : CreatureScript("boss_thorim") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetUlduarAI<boss_thorimAI>(creature);
    }

    struct boss_thorimAI : public BossAI
    {
        boss_thorimAI(Creature* creature) : BossAI(creature, BOSS_THORIM), Summons(me)
        {
        }
	
		SummonList Summons;
		uint32 phase;
		uint32 tempStormhammer;
		uint32 tempSummon;
		uint32 tempOrb;
		uint32 tempSummonLBV;
		uint32 tempChainlin;
		uint32 tempStrike;

        void Reset()
        {
			phase = 1;
			Summons.DespawnAll();
			tempSummon = 10000;
			tempSummonLBV = 25000;
			tempChainlin = 13000;
			tempStrike = 6000;
			me->SetReactState(REACT_PASSIVE);
			tempStormhammer = 8000;
            _Reset();
        }

        void EnterEvadeMode()
        {
			me->GetMotionMaster()->MovePoint(0,home);
            DoScriptText(SAY_WIPE, me);
			me->SetReactState(REACT_PASSIVE);
            _EnterEvadeMode();
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void JustDied(Unit* /*killer*/)
        {
			Summons.DespawnAll();
			DoScriptText(SAY_DEATH, me);
			me->SetReactState(REACT_PASSIVE);
			_JustDied();
			me->RemoveAllAuras();
			me->AttackStop();
			me->setFaction(35);
			me->DeleteThreatList();
			me->CombatStop(true);
			if(GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL){
				me->SummonGameObject(194315,2162.41f,-249.7f,419.35f,3.37f,0, 0, 0, 0, 90000000);
			}
			if(GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL){
				me->SummonGameObject(194313,2162.41f,-249.7f,419.35f,3.37f,0, 0, 0, 0, 90000000);
			}
			me->DespawnOrUnsummon();
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2), me);
            _EnterCombat();
        }

		void DamageTaken(Unit* /*attacker*/, uint32& damage){
			
			if(HealthBelowPct(50)){
				me->SetReactState(REACT_AGGRESSIVE);
				phase = 2;
			}

		}
		
		void JustSummoned(Creature* summoned){
		
			Summons.Summon(summoned);
		
		}
		
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
    //SPELLS TODO:
			if(phase == 1){
				if(tempSummon <= diff){
					if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
						uint32 npc = urand(0,1);
						if(npc == 0){
							me->SummonCreature(NPC_COLOSSUS,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),TEMPSUMMON_CORPSE_DESPAWN);
						}
						else{
							me->SummonCreature(NPC_GIGANT,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),TEMPSUMMON_CORPSE_DESPAWN);
						}
					}
					tempSummon = 45000;
				}
				else tempSummon -= diff;

			}
			
			if(phase == 2){
				if(tempStrike<= diff){
					if(Unit* target = me->getVictim()){
						DoCast(target,SPELL_UNBALANCING_STRIKE);
					}
					 tempStrike = 6000;
				}
				else tempStrike -= diff;
			}
			
			if(tempStormhammer <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
					DoCast(target,SPELL_STORMHAMMER);
				}
				tempStormhammer = 8000;
			}
			else tempStormhammer -= diff;
			
			if(tempSummonLBV <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
					me->SummonCreature(NPC_LINGT_ORB,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),TEMPSUMMON_CORPSE_DESPAWN);
				}
				tempSummonLBV = 25000;
			}
			else tempSummonLBV -= diff;
			
			if(tempChainlin <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
					DoCast(target,SPELL_CHAIN_LIGHTNING);
				}
				tempChainlin = 13000;
			}
			else tempChainlin -= diff;
    //
            DoMeleeAttackIfReady();
        }
    };

};

class mob_runic_colossus : public CreatureScript
{
public:
    mob_runic_colossus() : CreatureScript("mob_runic_colossus") { }

    struct mob_runic_colossusAI : public ScriptedAI
    {
        mob_runic_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
            
        uint32 BarrierTimer;
        uint32 SmashTimer;
        uint32 ChargeTimer;

        void Reset()
        {
            BarrierTimer = urand(12000, 15000);
            SmashTimer = urand (15000, 18000);
            ChargeTimer = urand (20000, 24000);
            
        }
        
        void UpdateAI(const uint32 uiDiff)
        {
        
            if (!UpdateVictim())
                return;
                
                
            if (BarrierTimer <= uiDiff)
            {
                me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
                DoCast(me, SPELL_RUNIC_BARRIER);
                BarrierTimer = urand(35000, 45000);
            }
            else BarrierTimer -= uiDiff;
            
            if (SmashTimer <= uiDiff)
            {
                DoCast(me, SPELL_SMASH);
                SmashTimer = urand (15000, 18000);
            }
            else SmashTimer -= uiDiff;
            
            if (ChargeTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 8, true))
                    DoCast(pTarget, RAID_MODE(SPELL_CHARGE_10, SPELL_CHARGE_25));
                ChargeTimer = 20000;
            }
            else ChargeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit *victim)
        {
            me->DespawnOrUnsummon();
        }
		
		 void EnterEvadeMode()
        {
			me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_runic_colossusAI(pCreature);
    }
};

class npc_thorim_lightning_orb : public CreatureScript
{
public:
    npc_thorim_lightning_orb() : CreatureScript("npc_thorim_lightning_orb") { }

    struct npc_thorim_lightning_orbAI : public ScriptedAI
    {
        npc_thorim_lightning_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            
        }
		
		uint32 tempDesp;
		uint32 tempLBV;
		
        void Reset()
        {
           tempDesp = 20000;
		   tempLBV = 2000;
        }
		
		void UpdateAI(const uint32 diff){
		
			if(tempDesp <= diff){
				me->DespawnOrUnsummon();
			}
			else tempDesp -= diff;
			
			if(tempLBV <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
					DoCast(target,SPELL_LIGHT_BOLT_VOLLEY);
				}
				tempLBV = 2000;
			}
			else tempLBV -= diff;
		
		}
	};
	
	CreatureAI *GetAI(Creature *creature) const
	{
		return new npc_thorim_lightning_orbAI(creature);
	}
        
};

class mob_rune_giant : public CreatureScript
{
public:
    mob_rune_giant() : CreatureScript("mob_rune_giant") { }

    struct mob_rune_giantAI : public ScriptedAI
    {
        mob_rune_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
            
        uint32 StompTimer;
        uint32 DetonationTimer;

        void Reset()
        {
            StompTimer = urand(10000, 12000);
            DetonationTimer = 25000;
        }
        
        void EnterCombat(Unit* pWho)
        {
            me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
        }
        
        void UpdateAI(const uint32 uiDiff)
        {    
            if (!UpdateVictim())
                return;
                
            if (StompTimer <= uiDiff)
            {
                DoCast(me, RAID_MODE(SPELL_STOMP_10, SPELL_STOMP_25));
                StompTimer = urand(10000, 12000);
            }
            else StompTimer -= uiDiff;
            
            if (DetonationTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                    DoCast(pTarget, SPELL_RUNE_DETONATION);
                DetonationTimer = urand(10000, 12000);
            }
            else DetonationTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit *victim)
        {
            me->DespawnOrUnsummon();
        }
		
		 void EnterEvadeMode()
        {
			me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_rune_giantAI(pCreature);
    }
};


void AddSC_boss_thorim()
{
    new boss_thorim();
	new npc_thorim_lightning_orb();
	new mob_runic_colossus();
	new mob_rune_giant();
}