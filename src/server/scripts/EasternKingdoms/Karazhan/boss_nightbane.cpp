/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Nightbane
SD%Complete: 80
SDComment: SDComment: Timers may incorrect
SDCategory: Karazhan
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "karazhan.h"

//phase 1
#define SPELL_BELLOWING_ROAR        39427
#define SPELL_CHARRED_EARTH         30129
#define SPELL_DISTRACTING_ASH       30130
#define SPELL_SMOLDERING_BREATH     30210
#define SPELL_TAIL_SWEEP            25653
//phase 2
#define SPELL_RAIN_OF_BONES         37098
#define SPELL_SMOKING_BLAST         37057
#define SPELL_FIREBALL_BARRAGE      30282
#define SPELL_SEARING_CINDERS       30127
#define SPELL_SUMMON_SKELETON       30170

#define EMOTE_SUMMON                "An ancient being awakens in the distance..."
#define YELL_AGGRO                  "What fools! I shall bring a quick end to your suffering!"
#define YELL_FLY_PHASE              "Miserable vermin. I shall exterminate you from the air!"
#define YELL_LAND_PHASE_1           "Enough! I shall land and crush you myself!"
#define YELL_LAND_PHASE_2           "Insects! Let me show you my strength up close!"
#define EMOTE_BREATH                "takes a deep breath."

float IntroWay[2][3] =
{

    {-11187.28f, -1890.23f, 125.00f},
	{-11130.68f ,-1883.95f ,91.47f},
};

class boss_nightbane : public CreatureScript
{
public:
    boss_nightbane() : CreatureScript("boss_nightbane") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_nightbaneAI (creature);
    }

    struct boss_nightbaneAI : public ScriptedAI
    {
        boss_nightbaneAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Phase;

        uint32 BellowingRoarTimer;
        uint32 CharredEarthTimer;
        uint32 DistractingAshTimer;
        uint32 SmolderingBreathTimer;
        uint32 TailSweepTimer;
        uint32 RainofBonesTimer;
        uint32 SmokingBlastTimer;
        uint32 FireballBarrageTimer;
        uint32 SearingCindersTimer;
		uint32 SkeletonTimer;

        uint32 FlyCount;
        uint32 FlyTimer;

        bool Flying;

        void Reset()
        {
            BellowingRoarTimer = 30000;
            CharredEarthTimer = 15000;
            DistractingAshTimer = 2000;
            SmolderingBreathTimer = 10000;
            TailSweepTimer = 12000;
            RainofBonesTimer = 10000;
            SmokingBlastTimer = 20000;
            FireballBarrageTimer = 13000;
            SearingCindersTimer = 14000;
			SkeletonTimer = 15000;

            Phase =1;

            me->SetSpeed(MOVE_RUN, 2.0f);
            me->SetDisableGravity(true);
            me->SetWalk(false);
            me->setActive(true);

            Flying = false;
            FlyCount = 0;
		}
		
        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(TYPE_NIGHTBANE, IN_PROGRESS);
            me->MonsterYell(YELL_AGGRO, LANG_UNIVERSAL, 0);
        }


        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(TYPE_NIGHTBANE, DONE);
        }

        void JustSummoned(Creature* summoned)
        {
            summoned->AI()->AttackStart(me->getVictim());
        }
		
		void DamageTaken(Unit* /*attacker*/, uint32& damage){
		
			if (HealthBelowPct(80)){
				me->MonsterYell(YELL_LAND_PHASE_2, LANG_UNIVERSAL, 0);
				TakeOff();
			}
			
			if (HealthBelowPct(60)){
				
				me->MonsterYell(YELL_LAND_PHASE_1, LANG_UNIVERSAL, 0);
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MovePoint(3, IntroWay[1][0], IntroWay[1][1], IntroWay[1][2]);
                Flying = false;
				Phase = 1;
				me->SetSpeed(MOVE_RUN, 2.0f);
				me->SetDisableGravity(true);
				me->SetWalk(false);
				me->setActive(true);
				
			}
			
			if (HealthBelowPct(40)){
				me->MonsterYell(YELL_LAND_PHASE_2, LANG_UNIVERSAL, 0);
				TakeOff();
			}
			
			if (HealthBelowPct(20)){
				
				me->MonsterYell(YELL_LAND_PHASE_1, LANG_UNIVERSAL, 0);
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MovePoint(3, IntroWay[1][0], IntroWay[1][1], IntroWay[1][2]);
                Flying = false;
				Phase = 1;
				me->SetSpeed(MOVE_RUN, 2.0f);
				me->SetDisableGravity(true);
				me->SetWalk(false);
				me->setActive(true);
				
			}
		
		}

        void TakeOff()
        {
            me->MonsterYell(YELL_FLY_PHASE, LANG_UNIVERSAL, 0);

            me->InterruptSpell(CURRENT_GENERIC_SPELL);
            me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            me->SetDisableGravity(true);
            (*me).GetMotionMaster()->Clear(false);
            (*me).GetMotionMaster()->MovePoint(0, IntroWay[0][0], IntroWay[0][1], IntroWay[0][2]);

            Flying = true;
			Phase = 2;

            RainofBonesTimer = 5000; //timer wrong (maybe)
         }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
				
            //  Phase 1 "GROUND FIGHT"
            if (Phase == 1)
            {
                if (BellowingRoarTimer <= diff)
                {
                    if(Unit* target = me->getVictim()){
						DoCast(target, SPELL_BELLOWING_ROAR);
					}
                    BellowingRoarTimer = urand(30000, 40000);
                } else BellowingRoarTimer -= diff;

                if (SmolderingBreathTimer <= diff)
                {
                    if(Unit* target = me->getVictim()){
						DoCast(target, SPELL_SMOLDERING_BREATH);
					}
                    SmolderingBreathTimer = 20000;
                } else SmolderingBreathTimer -= diff;

                if (CharredEarthTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_CHARRED_EARTH);
                    CharredEarthTimer = 20000;
                } else CharredEarthTimer -= diff;

                if (TailSweepTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        if (!me->HasInArc(M_PI, target))
                            DoCast(target, SPELL_TAIL_SWEEP);
                    TailSweepTimer = 15000;
                } else TailSweepTimer -= diff;

                if (SearingCindersTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_SEARING_CINDERS);
                    SearingCindersTimer = 10000;
                } else SearingCindersTimer -= diff;


                DoMeleeAttackIfReady();
            }

            //Phase 2 "FLYING FIGHT"
            if (Phase == 2)
            {
                if(SkeletonTimer <= diff){
                    for (uint8 i = 0; i <= 3; ++i)
                    {
                        if(Unit* target = me->getVictim()){
							DoCast(target, SPELL_SUMMON_SKELETON);
						}
                    }
                
					SkeletonTimer = 15000;
                }
				else SkeletonTimer -= diff;

                if (RainofBonesTimer < diff)
                {
					if(Unit* target = me->getVictim()){
						DoCast(target, SPELL_RAIN_OF_BONES);
					}
                    RainofBonesTimer = 20000;
                }else RainofBonesTimer -= diff;

                if (DistractingAshTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_DISTRACTING_ASH);
                    DistractingAshTimer = 2000; //timer wrong
                } else DistractingAshTimer -= diff;

                if (SmokingBlastTimer <= diff)
                    {	
					   if(Unit* target = me->getVictim()){
							DoCast(target, SPELL_SMOKING_BLAST);
					   }
                       SmokingBlastTimer = 1500; //timer wrong
                    } else SmokingBlastTimer -= diff;
                

                if (FireballBarrageTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0))
                        DoCast(target, SPELL_FIREBALL_BARRAGE);
                    FireballBarrageTimer = 20000;
                } else FireballBarrageTimer -= diff;
            }
        }
    };

};

void AddSC_boss_nightbane()
{
    new boss_nightbane();
}
