#include "BuildUnits.h"

#include "Nation.h"
#include "HexHandler.h"

//Constructor.
BuildUnits::BuildUnits()
{
	m_effect = AINodes::buildArmy;
	m_preconditions.push_back(AINodes::hasMoneyForTroops);
}

//Main action for the node. Builds a unit in the current nations owned provinces.
void BuildUnits::action(Nation* currentNation, HexHandler* HexArray)
{
	HexArray->buildUnit(currentNation, HexArray, currentNation->m_ownedTiles[0]);
}