#ifndef AGGRESSIVESTANCE_H
#define AGGRESSIVESTANCE_H

#include "AINodes.h"
class AggressiveStance: public AINodes
{
public:
	void action(Nation* currentNation, HexHandler* HexArray);
	AggressiveStance();

};

#endif // !AGGRESSIVESTANCE_H
