#include "mapSaveHandler.h"

#include "HexHandler.h"
#include "NationHandler.h"
#include "Nation.h"
#include "UnitStack.h"

//Saves all the information from the game into a .gmf file.
void mapSaveHandler::saveGameToFile(HexHandler& mapInfo, NationHandler& nationInfo, char* fileLocation)
{
	//Open file.
	ofstream fileToSaveTo;
	fileToSaveTo.open(fileLocation);

	//Save start date
	string valueToUse = "Date:1000";
	valueToUse += ',';
	saveLine(valueToUse, fileToSaveTo);

	//Save nations.
	for (int i = 0; i < nationInfo.m_nationsInGame.size(); i++)
	{
		saveNationToFile(*nationInfo.m_nationsInGame[i], fileToSaveTo);
	}

	//Save provinces.
	for (int i = 0; i < mapInfo.getMapWidth(); i++)
	{
		for (int j = 0; j < mapInfo.getMapHeight(); j++)
		{
			saveProvinceToFile(*mapInfo.getHexAtPosition(i,j), fileToSaveTo);
		}
	}

	for (int i = 0; i < mapInfo.m_unitsInGame.size(); i++)
	{
		saveUnitToFile(*mapInfo.m_unitsInGame[i], fileToSaveTo);
	}

	//Close files.
	fileToSaveTo.close();
}

//Saves all info about the nation to the save file.
void mapSaveHandler::saveNationToFile(Nation& nationToConvert, ofstream& fileToSaveTo)
{
	//Saving the header for identification later.
	string valueToSave = "Nation";
	saveLine(valueToSave, fileToSaveTo);
	valueToSave = "{";
	saveLine(valueToSave, fileToSaveTo);

	//Saving the name of the country.
	valueToSave = "Name:";
	valueToSave += nationToConvert.getCountryName();
	valueToSave += ',';
	saveLine(valueToSave, fileToSaveTo);

	//Saving tag of country.
	valueToSave = "Tag:";
	valueToSave += nationToConvert.getCountryTag();
	valueToSave += ',';
	saveLine(valueToSave, fileToSaveTo);

	//Saving money of country.
	valueToSave = "Money:";
	valueToSave += std::to_string(nationToConvert.getMoney());
	valueToSave += ',';
	saveLine(valueToSave, fileToSaveTo);

	//Saving queues
	saveOwnedProvincesToFile(nationToConvert, fileToSaveTo);
	saveOwnedUnitsToFile(nationToConvert.m_ownedUnits, fileToSaveTo);

	//Saving the footer.
	valueToSave = "}";
	saveLine(valueToSave, fileToSaveTo);

}

//Saves the provinces owned by the nation by using their ID's.
void mapSaveHandler::saveOwnedProvincesToFile(Nation& nationToUse, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Owned Provinces";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);
	for (int i = 0; i < nationToUse.m_ownedTiles.size(); i++)
	{
		valueToUse = std::to_string(nationToUse.m_ownedTiles[i]->hexID);
		valueToUse += ',';
		saveLine(valueToUse, fileToSaveTo);
	}
	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}

//This method goes through all the owned units in a nation and outputs it into the file.
void mapSaveHandler::saveOwnedUnitsToFile(std::vector<UnitStack*>& unitsToConvert, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Owned Units";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);
	for (int i = 0; i < unitsToConvert.size(); i++)
	{
		valueToUse = std::to_string(unitsToConvert[i]->getUnitID());
		valueToUse += ',';
		saveLine(valueToUse, fileToSaveTo);
	}
	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}


//saves the map information to a file which can be called later.
void mapSaveHandler::saveProvinceToFile(Hex& province, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Province";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);

	//Saving province ID.
	valueToUse = "ID:";
	valueToUse += std::to_string(province.hexID);
	valueToUse += ',';
	saveLine(valueToUse, fileToSaveTo);

	//Saving tile type.
	valueToUse = "Tile type:";
	valueToUse += convertTileToChar(province.m_tileType);
	valueToUse += ',';
	saveLine(valueToUse, fileToSaveTo);

	//Saving resources... TODO:

	//Saving build queues.
	saveUnitsInQueue(province, fileToSaveTo);
	saveNavyInQueue(province, fileToSaveTo);
	saveBuildingsInQueue(province, fileToSaveTo);

	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}

//Converts the input tileType to a usable char.
char mapSaveHandler::convertTileToChar(Hex::tileTypes typeToConvert)
{
	switch (typeToConvert)
	{
	case Hex::grass:
		return 'g';
		break;
	case Hex::hill:
		return 'h';
		break;
	case Hex::field:
		return 'f';
		break;
	case Hex::city:
		return 'c';
		break;
	case Hex::inlandSea:
		return 'i';
		break;
	case Hex::openSea:
		return 'o';
		break;
	default:
		break;
	}
	return ' ';
}

//Saves all units in the queue to file.
void mapSaveHandler::saveUnitsInQueue(Hex& province, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Unit Queue";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);

	for (int i = 0; i < province.m_unitBuildQueue.size(); i++)
	{
		valueToUse = province.m_unitBuildQueue[i];
		valueToUse += ',';
		saveLine(valueToUse, fileToSaveTo);
	}
	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}

//Saves all navy in the queue to file.
void mapSaveHandler::saveNavyInQueue(Hex& province, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Navy Queue";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);

	for (int i = 0; i < province.m_shipBuildQueue.size(); i++)
	{
		valueToUse = province.m_shipBuildQueue[i];
		valueToUse += ',';
		saveLine(valueToUse, fileToSaveTo);
	}

	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}

//Saves all buildings in the queue to file.
void mapSaveHandler::saveBuildingsInQueue(Hex& province, ofstream& fileToSaveTo)
{
	string valueToUse;
	valueToUse = "Building Queue";
	saveLine(valueToUse, fileToSaveTo);
	valueToUse = "{";
	saveLine(valueToUse, fileToSaveTo);

	for (int i = 0; i < province.m_buildingsBuildQueue.size(); i++)
	{
		valueToUse = province.m_buildingsBuildQueue[i];
		valueToUse += ',';
		saveLine(valueToUse, fileToSaveTo);
	}

	valueToUse = "}";
	saveLine(valueToUse, fileToSaveTo);
}

//Saves all units to the file.
void mapSaveHandler::saveUnitsToFile(NationHandler& nationInfo, ofstream& fileToSaveTo)
{
	for (int i = 0; i < nationInfo.m_nationsInGame.size(); i++)
	{
		for (int j = 0; j < nationInfo.m_nationsInGame[i]->m_ownedUnits.size(); j++)
		{
			saveUnitToFile(*nationInfo.m_nationsInGame[i]->m_ownedUnits[j], fileToSaveTo);
		}
	}
}

//Saves the UnitStacks to the file.
void mapSaveHandler::saveUnitToFile(UnitStack& unitToConvert, ofstream& fileToSaveTo)
{
	string valueToSave;
	//Is a land unit.
	if (unitToConvert.getIsLandUnit())
	{
		valueToSave = "Unit Stack";
		saveLine(valueToSave, fileToSaveTo);
	}
	else
	{
		valueToSave = "Navy Stack";
		saveLine(valueToSave, fileToSaveTo);
	}
	valueToSave = "{";
	saveLine(valueToSave, fileToSaveTo);

	//Save ID.
	valueToSave = "ID:";
	valueToSave += std::to_string(unitToConvert.getUnitID());
	valueToSave += ',';
	saveLine(valueToSave, fileToSaveTo);

	//Save Province position.
	valueToSave = "Province ID:";
	valueToSave += std::to_string(unitToConvert.m_latestHex->hexID);
	valueToSave += ',';
	saveLine(valueToSave, fileToSaveTo);


	//Is a land unit.
	if (unitToConvert.getIsLandUnit())
	{
		//Save number of cav.
		valueToSave = "Cavalry:";
		valueToSave += std::to_string(unitToConvert.m_landUnits[Hex::cavalry]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//Save number of inf.
		valueToSave = "Infantry:";
		valueToSave += std::to_string(unitToConvert.m_landUnits[Hex::infantry]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//save number of can.
		valueToSave = "Cannons:";
		valueToSave += std::to_string(unitToConvert.m_landUnits[Hex::cannon]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);
	}
	//Is a naval unit.
	else
	{
		//Save number of light ships.
		valueToSave = "Light:";
		valueToSave += std::to_string(unitToConvert.m_navalUnits[Hex::light]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//Save number of galley ships.
		valueToSave = "Galley:";
		valueToSave += std::to_string(unitToConvert.m_navalUnits[Hex::galley]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//Save number of heavy ships.
		valueToSave = "Heavy:";
		valueToSave += std::to_string(unitToConvert.m_navalUnits[Hex::heavy]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//Save number of trade ships.
		valueToSave = "Trade:";
		valueToSave += std::to_string(unitToConvert.m_navalUnits[Hex::trade]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);

		//Save number of transport ships.
		valueToSave = "Transport:";
		valueToSave += std::to_string(unitToConvert.m_navalUnits[Hex::transport]);
		valueToSave += ',';
		saveLine(valueToSave, fileToSaveTo);
	}

	valueToSave = "}";
	saveLine(valueToSave, fileToSaveTo);

}

//Takes the input of a string and loads it into the file with a '\n' character to end the line.
void mapSaveHandler::saveLine(string& textToSave, ofstream& fileToSaveTo)
{
	fileToSaveTo << textToSave << '\n';
}