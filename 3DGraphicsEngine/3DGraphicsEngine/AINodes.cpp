#include "AINodes.h"
#include "UnitStack.h"

//Constructor.
void AINodes::startUp(std::vector<bool*>& preconditions, std::vector<AINodes*>& nodes)
{
	for (int i = 0; i < m_preconditions.size(); i++)
	{
		m_preconditionsState.push_back(preconditions[m_preconditions[i]]);
	}
	for (int i = 0; i < m_actionPreconditions.size(); i++)
	{
		m_nodePreconditionStates.push_back(preconditions[m_actionPreconditions[i]]);
	}
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < m_actionPreconditions.size(); j++)
		{
			if (nodes[i]->m_effect == m_actionPreconditions[j])
			{
				m_preconditionNodes.push_back(nodes[i]);
			}
		}
	}
	m_effectState = preconditions[m_effect];
}

//returns the best path the node can access.
std::vector<AINodes*> AINodes::getBestPath()
{
	std::vector<AINodes*> pathToReturn;
	int cost = 0;

	//Checks whether the nodes preconditions are already met.
	if (checkNodePreconditions())
	{
		m_cost = m_baseCost;
		pathToReturn.push_back(this);
		return pathToReturn;
	}
	else
	{
		cost = k_maxValue;
	}

	for (int j = 0; j < m_preconditionNodes.size(); j++)
	{
		if (m_preconditionNodes[j]->checkPreconditions())
		{
			std::vector<AINodes*> temp = m_preconditionNodes[j]->getBestPath();
			if (temp.size() > 0)
			{
				if (temp[0]->m_cost < cost && temp[0]->m_cost != k_maxValue)
				{
					cost = temp[0]->m_cost;
					pathToReturn = temp;
				}
			}
		}
	}
	//If the cost is equal to maxValue then that node has no path to completion and therefore is not a legal path.
	if (cost != k_maxValue)
	{
		pathToReturn.push_back(this);
		cost += m_baseCost;
	}
	m_cost = cost;
	return pathToReturn;

}

//Checks all preconditions are true.
bool AINodes::checkPreconditions()
{
	for (int i = 0; i < m_preconditionsState.size(); i++)
	{
		if (!*m_preconditionsState[i])
		{
			return false;
		}
	}
	return true;
}

//Checks if all the internal preconditions are already true.
bool AINodes::checkNodePreconditions()
{
	for (int i = 0; i < m_nodePreconditionStates.size(); i++)
	{
		if (!*m_nodePreconditionStates[i])
		{
			return false;
		}
	}
	return true;
}