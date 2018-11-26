#include "Nation.h"

#include <math.h>
#include <time.h>

#include "Hex.h"
#include "UnitStack.h"

//Constructor with map size for early memory reservation.
Nation::Nation(const int mapWidth, const int mapHeight)
{
	m_ownedTiles.reserve((mapWidth * mapHeight));
	m_ownedUnits.reserve(1000);
	m_money = 300;
	sf::Uint8 r = static_cast<sf::Uint8>(rand() % 255);
	sf::Uint8 g = static_cast<sf::Uint8>(rand() % 255);
	sf::Uint8 b = static_cast<sf::Uint8>(rand() % 255);

	m_nationColor = new sf::Color(r,g,b,255);
}

//Destructor.
Nation::~Nation()
{
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		m_ownedTiles[i] = nullptr;
	}
	delete m_nationColor;
}

//Main update for nation info.
void Nation::update()
{
	m_NationText->setString("Money: " + std::to_string(m_money));
	calculateIncome();
	calculateExpenses();
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		m_ownedTiles[i]->ChangeColour(*m_nationColor);
	}
	for (int i = 0; i < m_ownedUnits.size(); i++)
	{
		if (m_ownedUnits[i]->m_destroyed)
		{
			m_ownedUnits[i]->m_nation = nullptr;
			m_ownedUnits.erase(m_ownedUnits.begin() + i);
		}
	}
	m_money += (m_income - m_expenses);
}

//Calculates the expenses generated across the nation.
void Nation::calculateExpenses()
{
	int temp = 0;
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		temp += m_ownedTiles[i]->getExpenses();
	}
	float troopCosts = 0;
	for (int i = 0; i < m_ownedUnits.size(); i++)
	{
		troopCosts += m_ownedUnits[i]->getCost();
	}
	temp += static_cast<int>(troopCosts);
	setExpenses(temp);
}

//Calculates the income generated across the nation.
void Nation::calculateIncome()
{
	int temp = 0;
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		temp += static_cast<int>(m_ownedTiles[i]->getIncome());
	}
	setIncome(temp);
}

//Checks whether the nation already owns the hex.
bool Nation::ownsHex(Hex* hexToTest)
{
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		if (hexToTest == m_ownedTiles[i])
		{
			return true;
		}
	}
	return false;
}

//Removes the hex from the nation.
void Nation::removeHex(Hex* hexToRemove)
{
	for (int i = 0; i < m_ownedTiles.size(); i++)
	{
		if (m_ownedTiles[i] == hexToRemove)
		{
			m_ownedTiles.erase((m_ownedTiles.begin() + i));
		}
	}
}

//Checks whether the country is at war.
bool Nation::checkAtWar()
{
	for (int i = 0; i < m_relations.m_diploInteractionsState.size(); i++)
	{
		if (m_relations.m_diploInteractionsState[i]->getInteractionState(DiploInteractions::war))
		{
			return true;
		}
	}
	return false;
}
