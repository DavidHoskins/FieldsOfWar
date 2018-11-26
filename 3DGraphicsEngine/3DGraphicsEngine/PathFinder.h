#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <vector>
#include <cmath>

#include "Hex.h"

class PathFinder
{
private:
	int m_pathLength;
	sf::CircleShape* m_path;
	void openSurroundingTiles(Hex* currentPoint, std::vector<Hex*>& openHexs, std::vector<Hex*>& closedHexs, Hex** HexArray, int arrayWidth, int arrayHeight, bool isLandUnit);
	Hex* findLowestCost(std::vector<Hex*>& openHexs, Hex* targetHex, Hex* startHex);
	bool isOpen(std::vector<Hex*>& openHexs, Hex* currentPoint);
	bool isCorrectTileType(Hex::tileTypes hexTileType, bool isLandUnit);
public:
	PathFinder();
	~PathFinder();

	std::vector<Hex*> m_returnPath;

	void FindPath(Hex* startPoint, Hex* endPoint, Hex** HexArray, int arrayWidth, int arrayHeight, bool isLandUnit);
};

#endif // !PATHFINDER_H

