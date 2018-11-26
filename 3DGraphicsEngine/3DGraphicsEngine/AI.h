#ifndef AI_H
#define AI_H

#include <vector>

#include "AINodes.h"

class Nation;
class Hex;
class HexHandler;

class AI
{
private:
	//Used to store all AILogic nodes.
	std::vector<AINodes*> m_AILogic;

	//Used for the current AI logic path.
	std::vector<AINodes*> m_currentAIPath;
	std::vector<bool*> m_preconditionsState;

	void checkPreconditions(Nation* currentNation, HexHandler* HexArray);
	void checkAtWar(Nation* currentNation);
	void checkHasEnoughForTroops(Nation* currentNation);
	void checkSuitableTargetForWar(Nation* currentNation, HexHandler* HexArray);
	void checkForPeaceOut(Nation* currentNation);

	Nation* HexesNation(HexHandler* HexArray, Hex* HexToTest);
	Nation* suitableTarget(Nation* currentNation, HexHandler* HexArray);

	int m_numTiles = 0;

public:
	void update(Nation* currentNation, HexHandler* HexArray);
	AI();
	~AI();
};
#endif //!AI_H
