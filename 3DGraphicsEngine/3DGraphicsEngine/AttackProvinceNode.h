#pragma once
#include "UnitAINode.h"
class Hex;
class AttackProvinceNode: public UnitAINode
{
public:
	AttackProvinceNode(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);
	void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit);
private:
	Hex* findLegalTile(std::vector<Hex*>& tilesToCheck);
	Hex* findFirstLegalTile(Nation* currentNation, HexHandler* HexArray);
};

