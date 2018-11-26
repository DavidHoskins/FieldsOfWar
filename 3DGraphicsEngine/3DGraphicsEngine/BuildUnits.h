#ifndef BUILDUNITS_H
#define BUILDUNITS_H

#include "AINodes.h"
class BuildUnits:
	public AINodes
{
public:
	BuildUnits();
	void action(Nation* currentNation, HexHandler* HexArray);
};

#endif //!BUILDUNITS_H