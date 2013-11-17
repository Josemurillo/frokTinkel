/*
	Scripts by Joseramon
*/

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum say
{
	SAY_AGGRO = 0,
	SAY_KILL = 1,
	SAY_DIED = 2,
	SAY_RAGE = 3,

};

enum spell
{
	BLADESTORM = 65947,				//Filotormenta
	CURSEOFAGONY = 65814,			//Maldicion de Agonia
	UNSTABLEAFFLICTON = 65812,		//Afliccion inestable
	ICYTOUCH = 66021,				//Toque helado
	ENRAGE = 42705,					//Enfurecer (Se acumula hasta 50)
	
	BERSEK = 26662,					//Rage

};

enum creature
{
	SUMMON = 130190,				//Invocacion
};

#define EMOTE_ERRAGE10        "Suman 10 Acumulaciones, 40 para Bersek"		//Aviso con 10 Enfurecer
#define EMOTE_ERRAGE20        "Suman 20 Acumulaciones, 30 para Bersek"		//Aviso con 20 Enfurecer
#define EMOTE_ERRAGE30        "Suman 30 Acumulaciones, 20 para Bersek"		//Aviso con 30 Enfurecer
#define EMOTE_ERRAGE40        "Suman 40 Acumulaciones, 10 para Bersek"		//Aviso con 40 Enfurecer
#define EMOTE_ERRAGE45        "Suman 45 Acumulaciones, 5 para Bersek"		//Aviso con 45 Enfurecer


// Variables globales para controlar el numero de invocaciones y si alguna invocacion esta viva
uint32 summonCount = 0;
bool summonActive = false;

class esquele : public CreatureScript
{
public:
	esquele(): CreatureScript("boss_esquele") {}
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new esqueleAI (creature);
    }
	
	struct esqueleAI : public ScriptedAI
	{
		esqueleAI(Creature* creature) : ScriptedAI(creature), Summons(me){}
		
		// Inicializacion de las variables de tiempo de ataques
		SummonList Summons;
		uint32 bladestormTemp;
		uint32 curseofagonyTemp;
		uint32 unstableafflicionTemp;
		uint32 icytouchTemp;
		uint32 enrageTemp;
		uint32 enrageCount;
		uint32 summonTemp;
		uint32 mana;

		uint32 compromana;
		
		uint32 bersekTemp;
		
		void Reset()
		{	
			Summons.DespawnAll();
			enrageCount = 0;
			bladestormTemp = 41000;
			curseofagonyTemp = 20000;
			unstableafflicionTemp = 14000;
			icytouchTemp = 6000;
			enrageTemp = 1000;
			summonTemp = 90000;
			summonActive = false;
			summonCount = 0;
			mana = 0;

			compromana = 5000;
			
			bersekTemp = 480000;

			me->RemoveAurasDueToSpell(ENRAGE);
			me->RemoveAurasDueToSpell(BERSEK);
		}
		
		void EnterCombat(Unit* /*who*/)
		{
			mana = me->GetMaxPower(POWER_MANA);
			Talk(SAY_AGGRO);
		}
		
		void JustDied(Unit* /*died*/)
		{
			Talk(SAY_DIED);
			Summons.DespawnAll();
		}

		void JustSummoned(Creature* summoned){
		
			Summons.Summon(summoned);
			// Las invoaciones entran en combate
			summoned->SetInCombatWithZone();
		
		}
		
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
                return;
				
			// Para recuperar mana
			if(compromana <= diff){
				if(me->GetPower(POWER_MANA) < mana*0.2){
					me->SetPower(POWER_MANA,mana);
				}
				compromana = 5000;
			}
			else compromana -= diff;

			// Siempre que una invocacion este activa cada 35 segundos acumulara una carga mas de Enfurecer
			if(summonActive){
				if(enrageTemp <= diff){
					
					DoCast(me,ENRAGE);
					enrageCount ++;

					// Avisos de enfurecer
					if(enrageCount == 10){
						me->MonsterTextEmote(EMOTE_ERRAGE10, 0, true);
					}
					else if(enrageCount == 20){
						me->MonsterTextEmote(EMOTE_ERRAGE20, 0, true);
					}
					else if(enrageCount == 30){
						me->MonsterTextEmote(EMOTE_ERRAGE30, 0, true);
					}
					else if(enrageCount == 40){
						me->MonsterTextEmote(EMOTE_ERRAGE40, 0, true);
					}
					else if(enrageCount == 45){
						me->MonsterTextEmote(EMOTE_ERRAGE45, 0, true);
					}

					enrageTemp = 35000;
					
				}
				else enrageTemp -= diff;
			}

			// Maldicion de Agonia a cualquier objetivo
			if(curseofagonyTemp <= diff){
				
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
					DoCast(target,CURSEOFAGONY);
				curseofagonyTemp = 20000;
			}
			else curseofagonyTemp -= diff;

			// Afliccion Inestable a cualquier objetivo
			if(unstableafflicionTemp <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
					DoCast(target,UNSTABLEAFFLICTON);
				unstableafflicionTemp = 14000;
			}
			else unstableafflicionTemp -= diff;

			//Toque helado solo al tanque
			if(icytouchTemp <= diff){
				if(Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO,0))
					DoCast(target,ICYTOUCH);
				icytouchTemp = 6000;
			}
			else icytouchTemp -= diff;

			// Invocacion de los npc 4 en total
			if(summonTemp <= diff){
				for(uint32 i = 0; i < 4; i++){
					if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0)){
						me->SummonCreature(SUMMON,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),TEMPSUMMON_CORPSE_DESPAWN);
						summonCount ++; // Aumentamos contador de invocaciones activas
						summonActive = true; //Ponemos a verdadero que hay alguna invocacion viva
					}
				}
				summonTemp = 90000;
			}
			else summonTemp -= diff;
				
			// Filotormentas
			if(bladestormTemp <= diff){
				
				if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
					DoCast(target,BLADESTORM);
				bladestormTemp = 41000;
			}
			else bladestormTemp -= diff;
			
			// Si pasa el tiempo para el rage o los contadores de Enfurecer llegan a 50 el boss entra en Rage
			if(bersekTemp <= diff || enrageCount >= 50){
			
				Talk(SAY_RAGE);
				DoCast(me,BERSEK);
				bersekTemp = 480000;
				
			}
			else bersekTemp -= diff;
		
			DoMeleeAttackIfReady();
		
		}
	};
};

class sesquele : public CreatureScript
{
public:
	sesquele(): CreatureScript("summon_esquele") {}
	
	CreatureAI* GetAI(Creature* creature) const
    {
        return new sesqueleAI (creature);
    }
	
	struct sesqueleAI : public ScriptedAI
	{
		sesqueleAI(Creature* creature) : ScriptedAI(creature){}
		
		
		void Reset(){}
		
		void EnterCombat(Unit* /*who*/){}
		
		void JustDied(Unit* /*died*/)
		{
			//Cuando muere reducimos en 1 el contador de invoaciones
			summonCount = summonCount -1;
			
			// Si el numero de invocaciones es 0 
			//es que no hay ninguna invocacion viva y ponemos el comprobador de invocaciones a falso
			if(summonCount<=0){
			
				summonActive = false;
			}
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
                return;
		
			DoMeleeAttackIfReady();
		
		}
	};
};

void AddSC_boss_esquele()
{
	new esquele();
	new sesquele();
}
