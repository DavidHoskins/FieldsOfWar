#include "AttackTerritory.h"


//constructor.
AttackTerritory::AttackTerritory(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes)
{
	m_preconditions.push_back(UnitAINode::LessUnitsInStack);
	m_actionPreconditions.push_back(UnitAINode::provinceAttacked);
	m_effect = UnitAINode::UnitAttacked;
}

//Main action.
void AttackTerritory::action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit)
{

}
