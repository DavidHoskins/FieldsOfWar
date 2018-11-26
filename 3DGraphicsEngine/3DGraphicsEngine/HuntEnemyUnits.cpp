#include "HuntEnemyUnits.h"

#include "UnitStack.h"
#include "HexHandler.h"
#include "NationHandler.h"
#include "Nation.h"

HuntEnemyUnits::HuntEnemyUnits(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes)
{
	m_preconditions.push_back(UnitAINode::MoreUnitsInStack);
	m_effect = UnitAINode::UnitAttacked;
}

//Main action to be enacted if the node is called.
void HuntEnemyUnits::action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (HexArray->m_nationsInfo->m_nationsInGame[i] != currentNation)
		{
			for (int j = 0; j < HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits.size(); j++)
			{
				if (HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_landUnits[Hex::infantry] <= currentUnit->m_landUnits[Hex::infantry])
				{
					if (currentUnit->m_AITargetHex != HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex)
					{
						currentUnit->m_targetHex = HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex;
						currentUnit->m_AITargetHex = HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex;
						return;
					}
					else
					{
						return;
					}
				}
			}
		}
	}
}
