#include "HexQuadTree.h"

#include "Hex.h";

//Constructor
HexQuadTree::HexQuadTree(sf::Vector2f topLeftPosition, sf::Vector2f bottomRightPosition)
{
	for(int i = 0; i < 4; i++)
	{
		nodes[i] = nullptr;
		data[i] = nullptr;
	}
	topLeft = topLeftPosition;
	bottomRight = bottomRightPosition;
}

//Destructor
HexQuadTree::~HexQuadTree()
{
	for(int i = 0; i < 4; i++)
	{
		delete nodes[i];
	}
}

//When data is input the tree will find the next avaliable position for the hex within the specified bounding box.
bool HexQuadTree::insertData(Hex* hexData)
{
	if(!checkInBoundingBox(hexData))
	{
		return false;
	}
	if(spaceUsed < allocatedSpace)
	{
		data[spaceUsed] = hexData;
		spaceUsed++;
		return true;
	}
	if(nodes[northWest] == nullptr)
	{
		subdivide();
	}

	if (nodes[northWest]->insertData(hexData)) { return true; }
	if (nodes[northEast]->insertData(hexData)) { return true; }
	if (nodes[southWest]->insertData(hexData)) { return true; }
	if (nodes[southEast]->insertData(hexData)) { return true; }

	std::cerr << "Error creating quad-tree unexpected exception." << std::endl;
	return false;
}

//When all data is filled a new set of quad trees will be created to facilitate this data.
void HexQuadTree::subdivide()
{
	nodes[northWest] = new HexQuadTree(topLeft, sf::Vector2f(topLeft.x + (bottomRight.x / 2), topLeft.y + (bottomRight.y / 2)));
	nodes[northEast] = new HexQuadTree(sf::Vector2f(bottomRight.x - (bottomRight.x / 2), topLeft.y), sf::Vector2f(bottomRight.x, topLeft.y + (bottomRight.y / 2)));

	nodes[southEast] = new HexQuadTree(sf::Vector2f(bottomRight.x - (bottomRight.x / 2), topLeft.y + (bottomRight.y / 2)), bottomRight);
	nodes[southWest] = new HexQuadTree(sf::Vector2f(topLeft.x, topLeft.y + (bottomRight.y / 2)), sf::Vector2f(bottomRight.x - (bottomRight.x / 2), bottomRight.y));

	int test = 0;

	for(int i = 0; i < spaceUsed; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(nodes[j]->insertData(data[i]))
			{
				test++;
			}
		}
		data[i] = nullptr;
	}
	spaceUsed = 0;
	if(test < 4)
	{
		std::cerr << "Error filling quad-tree correctly." << std::endl;
	}
}

//Checks whether the position of the Hex Falls inside this bounding box.
bool HexQuadTree::checkInBoundingBox(Hex* Hex)
{
	sf::Vector2f tempPosition = Hex->getPosition();
	float tempRadius = Hex->getRadius();

	if(tempPosition.x - tempRadius > bottomRight.x || tempPosition.y - tempRadius > bottomRight.y)
	{
		return false;
	}
	else if(tempPosition.x + tempRadius < topLeft.x || tempPosition.y + tempRadius < topLeft.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

