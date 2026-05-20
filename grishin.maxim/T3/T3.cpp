#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include "Polygon.h"
#include "Commands.h"

// 7. RMECHO SAME

int main(int count, char* filename[])
{
    if (count < 2) {
        std::cout << "ERROR: Not enough parameters." << std::endl;
        return 1;
    }
    std::ifstream fileIn(filename[1]);
    if (!fileIn) {
        std::cout << "ERROR: Can't open file." << std::endl;
        return 1;
    }

    std::vector<Polygon> polygons;

    while (!fileIn.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(fileIn),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );
        if (fileIn.fail()) {
            fileIn.clear();
            fileIn.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.clear();
    std::string command;

    while (std::cin >> command) {

        std::cout << std::fixed << std::setprecision(1);
        try {
            if (command == "AREA") {
                std::string argument;
                std::cin >> argument;
                area(polygons, argument, std::cout);
            }
            else if (command == "MAX") {
                std::string argument;
                std::cin >> argument;
                max(polygons, argument, std::cout);
            }
            else if (command == "MIN") {
                std::string argument;
                std::cin >> argument;
                min(polygons, argument, std::cout);
            }
            else if (command == "COUNT") {
                std::string argument;
                std::cin >> argument;
                ccount(polygons, argument, std::cout);
            }
            else if (command == "RMECHO") {
                Polygon polygon;
                std::cin >> polygon;
                rmecho(polygons, polygon, std::cout);
            }
            else if (command == "SAME") {
                Polygon polygon;
                std::cin >> polygon;
                same(polygons, polygon, std::cout);
            }
            else {
                throw std::invalid_argument("ERROR: Unknown command.");
            }
        }
        catch (const std::exception& e) {
            std::cout << e.what();
            return 1;
        }
    }
    return 0;
}
