#include "AttackProvinceNode.h"

#include "HexHandler.h"
#include "NationHandler.h"
#include "Nation.h"
#include "UnitStack.h"


//Constructor.
AttackProvinceNode::AttackProvinceNode(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes)
{
	m_effect = UnitAINode::provinceAttacked;
	m_actionPreconditions.push_back(UnitAINode::UnitNotInRange);
}

//Main action of node.
void AttackProvinceNode::action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (HexArray->m_nationsInfo->m_nationsInGame[i] != currentNation)
		{
			if (HexArray->m_nationsInfo->m_nationsInGame[i]->m_relations.getRelationshipState(currentNation, DiploInteractions::war))
			{
				if (HexArray->m_nationsInfo->m_nationsInGame[i]->ownsHex(currentUnit->m_AITargetHex))
				{
					return;
				}
			}
		}
	}
	Hex* temp = findFirstLegalTile(currentNation, HexArray);
	if (temp != nullptr)
	{
		currentUnit->m_targetHex = temp;
		currentUnit->m_AITargetHex = temp;
	}
}

//Finds the first tile with the required specifications.
Hex* AttackProvinceNode::findFirstLegalTile(Nation* currentNation, HexHandler* HexArray)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (HexArray->m_nationsInfo->m_nationsInGame[i] != currentNation)
		{
			if (HexArray->m_nationsInfo->m_nationsInGame[i]->m_relations.getRelationshipState(currentNation, DiploInteractions::war))
			{
				return findLegalTile(HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedTiles);
			}
		}
	}
	return nullptr;
}

//Finds a hex with the required specifications.
Hex* AttackProvinceNode::findLegalTile(std::vector<Hex *>& tilesToCheck)
{
	for (int j = 0; j < tilesToCheck.size(); j++)
	{
		return tilesToCheck[j];
	}

	return nullptr;
}
