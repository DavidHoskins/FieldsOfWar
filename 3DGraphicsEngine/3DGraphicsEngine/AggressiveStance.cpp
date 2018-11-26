#include "AggressiveStance.h"

AggressiveStance::AggressiveStance()
{
	m_effect = startNode;
	m_actionPreconditions.push_back(ExpandRealm);
}

//Main action for the node.
void AggressiveStance::action(Nation* currentNation, HexHandler* HexArray)
{
	//Does nothing since this is just a sorting node.
}