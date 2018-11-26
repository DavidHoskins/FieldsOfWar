#ifndef ATTACKENEMY_H
#define ATTACKENEMY_H

#include "UnitAINode.h"

class AttackEnemy : public UnitAINode
{
public:
	AttackEnemy(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);
	void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit);
};

#endif //!ATTACKENEMY_H

