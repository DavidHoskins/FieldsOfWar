#include "DeclareWar.h"

//Constructor.
DeclareWar::DeclareWar()
{
	m_preconditions.push_back(AINodes::notAtWar);
	m_preconditions.push_back(AINodes::suitableTargetForWar);
	m_effect = AINodes::isAtWar;
	m_actionPreconditions.push_back(AINodes::buildArmy);
}

//Main action, when this is called the nation will find a suitable target and go to war with this target.
void DeclareWar::action(Nation* currentNation, HexHandler* HexArray)
{
	Nation* temp = suitableTarget(currentNation, HexArray);
	temp->m_relations.setRelationshipByType(temp, currentNation, DiploInteractions::war, true);
	for (int i = 0; i < currentNation->m_ownedUnits.size(); i++)
	{
		*currentNation->m_ownedUnits[i]->m_AI.m_preconditionsState[UnitAINode::IsAtWar] = true;
	}
}

//Finds a suitable nation (one that borders the current nation) and returns it for use.
Nation* DeclareWar::suitableTarget(Nation* currentNation, HexHandler* HexArray)
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
						if (temp != nullptr)
						{
							return temp;
						}
					}
				}
			}
		}
	}
	return nullptr;
}

//Returns the nation that owns this hex.
Nation* DeclareWar::HexesNation(HexHandler* HexArray, Hex* HexToTest)
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

