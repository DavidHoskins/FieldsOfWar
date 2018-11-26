#include "PeaceOut.h"

#include "HexHandler.h"
#include "NationHandler.h"
#include "Nation.h"
#include "RelationshipHandler.h"


//Constructor.
PeaceOut::PeaceOut()
{
	m_effect = AINodes::ExpandRealm;
	m_actionPreconditions.push_back(AINodes::isAtWar);
	m_actionPreconditions.push_back(AINodes::wantsToEndWar);
}

//Main action for this node.
void PeaceOut::action(Nation* currentNation, HexHandler* HexArray)
{
	for (int i = 0; i < currentNation->m_relations.m_diploInteractionsState.size(); i++)
	{
		if (currentNation->m_relations.m_diploInteractionsState[i]->getInteractionState(DiploInteractions::war))
		{
			for (int j = 0; j < HexArray->m_nationsInfo->m_nationsInGame.size(); j++)
			{
				if (currentNation->m_relations.m_diploInteractionsState[i]->getNationID() == HexArray->m_nationsInfo->m_nationsInGame[j]->getCountryTag())
				{
					currentNation->m_relations.setRelationshipByType(HexArray->m_nationsInfo->m_nationsInGame[j], currentNation, DiploInteractions::war, false);
				}
			}
		}
	}

	for (int i = 0; i < currentNation->m_ownedUnits.size(); i++)
	{
		*currentNation->m_ownedUnits[i]->m_AI.m_preconditionsState[UnitAINode::IsAtWar] = false;
	}
}