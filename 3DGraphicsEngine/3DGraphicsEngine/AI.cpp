#include "AI.h"

#include "Nation.h"
#include "HexHandler.h"

#include "AggressiveStance.h"
#include "BuildUnits.h"
#include "DeclareWar.h"
#include "PeaceOut.h"

//Constructor.
AI::AI()
{
	//Sorts preconditions states to a default state. 
	for (int i = 0; i < AINodes::finalCheck; i++)
	{
		m_preconditionsState.push_back(new bool(false));
	}

	m_AILogic.push_back(new AggressiveStance());
	m_AILogic.push_back(new BuildUnits());
	m_AILogic.push_back(new DeclareWar());
	m_AILogic.push_back(new PeaceOut());
	for (int i = 0; i < m_AILogic.size(); i++)
	{
		m_AILogic[i]->startUp(m_preconditionsState, m_AILogic);
	}
}

//Destructor.
AI::~AI()
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

//Main update for the AI.
void AI::update(Nation* currentNation, HexHandler* HexArray)
{
	checkPreconditions(currentNation, HexArray);
	m_currentAIPath = m_AILogic[0]->getBestPath();
	if (m_currentAIPath.size() != 0)
	{
		m_currentAIPath[0]->action(currentNation, HexArray);
	}
}



//Checks and updates preconditions.
void AI::checkPreconditions(Nation* currentNation, HexHandler* HexArray)
{
	checkAtWar(currentNation);
	checkHasEnoughForTroops(currentNation);
	checkSuitableTargetForWar(currentNation, HexArray);
	checkForPeaceOut(currentNation);
}

//Checks whether nation is at war.
void AI::checkAtWar(Nation* currentNation)
{
	for (int i = 0; i < currentNation->m_relations.m_diploInteractionsState.size(); i++)
	{
		if (currentNation->m_relations.m_diploInteractionsState[i]->getInteractionState(DiploInteractions::war))
		{
			*m_preconditionsState[AINodes::isAtWar] = true;
			*m_preconditionsState[AINodes::notAtWar] = false;
			return;
		}
	}
	*m_preconditionsState[AINodes::isAtWar] = false;
	*m_preconditionsState[AINodes::notAtWar] = true;
}

//Checks whether this nation can afford  more units.
void AI::checkHasEnoughForTroops(Nation* currentNation)
{
	if ((currentNation->getIncome() - currentNation->getExpenses()) > 20.0f)
	{
		*m_preconditionsState[AINodes::hasMoneyForTroops] = true;
		*m_preconditionsState[AINodes::buildArmy] = false;
	}
	else
	{
		*m_preconditionsState[AINodes::hasMoneyForTroops] = false;
		*m_preconditionsState[AINodes::buildArmy] = true;
	}
}

//Checks whether their is a target nation that is available for war.
void AI::checkSuitableTargetForWar(Nation* currentNation, HexHandler* HexArray)
{
	if (suitableTarget(currentNation,HexArray) != nullptr)
	{
		*m_preconditionsState[AINodes::suitableTargetForWar] = true;
	}
	else
	{
		*m_preconditionsState[AINodes::suitableTargetForWar] = false;
	}
}

//Finds a suitable nation (one that borders the current nation) and returns it for use.
Nation* AI::suitableTarget(Nation* currentNation, HexHandler* HexArray)
{
	for (int i = 0; i < currentNation->m_ownedTiles.size(); i++)
	{
		Hex* currentPoint = currentNation->m_ownedTiles[i];
		sf::Vector2f currentPosition(currentPoint->getPosition());
		for (int j = 0; j < HexArray->getMapWidth(); j++)
		{
			for (int y = 0; y < HexArray->getMapHeight(); y++)
			{
				Hex* hexToTest = HexArray->getHexAtPosition(i, j);
				sf::Vector2f positionToTest = hexToTest->getPosition();

				if (positionToTest.y >= (currentPosition.y - (currentPoint->getRadius() * 2.2)) && positionToTest.y <= (currentPosition.y + (currentPoint->getRadius() * 2.2)))
				{
					if (positionToTest.x >= (currentPosition.x - (currentPoint->getRadius() * 2.2)) && positionToTest.x <= (currentPosition.x + (currentPoint->getRadius() * 2.2)))
					{
						Nation* temp = HexesNation(HexArray, hexToTest);
						return temp;
					}
				}
			}
		}
	}
	return nullptr;
}

//Returns the nation that owns this hex.
Nation* AI::HexesNation(HexHandler* HexArray, Hex* HexToTest)
{
	for (int i = 0; i < HexArray->m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (HexArray->m_nationsInfo->m_nationsInGame[i]->ownsHex(HexToTest))
		{
			return HexArray->m_nationsInfo->m_nationsInGame[i];
		}
	}
	return nullptr;
}

//Checks whether you've gained or lost 5 tiles since being at war.
void AI::checkForPeaceOut(Nation* currentNation)
{
	if (*m_preconditionsState[AINodes::isAtWar])
	{
		if (m_numTiles > (currentNation->m_ownedTiles.size() + 4) || m_numTiles < (currentNation->m_ownedTiles.size() - 4))
		{
			*m_preconditionsState[AINodes::wantsToEndWar] = true;
		}
		else
		{
			*m_preconditionsState[AINodes::wantsToEndWar] = false;
		}
	}
	else
	{
		m_numTiles = static_cast<int>(currentNation->m_ownedTiles.size());
	}
}

