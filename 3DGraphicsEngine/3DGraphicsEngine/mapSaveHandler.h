#ifndef MAPSAVEHANDLER_H
#define MAPSAVEHANDLER_H

#include <fstream>
#include <windows.h>

#include "Hex.h"

using namespace std;

class HexHandler;
class NationHandler;
class Nation;
class UnitStack;

class mapSaveHandler
{
public:
	void saveGameToFile(HexHandler& mapInfo, NationHandler& nationInfo, char* fileLocation);
private:
	void saveNationToFile(Nation& nationToConvert, ofstream& fileToSaveTo);
	void saveOwnedUnitsToFile(std::vector<UnitStack*>& unitsToConvert, ofstream& fileToSaveTo);
	void saveOwnedProvincesToFile(Nation& nationToUse, ofstream& fileToSaveTo);

	void saveProvinceToFile(Hex& province, ofstream& fileToSaveTo);
	char convertTileToChar(Hex::tileTypes typeToConvert);
	void saveUnitsInQueue(Hex& province, ofstream& fileToSaveTo);
	void saveNavyInQueue(Hex& province, ofstream& fileToSaveTo);
	void saveBuildingsInQueue(Hex& province, ofstream& fileToSaveTo);

	void saveUnitsToFile(NationHandler& nationInfo, ofstream& fileToSaveTo);
	void saveUnitToFile(UnitStack& unitToConvert, ofstream& fileToSaveTo);

	void saveLine(string& textToSave, ofstream& fileToSaveTo);
};

#endif //!MAPSAVEHANDLER

