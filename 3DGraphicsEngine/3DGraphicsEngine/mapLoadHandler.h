#ifndef MAPLOADHANDLER_H
#define MAPLOADHANDLER_H

#include <string>
#include <vector>
#include <map>

#include "Hex.h"

class NationHandler;
class Nation;
class HexHandler;

class mapLoadHandler
{
private:

	char* m_preloadedMapFolder = "maps/*.gmf*";
	char* m_saveFilesMapFolder = "saveFiles/*.gmf*";
	void findFilesFromFolder(const std::string& mapFolder, std::vector<std::string>& fileNames);
	void sortData(HexHandler* mapInfo, NationHandler* nationInfo, const std::vector<std::string>& fileInfo);
	int loadNumberValue(const std::string& textToReadFrom);
	bool loadBoolValue(const std::string& textToReadFrom);
	std::string loadStringValue(const std::string& textToReadFrom);

	void sortProvince(const std::vector<std::string>& infoToSort, HexHandler& mapInfo);
	void loadTileType(const std::string& textToReadFrom, Hex* hexToChange);
	void loadResourceType(const std::string& textToReadFrom, Hex* hexToChange);

	void loadQueues(const std::vector<std::string>& linesToReadFrom, Hex* hexToChange);
	std::vector<std::string> sortLinesForQueues(const std::vector<std::string>& linesToReadFrom, const int valueToStartFrom);
	void loadBuildings(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::buildingTypes>& buildingsQueue);
	void loadShipQueue(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::shipTypes>& shipQueue);
	void loadUnitsQueue(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::unitTypes>& unitsQueue);

	void sortNation(const std::vector<std::string>& infoToSort, NationHandler& nationInfo);
	void loadNationQueues(const std::vector<std::string>& infoToSort, Nation* nationInfo);
	void loadOwnedUnits(const std::vector<std::string>& linesToReadFrom, std::vector<int>& unitsID);

	void unitStack(NationHandler& nationInfo, HexHandler& mapInfo, std::vector<std::string> linesToCheck);
	void navyStack(NationHandler& nationInfo, HexHandler& mapInfo, std::vector<std::string> linesToCheck);
public:

	mapLoadHandler();
	~mapLoadHandler();
	std::string m_testing;
	void loadMapFromFile(HexHandler* mapInfo, NationHandler* nationInfo, std::string& folderName);
	std::vector<std::string> preloadedMapFileNames;
	std::vector<std::string> saveMapFileNames;

	std::map<char, int> charToInt {{'0' , 0}, {'1' , 1}, {'2', 2}, {'3' , 3}, {'4' , 4}, {'5' , 5}, {'6' , 6}, {'7' , 7}, {'8' , 8}, {'9' , 9}};
	std::map<char, Hex::tileTypes> charToTileType {{'g', Hex::grass}, {'f', Hex::field}, {'h',Hex::hill}, {'c', Hex::city}, {'i', Hex::inlandSea}, {'o', Hex::openSea}};
	std::map<std::string, Hex::buildingTypes> stringToBuildingType {{"MARKET", Hex::market}, {"TEMPLE", Hex::temple}, {"FISHERY", Hex::fishery}, {"FACTORY", Hex::factory}};
	std::map<std::string, Hex::unitTypes> stringToUnitTypes {{"CAVALRY", Hex::cavalry}, {"INFANTRY", Hex::infantry}, {"CANNON", Hex::cannon}};
	std::map<std::string, Hex::shipTypes> stringToShipTypes {{"LIGHT", Hex::light}, {"HEAVY", Hex::heavy}, {"GALLEY", Hex::galley}, {"TRADE", Hex::trade}, {"TRANSPORT", Hex::transport}};
};

#endif // !MAPLOADHANDLER_H

