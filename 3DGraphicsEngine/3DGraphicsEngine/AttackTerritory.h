#pragma once
#include "UnitAINode.h"
class AttackTerritory: public UnitAINode
{
public:
	AttackTerritory(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);
	void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit);
};

