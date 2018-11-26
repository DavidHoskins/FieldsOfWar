#ifndef DECLAREWAR_H
#define DECLAREWAR_H

#include "AINodes.h"

#include "Nation.h"
#include "RelationshipHandler.h"
#include "HexHandler.h"
class DeclareWar:public AINodes
{
public:
	DeclareWar();
	void action(Nation* currentNation, HexHandler* HexArray);
private:
	Nation* suitableTarget(Nation* currentNation, HexHandler* HexArray);
	Nation* HexesNation(HexHandler* HexArray, Hex* HexToTest);
};

#endif //!DECLAREWAR_H