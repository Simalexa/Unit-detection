#ifndef UNIT_H
#define UNIT_H
#include <string>
#include <vector>

const double PI = 3.141592653589793238463;

struct Unit
{
	int x, y, unitsNear; //unit coordinates and number of units visible by curent unit
	std::string name; //unit name
	double Vx, Vy; //dirrection
};

class Analyzer
{
private:
	void createUnit(std::string &s); //add new unit to vector
	std::string findFirstNumber(std::string& s); //analyzing string from the input file and returning first number from it(number cooresponds to unit coodinates/dirrection)
	std::string findName(std::string& s); //return name of Unit from the string
	void checkUnit(Unit &u); //checking if dirrection is normilized vector

public:
	Analyzer();
	~Analyzer();
	void readFile(std::string fileName); //read file and generate vector of units structure
	void detectionForUnits(int firstUnit, int lastUnit); //find nearby units for units from [firstUnits, lastUnits]
protected:
	float _detectionAngle = 0;
	int _detectionRadius = 0;
	std::vector<Unit> _units; //all units in the map
	void getMapSize(int& minX, int& maxX, int& minY, int& maxY); //get maximum and minimum units coordiante to print the map
	bool avaliablePosition(int x, int y); //checking if position is free from units
	void detectUnitsForOneUnit(int numberOfUnit); //find units near current units
};

class GetInformation: public Analyzer
{
public:
	int getMaximumUnits() { return _units.size(); }
	void printUnitCharacteristics(); //print units parameters in console
	void printMap(); //printing map where 0 is free position and 1 is unit position
	void printNumberOfUnitsNearTargets(); //print in console
	void printResultInFile(std::string fileName);
};

#endif