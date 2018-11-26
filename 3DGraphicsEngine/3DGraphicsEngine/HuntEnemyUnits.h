#ifndef HUNTENEMYUNITS_H
#define HUNTENEMYUNITS_H

#include "UnitAINode.h"
class HuntEnemyUnits: public UnitAINode
{
public:
	HuntEnemyUnits(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);
	void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit);
};

#endif //!HUNTENEMYUNITS_H
