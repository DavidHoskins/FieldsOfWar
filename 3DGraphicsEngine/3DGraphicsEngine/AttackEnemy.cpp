#include "AttackEnemy.h"


//constructor.
AttackEnemy::AttackEnemy(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes)
{
	m_effect = UnitAINode::IsAtWar;
	m_actionPreconditions.push_back(UnitAINode::UnitAttacked);
	startUp(preconditions, nodes);
}

//main actions of this node.
void AttackEnemy::action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit)
{

}