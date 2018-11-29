#ifndef HEXQUADTREE_H
#define HEXQUADTREE_H

#include <vector>
#include <iostream>

#include <SFML/System/Vector2.hpp>

class Hex;

class HexQuadTree
{
	enum nodePosition {northWest, northEast, southEast, southWest};
	HexQuadTree* nodes[4];

	Hex* data[4];
	const int allocatedSpace = 4;
	int spaceUsed = 0;

	sf::Vector2f topLeft;
	sf::Vector2f bottomRight;

	void subdivide();
	bool checkInBoundingBox(Hex* Hex);
public:
	bool insertData(Hex* hexData);

	HexQuadTree(sf::Vector2f topLeftPosition, sf::Vector2f bottomRightPosition);
	~HexQuadTree();
};

#endif