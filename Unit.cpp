#include "Unit.h"
#include <fstream>
#include <iostream>

Analyzer::Analyzer()
{
}
Analyzer::~Analyzer()
{
}
void Analyzer::readFile(std::string fileName)
{
	std::ifstream file;
	std::string str;
	int n = 0;
	file.open(fileName);
	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			if (n == 0)
			{
				_detectionAngle = std::stod(findFirstNumber(str)) / 180 * PI;
				_detectionRadius = std::stoi(findFirstNumber(str));
			}
			if (n > 0)
				createUnit(str);
			n++;
		}
		file.close();
	}
	else
		throw "File with unit characteristics not founded.";
}
void Analyzer::createUnit(std::string &s)
{
	Unit u;
	u.name = findName(s);
	u.x = std::stoi(findFirstNumber(s));
	u.y = std::stoi(findFirstNumber(s));
	u.Vx = std::stod(findFirstNumber(s));
	u.Vy = std::stod(findFirstNumber(s));
	u.unitsNear = 0;
	checkUnit(u);
	_units.push_back(u);
}

std::string Analyzer::findName(std::string& s)
{
	std::string name;
	size_t lastLetter = s.find_first_of(",");
	name = s.substr(0, lastLetter);
	s.erase(0, lastLetter);
	return name;
}

std::string Analyzer::findFirstNumber(std::string& s)
{
	std::string digits = "0123456789+-.", number;
	size_t firstDigit = s.find_first_of(digits);
	s.erase(s.begin(), s.begin() + firstDigit);
	size_t lastDigit = s.find_first_not_of(digits);
	number = s.substr(0, lastDigit);
	s.erase(s.begin(), s.begin() + lastDigit);
	return number;
}
void Analyzer::checkUnit(Unit &u)
{
	if (u.Vx * u.Vx + u.Vy * u.Vy != 1) //if vector is not normalized
	{
		u.Vx /= sqrt(u.Vx * u.Vx + u.Vy * u.Vy);
		u.Vy /= sqrt(u.Vx * u.Vx + u.Vy * u.Vy);
	}
}
void Analyzer::detectUnitsForOneUnit(int numberOfUnit)
{
	int numberOfUnitsAround = 0;
	for (int i = 0; i < _units.size(); i++)
		if (i != numberOfUnit)
		{
			double newRadius = sqrt(pow(_units[i].x - _units[numberOfUnit].x, 2) + pow(_units[i].y - _units[numberOfUnit].y, 2));
			if (newRadius <= _detectionRadius) //check if inside radius
			{
				double relativeAngle = acos((_units[numberOfUnit].Vx * (1.0 * _units[i].x - 1.0 * _units[numberOfUnit].x) + _units[numberOfUnit].Vy * (1.0 * _units[i].y - 1.0 * _units[numberOfUnit].y)) / newRadius);
				if (relativeAngle < _detectionAngle / 2 && relativeAngle > -1.0 * _detectionAngle / 2) //check if angle lay between two vectors lay in diaposon [ - _detectionAngle / 2, _detectionAngle / 2]
					numberOfUnitsAround++;
			}
		}
	_units[numberOfUnit].unitsNear = numberOfUnitsAround;
}
void Analyzer::detectionForUnits(int firstUnit, int lastUnit)
{
	for (int i = firstUnit; i < lastUnit; i++)
		detectUnitsForOneUnit(i);
}

bool Analyzer::avaliablePosition(int x, int y)
{
	for (int i = 0; i < _units.size(); i++)
		if (_units[i].x == x && _units[i].y == y)
			return false;
	return true;
}
void Analyzer::getMapSize(int &minX, int &maxX, int &minY, int &maxY)
{
	minX = _units[0].x;
	maxX = _units[0].x;
	minY = _units[0].y;
	maxY = _units[0].y;
	for (int i = 1; i < _units.size(); i++)
	{
		maxX = maxX > _units[i].x ? maxX : _units[i].x;
		minX = minX < _units[i].x ? minX : _units[i].x;
		maxY = maxY > _units[i].y ? maxY : _units[i].y;
		minY = minY < _units[i].y ? minY : _units[i].y;
	}
}

void GetInformation::printUnitCharacteristics()
{
	std::cout << "detection angle = " << _detectionAngle << ", detection radius = " << _detectionRadius << std::endl;
	for (int i = 0; i < _units.size(); i++)
		std::cout << _units[i].name << " позици€ (" << _units[i].x << "," << _units[i].y << "), направление (" << _units[i].Vx << "," << _units[i].Vy << ")\n";
}
void GetInformation::printMap()
{
	int minX = 0, maxX = 0, minY = 0, maxY = 0;
	getMapSize(minX, maxX, minY, maxY);
	std::cout << " арта: \n";
	for (int j = maxY + 1; j >= minY - 1; j--)
	{
		for (int i = minX - 1; i <= maxX + 1; i++)
		{
			if (avaliablePosition(i, j))
				std::cout << "0";
			else
				std::cout << "1";
		}
		std::cout << "\n";
	}
}
void GetInformation::printNumberOfUnitsNearTargets()
{
	for (int i = 0; i < _units.size(); i++)
		std::cout << _units[i].name << ": видит " << _units[i].unitsNear << std::endl;
}
void GetInformation::printResultInFile(std::string fileName)
{
	std::ofstream file;
	file.open(fileName);
	if (file.is_open())
	{
		for (int i = 0; i < _units.size(); i++)
			file << _units[i].name + ": видит " + std::to_string(_units[i].unitsNear) + "\n";
		file.close();
	}
	else
		throw "Program can not create results file.";
}
