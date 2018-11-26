#include "mapLoadHandler.h"

#include <fstream>
#include <windows.h>
#include <math.h>

#include "NationHandler.h"
#include "Nation.h"
#include "HexHandler.h"
#include "Hex.h"

//Main constructor loads all maps from the files.
mapLoadHandler::mapLoadHandler()
{
	findFilesFromFolder(m_preloadedMapFolder, preloadedMapFileNames);
	findFilesFromFolder(m_saveFilesMapFolder, saveMapFileNames);
}

//Destructor.
mapLoadHandler::~mapLoadHandler()
{

}

//Checks if folder exists if it doesn't it creates it and displays any .gmf files within
void mapLoadHandler::findFilesFromFolder(const std::string& mapFolder, std::vector<std::string>& fileNames)
{

	LPCSTR folderUsableName = mapFolder.c_str();

	//Checks the save game files exist before trying to open file.
	if (GetFileAttributesA(folderUsableName) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(folderUsableName, NULL);
	}

	HANDLE hfind;
	WIN32_FIND_DATAA data;
	hfind = FindFirstFileA(folderUsableName, &data);

	do
	{
		std::string temp = mapFolder;
		for (int i = 0; i < 6; i++)
		{
			temp.pop_back();
		}
		temp += data.cFileName;
		fileNames.push_back(temp);
	} while (FindNextFileA(hfind, &data));
	FindClose(hfind);
}

//Loads from a file directory.
void mapLoadHandler::loadMapFromFile(HexHandler* mapInfo, NationHandler* nationInfo, std::string& fileName)
{
	std::vector<std::string> lines;
	std::ifstream file;
	file.open(fileName);
	lines.resize(1);
	while (getline(file, lines[(lines.size() - 1)]))
	{
		lines.resize(lines.size() + 1);
	}
	sortData(mapInfo, nationInfo, lines);
	file.close();
}

//Sorts through the data identifies what needs to be done with the data and hands the data off to a system to sort depending on whats required. 
void mapLoadHandler::sortData(HexHandler* mapInfo, NationHandler* nationInfo, const std::vector<std::string>& fileInfo)
{
	for (int i = 0; i < fileInfo.size(); i++)
	{
		//Checks for encapsulated value from title.
		if (fileInfo[i][0] == 'D' && fileInfo[i][1] == 'a' && fileInfo[i][2] == 't' && fileInfo[i][3] == 'e' && fileInfo[i][4] == ':')
		{
			int test;
			test = loadNumberValue(fileInfo[i]);
			mapInfo->m_date = test;
		}
		else if (fileInfo[i] == "Province")
		{
			int numberToCheck = 1;
			int linesToCheck = (i + 2);
			while (numberToCheck > 0)
			{
				if (fileInfo[linesToCheck][0] == '}')
				{
					numberToCheck--;
				}
				else if (fileInfo[linesToCheck][0] == '{')
				{
					numberToCheck++;
				}
				linesToCheck++;
			}
			std::vector<std::string> temp;
			for (int j = i; j < linesToCheck; j++)
			{
				temp.push_back(fileInfo[j]);
			}
			sortProvince(temp, *mapInfo);
		}
		else if (fileInfo[i] == "Nation")
		{
			int numberToCheck = 1;
			int linesToCheck = (i + 2);
			while (numberToCheck > 0)
			{
				if (fileInfo[linesToCheck][0] == '}')
				{
					numberToCheck--;
				}
				else if (fileInfo[linesToCheck][0] == '{')
				{
					numberToCheck++;
				}
				linesToCheck++;
			}
			std::vector<std::string> temp;
			for (int j = i; j < linesToCheck; j++)
			{
				temp.push_back(fileInfo[j]);
			}
			sortNation(temp, *nationInfo);
			nationInfo->m_nationsInGame[(nationInfo->m_nationsInGame.size() - 1)]->m_NationText = mapInfo->m_UI->createUITextElement(sf::Vector2f(100, 30), 14, sf::Color::White);
		}
		else if (fileInfo[i] == "Unit Stack")
		{
			int numberToCheck = 1;
			int linesToCheck = (i + 2);
			while (numberToCheck > 0)
			{
				if (fileInfo[linesToCheck][0] == '}')
				{
					numberToCheck--;
				}
				else if (fileInfo[linesToCheck][0] == '{')
				{
					numberToCheck++;
				}
				linesToCheck++;
			}
			std::vector<std::string> temp;
			for (int j = i; j < linesToCheck; j++)
			{
				temp.push_back(fileInfo[j]);
			}
			unitStack(*nationInfo, *mapInfo, temp);
		}
		else if (fileInfo[i] == "Navy Stack")
		{
			int numberToCheck = 1;
			int linesToCheck = (i + 2);
			while (numberToCheck > 0)
			{
				if (fileInfo[linesToCheck][0] == '}')
				{
					numberToCheck--;
				}
				else if (fileInfo[linesToCheck][0] == '{')
				{
					numberToCheck++;
				}
				linesToCheck++;
			}
			std::vector<std::string> temp;
			for (int j = i; j < linesToCheck; j++)
			{
				temp.push_back(fileInfo[j]);
			}
			navyStack(*nationInfo, *mapInfo, temp);
		}
	}
}

//Loads a land unit stack from a string.
void mapLoadHandler::unitStack(NationHandler& nationInfo, HexHandler& mapInfo, std::vector<std::string> linesToCheck)
{
	UnitStack* tempStack = nationInfo.createUnitWithID(loadNumberValue(linesToCheck[2]), &mapInfo);
	tempStack->setIsLandUnit(true);
	Hex* tempProvince = mapInfo.findHexByID(loadNumberValue(linesToCheck[3]));
	tempStack->m_landUnits[Hex::cavalry] = loadNumberValue(linesToCheck[4]);
	tempStack->m_landUnits[Hex::infantry] = loadNumberValue(linesToCheck[5]);
	tempStack->m_landUnits[Hex::cannon] = loadNumberValue(linesToCheck[6]);

}

//Load a navy unit from a string.
void mapLoadHandler::navyStack(NationHandler& nationInfo, HexHandler& mapInfo, std::vector<std::string> linesToCheck)
{
	UnitStack* tempStack = nationInfo.createUnitWithID(loadNumberValue(linesToCheck[2]), &mapInfo);
	tempStack->setIsLandUnit(false);
	Hex* tempProvince = mapInfo.findHexByID(loadNumberValue(linesToCheck[3]));
	tempStack->m_navalUnits[Hex::light] = loadNumberValue(linesToCheck[4]);
	tempStack->m_navalUnits[Hex::galley] = loadNumberValue(linesToCheck[5]);
	tempStack->m_navalUnits[Hex::heavy] = loadNumberValue(linesToCheck[6]);
	tempStack->m_navalUnits[Hex::trade] = loadNumberValue(linesToCheck[7]);
	tempStack->m_navalUnits[Hex::transport] = loadNumberValue(linesToCheck[8]);

}

//Loads the date from the string and into the date int value.
int mapLoadHandler::loadNumberValue(const std::string& textToReadFrom)
{
	int valueToCheck = 0;
	while (textToReadFrom[valueToCheck] != ':' && valueToCheck != textToReadFrom.size())
	{
		valueToCheck++;
	}
	//Used in the case a ':' is not used.
	if (valueToCheck == textToReadFrom.size())
	{
		valueToCheck = 0;
	}
	int start = valueToCheck;

	while (textToReadFrom[valueToCheck] != ',')
	{
		valueToCheck++;
	}
	int valueLength = valueToCheck - start;
	int finalValue = 0;

	for (int i = start; i < valueToCheck; i++)
	{
		//Adds the date value
		finalValue += static_cast<int>((charToInt[textToReadFrom[i]] * pow(10, (valueToCheck - i - 1))));
	}
	return finalValue;
}

//Checks if the value from the string input is true or false.
bool mapLoadHandler::loadBoolValue(const std::string& textToReadFrom)
{
	int i = 0;
	while (textToReadFrom[i] != ':')
	{
		i++;
	}
	if (textToReadFrom[(i + 1)] == 'T')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Loads the string directly from the string file.
std::string mapLoadHandler::loadStringValue(const std::string& textToReadFrom)
{
	int i = 0;
	while (textToReadFrom[i] != ':')
	{
		i++;
	}
	i++;
	int start = i;
	while (textToReadFrom[i] != ',')
	{
		i++;
	}
	int end = i;
	std::string value;
	for (int j = start; j < end; j++)
	{
		value.push_back(textToReadFrom[j]);
	}
	return value;
}

//sorts the data from the array of strings to the map for the game.
void mapLoadHandler::sortProvince(const std::vector<std::string>& infoToSort, HexHandler& mapInfo)
{
	int IDValue = loadNumberValue(infoToSort[2]);
	Hex* hexToChange = mapInfo.findHexByID(IDValue);

	loadTileType(infoToSort[3], hexToChange);
	//loadResourceType(infoToSort[4], hexToChange);
	//hexToChange->isDock = loadBoolValue(infoToSort[5]);
	loadQueues(infoToSort, hexToChange);
}

//Creates a nation and inputs the values of the loaded nation into it.
void mapLoadHandler::sortNation(const std::vector<std::string>& infoToSort, NationHandler& nationInfo)
{
	std::string NationTag = loadStringValue(infoToSort[3]);
	Nation* nationToUse = nationInfo.createNationWithTag(NationTag);
	nationToUse->setCountryName(loadStringValue(infoToSort[2]));
	nationToUse->setMoney(loadNumberValue(infoToSort[4]));
	loadNationQueues(infoToSort, nationToUse);
}

//Uses the text to load the tile type to the hex.
void mapLoadHandler::loadTileType(const std::string& textToReadFrom, Hex* hexToChange)
{
	int i = 0;
	while (textToReadFrom[i] != ':')
	{
		i++;
	}
	hexToChange->m_tileType = charToTileType[textToReadFrom[(i + 1)]];
}

//Loads resources from the text input... no resources implemented into the base game yet so no need to load it.
void mapLoadHandler::loadResourceType(const std::string& textToReadFrom, Hex* hexToChange)
{
	int i = 0;
	while (textToReadFrom[i] != ':')
	{
		i++;
	}
	//TODO: when resource become a feature implement loading depending on the specific types.
	if (textToReadFrom[(i + 1)] == 'G')
	{
	}
}

//Loads the data from linesToReadFrom that are relevant to the queues on the hex.
void mapLoadHandler::loadQueues(const std::vector<std::string>& linesToReadFrom, Hex* hexToChange)
{
	std::vector<std::string> tempLines;
	for (int i = 0; i < linesToReadFrom.size(); i++)
	{
		if (linesToReadFrom[i] == "Build Queue:")
		{
			tempLines = sortLinesForQueues(linesToReadFrom, i);
			loadBuildings(tempLines, hexToChange->m_buildingsBuildQueue);

		}
		else if (linesToReadFrom[i] == "Dock Queue:")
		{
			tempLines = sortLinesForQueues(linesToReadFrom, i);
			loadShipQueue(tempLines, hexToChange->m_shipBuildQueue);
		}
		else if (linesToReadFrom[i] == "Unit Queue:")
		{
			tempLines = sortLinesForQueues(linesToReadFrom, i);
			loadUnitsQueue(tempLines, hexToChange->m_unitBuildQueue);
		}
		else if (linesToReadFrom[i] == "Buildings:")
		{
			tempLines = sortLinesForQueues(linesToReadFrom, i);
			loadBuildings(tempLines, hexToChange->m_buildings);
		}

	}
}

//This loads a nations "Queues" with its owned provinces and units by using its ID.
void mapLoadHandler::loadNationQueues(const std::vector<std::string>& infoToSort, Nation* nationInfo)
{
	std::vector<std::string> tempLines;
	for (int i = 0; i < infoToSort.size(); i++)
	{
		if (infoToSort[i] == "Owned Provinces")
		{
			tempLines = sortLinesForQueues(infoToSort, i);
			loadOwnedUnits(tempLines, nationInfo->m_ownedProvincesID);
		}
		else if (infoToSort[i] == "Owned Units")
		{
			tempLines = sortLinesForQueues(infoToSort, i);
			loadOwnedUnits(tempLines, nationInfo->m_ownedUnitsID);
		}
	}
}

//Sorts data so the only lines in the output vector are usable data, that falls between the expected bounds, any unexpected data will be ignored later.
std::vector<std::string> mapLoadHandler::sortLinesForQueues(const std::vector<std::string>& linesToReadFrom, const int valueToStartFrom)
{
	std::vector<std::string> tempLines;
	int linesToCheck = (valueToStartFrom + 1);
	while (linesToReadFrom[linesToCheck][0] != '{')
	{
		linesToCheck++;
	}
	int start = (linesToCheck + 1);
	while (linesToReadFrom[linesToCheck][0] != '}')
	{
		linesToCheck++;
	}
	int end = linesToCheck;
	for (int j = start; j < end; j++)
	{
		tempLines.push_back(linesToReadFrom[j]);
	}
	return tempLines;
}

//This loads data from the vector of strings and converts that into data which is then put into the input buildingsQueue variable.
void mapLoadHandler::loadBuildings(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::buildingTypes>& buildingsQueue)
{
	std::string tempString;

	for (int i = 0; i < linesToReadFrom.size(); i++)
	{
		tempString = linesToReadFrom[i];
		tempString.pop_back();
		buildingsQueue.push_back(stringToBuildingType[tempString]);
	}
}

//This loads data from the vector of strings and converts that into data which is then put into the input shipQueue variable.
void mapLoadHandler::loadShipQueue(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::shipTypes>& shipQueue)
{
	std::string tempString;

	for (int i = 0; i < linesToReadFrom.size(); i++)
	{
		tempString = linesToReadFrom[i];
		tempString.pop_back();
		shipQueue.push_back(stringToShipTypes[tempString]);
	}
}

//This loads data from the vector of strings and converts that into data which is then put into the input unitsQueue variable.
void mapLoadHandler::loadUnitsQueue(const std::vector<std::string>& linesToReadFrom, std::vector<Hex::unitTypes>& unitsQueue)
{
	std::string tempString;

	for (int i = 0; i < linesToReadFrom.size(); i++)
	{
		tempString = linesToReadFrom[i];
		tempString.pop_back();
		unitsQueue.push_back(stringToUnitTypes[tempString]);
	}
}

//This loads data from the vector of strings and converts that into data which is then put into the input unitsIDQueue variable.
void mapLoadHandler::loadOwnedUnits(const std::vector<std::string>& linesToReadFrom, std::vector<int>& unitsIDQueue)
{
	std::string tempString;

	for (int i = 0; i < linesToReadFrom.size(); i++)
	{
		tempString = linesToReadFrom[i];
		unitsIDQueue.push_back(loadNumberValue(tempString));
	}
}