#pragma once
#include "UnitAINode.h"
class AvoidEnemy : public UnitAINode
{
public:
	AvoidEnemy(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);
	void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit);
};

