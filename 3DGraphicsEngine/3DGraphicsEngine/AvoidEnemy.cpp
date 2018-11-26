#include "AvoidEnemy.h"

#include "HexHandler.h"
#include "UnitStack.h"


//constructor.
AvoidEnemy::AvoidEnemy(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes)
{
	m_effect = UnitAINode::UnitNotInRange;
	m_preconditions.push_back(UnitAINode::UnitInRange);
}

//Main action for node.
void AvoidEnemy::action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit)
{
	Hex* currentTile = currentUnit->m_latestHex;
	int currentX;
	int currentY;
	for (int i = 0; i < HexArray->getMapWidth(); i++)
	{
		for (int j = 0; j < HexArray->getMapHeight(); j++)
		{
			if (HexArray->getHexAtPosition(i, j) == currentTile)
			{
				currentX = i;
				currentY = j;
			}
		}
	}
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if ((currentX + i) >= 0 && (currentX + i) < HexArray->getMapWidth() && (currentY + j) >= 0 && (currentY + j) < HexArray->getMapHeight())
			{
				currentUnit->m_targetHex = HexArray->getHexAtPosition((currentX + i), (currentY + j));
				return;
			}
		}
	}
}