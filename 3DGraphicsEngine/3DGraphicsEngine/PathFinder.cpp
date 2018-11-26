#include "PathFinder.h"

//Constructor.
PathFinder::PathFinder()
{

}

//Destructor.
PathFinder::~PathFinder()
{
	delete[] m_path;
}

//Finds the shortest path between two points using the A* method.
void PathFinder::FindPath(Hex* startPoint, Hex* endPoint, Hex** HexArray, int arrayWidth, int arrayHeight, bool isLandUnit)
{
	Hex* currentPoint = startPoint;

	std::vector<Hex*> openHexs;
	std::vector<Hex*> closedHexs;
	std::vector<Hex*> fastestPath;

	Hex* lastPath;

	while (currentPoint != endPoint)
	{
		lastPath = currentPoint;
		closedHexs.push_back(currentPoint);
		for (int i = 0; i < openHexs.size(); i++)
		{
			if (openHexs[i] == currentPoint)
			{
				openHexs.erase(openHexs.begin() + i);
			}
		}
		openSurroundingTiles(currentPoint, openHexs, closedHexs, HexArray, arrayWidth, arrayHeight, isLandUnit);
		currentPoint = findLowestCost(openHexs, endPoint, startPoint);
		//If a path cannot be made it will return a path with only the current position.
		if (openHexs.size() == 0 || currentPoint == nullptr)
		{
			fastestPath.push_back(startPoint);
			m_returnPath = fastestPath;
			return;
		}
	}

	while (currentPoint != startPoint)
	{
		fastestPath.push_back(currentPoint);
		currentPoint = currentPoint->m_fastestHex;
	}
	//Clears cached data inside the opened and closed Hex's as it is no longer required.
	for (int i = 0; i < openHexs.size(); i++)
	{
		openHexs[i]->m_fastestHex = nullptr;
	}
	for (int i = 0; i < closedHexs.size(); i++)
	{
		closedHexs[i]->m_fastestHex = nullptr;
	}

	m_returnPath = fastestPath;

	currentPoint = nullptr;
	lastPath = nullptr;
}

//Opens the tiles surrounding a point and changes the colour so I can see the effects.
void PathFinder::openSurroundingTiles(Hex* currentPoint, std::vector<Hex*>& openHexs, std::vector<Hex*>& closedHexs, Hex** HexArray, int arrayWidth, int arrayHeight, bool isLandUnit)
{
	int currentPointX = -1;
	int currentPointY = -1;

	for (int i = 0; i < arrayHeight; i++)
	{
		for (int j = 0; j < arrayWidth; j++)
		{
			if (HexArray[i + (j * arrayWidth)] == currentPoint)
			{
				currentPointX = i;
				currentPointY = j;
			}
		}
	}


	sf::Vector2f currentPosition = HexArray[currentPointX + (currentPointY * arrayWidth)]->getPosition();

	for (int i = 0; i < arrayWidth; i++)
	{
		for (int j = 0; j < arrayHeight; j++)
		{
			if (isCorrectTileType(HexArray[i + (j * arrayWidth)]->m_tileType, isLandUnit))
			{
				sf::Vector2f positionToTest = HexArray[i + (j * arrayWidth)]->getPosition();

				if (positionToTest.y >= (currentPosition.y - (currentPoint->getRadius() * 2.2)) && positionToTest.y <= (currentPosition.y + (currentPoint->getRadius() * 2.2)))
				{
					if (positionToTest.x >= (currentPosition.x - (currentPoint->getRadius() * 2.2)) && positionToTest.x <= (currentPosition.x + (currentPoint->getRadius() * 2.2)))
					{
						if (!(positionToTest.y == currentPosition.y && positionToTest.x == currentPosition.x))
						{
							if (!(isOpen(openHexs, HexArray[i + (j * arrayWidth)]) || isOpen(closedHexs, HexArray[i + (j * arrayWidth)])))
							{
								openHexs.push_back(HexArray[i + (j * arrayWidth)]);
								HexArray[i + (j * arrayWidth)]->m_fastestHex = currentPoint;
								float tempXPos = abs(currentPoint->getPosition().x - HexArray[i + (j * arrayWidth)]->getPosition().x);
								float tempYPos = abs(currentPoint->getPosition().y - HexArray[i + (j * arrayWidth)]->getPosition().y);
								float tempCost = tempXPos + tempYPos + currentPoint->m_baseMovementCost;
								HexArray[i + (j * arrayWidth)]->m_costToReturn = static_cast<int>(currentPoint->m_costToReturn + tempCost);
							}
						}
					}
				}
			}
		}
	}
}

//This method checks that the Hex is already opened.
bool PathFinder::isOpen(std::vector<Hex*>& openHexs, Hex* currentPoint)
{
	for (int i = 0; i < openHexs.size(); i++)
	{
		if (currentPoint == openHexs[i])
		{
			return true;
		}
	}
	return false;
}

//Returns the Hex with the lowest Manhattan cost for distance to the target + distance to start.
Hex* PathFinder::findLowestCost(std::vector<Hex*>& openHexs, Hex* targetHex, Hex* startHex)
{
	float lowestCost = 10000000.0f;
	Hex* HexToReturn = nullptr;

	for (int i = 0; i < openHexs.size(); i++)
	{
		if (openHexs[i] == targetHex)
		{
			return openHexs[i];
		}

		float distFromTarget = std::abs(openHexs[i]->getPosition().x - targetHex->getPosition().x) + std::abs(openHexs[i]->getPosition().y - targetHex->getPosition().y);

		float distFromStart = static_cast<float>(openHexs[i]->m_costToReturn);

		if (distFromTarget + distFromStart < lowestCost)
		{
			lowestCost = distFromTarget + distFromStart;
			HexToReturn = openHexs[i];
		}
	}
	return HexToReturn;
}

//Checks whether the unit can travel on this type of hex.
bool PathFinder::isCorrectTileType(Hex::tileTypes hexTileType, bool isLandUnit)
{
	if (isLandUnit)
	{
		switch (hexTileType)
		{
		case Hex::inlandSea:
			return false;
			break;
		case Hex::openSea:
			return false;
			break;
		default:
			return true;
			break;
		}
	}
	else
	{
		switch (hexTileType)
		{
		case Hex::inlandSea:
			return true;
			break;
		case Hex::openSea:
			return true;
			break;
		default:
			return false;
			break;
		}
	}
}
