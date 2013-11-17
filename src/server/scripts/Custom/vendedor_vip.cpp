#include "ScriptPCH.h"
     
class VIP_NPC : public CreatureScript
{
	public:
	VIP_NPC() : CreatureScript("VIP_NPC") { }
     
	bool OnGossipHello(Player * pPlayer, Creature * pCreature)
	{
		if (pPlayer->GetSession()->GetSecurity() >= SEC_VIP)
		{
			pPlayer->ADD_GOSSIP_ITEM(6, "Vendedor Vip", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
		}

		pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
		return true;
		}
           
	bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
    {
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
			case GOSSIP_OPTION_VENDOR:
				player->GetSession()->SendListInventory(m_creature->GetGUID());
				break;
		}
        return true;
    }
};
 
void AddSC_VIP_NPC()
{
    new VIP_NPC();
}