#include <iostream>
#include <thread>
#include <fstream>
#include "Unit.h"

int main()
{
    setlocale(LC_CTYPE, "Russian");
    GetInformation inf;
    try
    {
        std::string fileName;
        std::cout << "Input file name: ";
        std::cin >> fileName;
        inf.readFile(fileName);
        std::vector<std::thread> vectorOfThread;
        std::thread thr1([&]()
            {
                inf.detectionForUnits(0, inf.getMaximumUnits() / 2);
            });
        std::thread thr2([&]()
            {
                inf.detectionForUnits(inf.getMaximumUnits() / 2, inf.getMaximumUnits());
            });
        thr1.join();
        thr2.join();
        int n;
        bool exit = false;
        std::cout << "1 - writing the results to a file \n2 - writing the results to a console \n3 - printing the map to a console \n0 - exit\n";
        do
        {
            std::cout << "Input number:";
            std::cin >> n;
            switch (n)
            {
                case 0:
                    exit = true;
                    break;
                case 1:
                    inf.printResultInFile("results.txt");
                    break;
                case 2:
                    std::cout << "Результат: \n";
                    inf.printNumberOfUnitsNearTargets();
                    break;
                case 3:
                    inf.printMap();
                    break;
                default:
                    std::cout << "Wrong choice!\n";
            }
        } while (!exit);
    }
    catch (const char* m)
    {
        std::cout << m;
    }
}