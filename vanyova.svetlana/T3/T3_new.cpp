#include <iostream>
#include <string>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>
#include <vector>
#include <fstream>
#include <limits>
#include "Polygon.h"
#include "commands.h"

int main(int count, char** file)
{
    if (count < 2) {
        std::cout << "ERROR: less arguments";
        return 1;
    }
    std::ifstream inputf(file[1]);
    if (!inputf) {
        std::cout << "ERROR: fail open file";
        return 1;
    }
    std::vector<Polygon> polygons;
    while (!inputf.eof())
    {
        std::copy(
            std::istream_iterator< Polygon >(inputf),
            std::istream_iterator< Polygon >(),
            std::back_inserter(polygons)
        );
        if (inputf.fail() && !inputf.eof())
        {
            inputf.clear();
            inputf.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }
    std::cin.clear();
    std::string cmd;
    while (std::cin >> cmd) {
        try {
            if (cmd == "AREA") {
                std::string arg;
                std::cin >> arg;
                area(polygons, arg, std::cout);
            }
            else if (cmd == "MAX") {
                std::string arg;
                std::cin >> arg;
                max(polygons, arg, std::cout);
            }
            else if (cmd == "MIN") {
                std::string arg;
                std::cin >> arg;
                min(polygons, arg, std::cout);
            }
            else if (cmd == "COUNT") {
                std::string arg;
                std::cin >> arg;
                ::count(polygons, arg, std::cout);
            }
            else if (cmd == "ECHO") {
                echo(polygons, std::cout);
            }
            else if (cmd == "RIGHTSHAPES") {
                rightshapes(polygons, std::cout);
            }
            else {
                throw std::invalid_argument("");
            }
        }
        catch (...) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }
    }
}
