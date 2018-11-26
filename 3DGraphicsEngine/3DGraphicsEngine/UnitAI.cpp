#include "UnitAI.h"

#include "AINodes.h"
#include "UnitStack.h"
#include "HexHandler.h"
#include "NationHandler.h"
#include "Nation.h"

//Constructor.
UnitAI::UnitAI()
{
	//Sorts preconditions states to a default state. 
	for (int i = 0; i < UnitAINode::finalCheck; i++)
	{
		m_preconditionsState.push_back(new bool(false));
	}
	m_AILogic.push_back(new AttackEnemy(m_preconditionsState, m_AILogic));
	m_AILogic.push_back(new AttackProvinceNode(m_preconditionsState, m_AILogic));
	m_AILogic.push_back(new AttackTerritory(m_preconditionsState, m_AILogic));
	m_AILogic.push_back(new AvoidEnemy(m_preconditionsState, m_AILogic));
	m_AILogic.push_back(new HuntEnemyUnits(m_preconditionsState, m_AILogic));

	for (int i = 0; i < m_AILogic.size(); i++)
	{
		m_AILogic[i]->startUp(m_preconditionsState, m_AILogic);
	}
}

//Destructor.
UnitAI::~UnitAI()
{
	int temp = static_cast<int>(m_currentAIPath.size());
	for (int i = 0; i < m_currentAIPath.size(); i++)
	{
		m_currentAIPath.pop_back();
	}
	for (int i = 0; i < m_preconditionsState.size(); i++)
	{
		delete m_preconditionsState[i];
	}

	for (int i = 0; i < m_AILogic.size(); i++)
	{
		delete m_AILogic[i];
		m_AILogic[i] = nullptr;
	}
}

//Main update for the unitAI.
void UnitAI::update(UnitStack* currentUnit, HexHandler* HexArray)
{
	if (currentUnit->m_nation != nullptr)
	{
		checkPreconditions(currentUnit, HexArray);
		if (m_preconditionsState[UnitAINode::IsAtWar])
		{
			m_currentAIPath = m_AILogic[0]->getBestPath();
			if (m_currentAIPath.size() != 0)
			{
				m_currentAIPath[0]->action(currentUnit->m_nation, HexArray, currentUnit);
			}
		}
		else
		{
			//currentAIPath = AILogic[1]->getBestPath();
		}
	}
}

//Checks the state of the preconditions.
void UnitAI::checkPreconditions(UnitStack* currentUnit, HexHandler* HexArray)
{
	checkInRange(currentUnit, HexArray);
	checkMoreUnitsInStack(currentUnit, HexArray);
}

//Checks the unit is still in range of an enemy unit.
void UnitAI::checkInRange(UnitStack* currentUnit, HexHandler* HexArray)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (HexArray->m_nationsInfo->m_nationsInGame[i] != currentUnit->m_nation)
		{
			for (int j = 0; j < HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits.size(); j++)
			{
				if (currentUnit->m_latestHex == HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex)
				{
					*m_preconditionsState[UnitAINode::UnitInRange] = true;
					*m_preconditionsState[UnitAINode::UnitNotInRange] = false;
					return;
				}
			}
		}
	}
	*m_preconditionsState[UnitAINode::UnitInRange] = false;
	*m_preconditionsState[UnitAINode::UnitNotInRange] = true;
}

//Checks whether the number of units in the stack is greater than any of the enemy unit stacks.
void UnitAI::checkMoreUnitsInStack(UnitStack* currentUnit, HexHandler* HexArray)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (currentUnit->m_nation->m_relations.getRelationshipState(HexArray->m_nationsInfo->m_nationsInGame[i], DiploInteractions::interactionsTypes::war))
		{
			if (HexArray->m_nationsInfo->m_nationsInGame[i] != currentUnit->m_nation)
			{
				if (HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits.size() == 0)
				{
					*m_preconditionsState[UnitAINode::MoreUnitsInStack] = false;
					*m_preconditionsState[UnitAINode::LessUnitsInStack] = true;
					return;
				}
				for (int j = 0; j < HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits.size(); j++)
				{
					if (HexArray->m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_landUnits[Hex::infantry] >= currentUnit->m_landUnits[Hex::infantry])
					{
						*m_preconditionsState[UnitAINode::MoreUnitsInStack] = false;
						*m_preconditionsState[UnitAINode::LessUnitsInStack] = true;
						return;
					}
				}
			}
		}
	}
	*m_preconditionsState[UnitAINode::MoreUnitsInStack] = true;
	*m_preconditionsState[UnitAINode::LessUnitsInStack] = false;
}