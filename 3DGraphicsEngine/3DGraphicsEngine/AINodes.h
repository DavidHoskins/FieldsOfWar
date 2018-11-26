#ifndef AINODES_H
#define AINODES_H

#include <vector>
#include <string.h>

class Nation;
class HexHandler;
class UnitStack;

class AINodes
{
public:
	enum preconditionTypes {startNode, ExpandRealm, TerritoryGained, buildArmy, wantsToEndWar, hasMoneyForTroops, suitableTargetForWar, isAtWar, notAtWar, brokenAlliance, brokenRoyalMarriage, finalCheck};
	int m_baseCost = 1;
	int m_cost;

	virtual void action(Nation* currentNation, HexHandler* HexArray) = 0;
	std::vector<AINodes*> getBestPath();
	void startUp(std::vector<bool*>& preconditions, std::vector<AINodes*>& nodes);

protected:
	//Global precondition (not effected by internal AI nodes).
	std::vector<bool*> m_preconditionsState;

	//Variables to know which preconditions an effects each node is responsible for.
	std::vector<preconditionTypes> m_preconditions;
	std::vector<preconditionTypes> m_actionPreconditions;
	preconditionTypes m_effect;

	//States which can be effected by internal nodes (checks whether is still true).
	std::vector<bool*> m_nodePreconditionStates;
	bool* m_effectState;

	//This variable holds all the nodes which can result in the needed effect, for this nodes preconditions.
	std::vector<AINodes*> m_preconditionNodes;

	const int k_maxValue = 1000000;

private:
	bool checkNodePreconditions();
	bool checkPreconditions();
};

#endif // !AINODES_H

