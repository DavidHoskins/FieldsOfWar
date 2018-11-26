#ifndef UNITAINODE_H
#define UNITAINODE_H

#include <vector>

class Nation;
class HexHandler;
class UnitStack;

class UnitAINode
{
public:
	//Final check is not a real precondition type and is only used as a check for how long preconditionTypes is.
		enum preconditionTypes {UnitInRange, UnitNotInRange, IsAtWar, UnitAttacked, MoreUnitsInStack, LessUnitsInStack, provinceAttacked, emptyCheck, finalCheck};

		int m_baseCost = 1;
		int m_cost;

		virtual void action(Nation* currentNation, HexHandler* HexArray, UnitStack* currentUnit) = 0;
		std::vector<UnitAINode*> getBestPath();
		void startUp(std::vector<bool*>& preconditions, std::vector<UnitAINode*>& nodes);

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
	std::vector<UnitAINode*> m_preconditionNodes;

	const int k_maxValue = 1000000;

private:
	bool checkNodePreconditions();
	bool checkPreconditions();
};

#endif //!UNITAINODE