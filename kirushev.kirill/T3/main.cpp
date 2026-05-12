#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <string>
#include <limits>

#include "Polygon.h"
#include "Commands.h"

// Variant №9 (RMECHO INTERSECTIONS)
int main(const int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: too few arguments\n";
        return 1;
    }
    if (argc > 2) {
        std::cerr << "Error: too many arguments\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: could not open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    while (!inputFile.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(inputFile),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
            );

        if (inputFile.fail() && !inputFile.eof()) {
            inputFile.clear();
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.clear();
    std::string command;

    while (std::cin >> command) {
        try {
            if (command == "AREA") {
                std::string arg;
                std::cin >> arg;
                area(polygons, arg, std::cout);
            } else if (command == "MAX") {
                std::string arg;
                std::cin >> arg;
                max(polygons, arg, std::cout);
            } else if (command == "MIN") {
                std::string arg;
                std::cin >> arg;
                min(polygons, arg, std::cout);
            } else if (command == "COUNT") {
                std::string arg;
                std::cin >> arg;
                count(polygons, arg, std::cout);
            } else if (command == "RMECHO") {
                Polygon polygon;
                if (!(std::cin >> polygon)) {
                    throw std::invalid_argument("");
                }
                rmecho(polygons, polygon, std::cout);
            } else if (command == "INTERSECTIONS") {
                Polygon polygon;
                if (!(std::cin >> polygon)) {
                    throw std::invalid_argument("");
                }
                intersections(polygons, polygon, std::cout);
            } else {
                throw std::invalid_argument("");
            }
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}
