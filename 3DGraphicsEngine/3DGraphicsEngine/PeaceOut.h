#pragma once
#include "AINodes.h"
class PeaceOut:
	public AINodes
{
public:
	PeaceOut();
	void action(Nation* currentNation, HexHandler* HexArray);
};

