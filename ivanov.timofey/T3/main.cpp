#include "Polygon.h"
#include "Commands.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        std::cerr << "ERROR: fail open file" << std::endl;
        return 1;
    }

    std::vector<Polygon> polygons;
    while (!inputFile.eof())
    {
        std::copy(
            std::istream_iterator<Polygon>(inputFile),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );
        if (inputFile.fail() && !inputFile.eof())
        {
            inputFile.clear();
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string cmd;
    while (std::cin >> cmd)
    {
        try
        {
            if (cmd == "AREA")
            {
                std::string arg;
                std::cin >> arg;
                area(polygons, arg, std::cout);
            }
            else if (cmd == "MAX")
            {
                std::string arg;
                std::cin >> arg;
                max(polygons, arg, std::cout);
            }
            else if (cmd == "MIN")
            {
                std::string arg;
                std::cin >> arg;
                min(polygons, arg, std::cout);
            }
            else if (cmd == "COUNT")
            {
                std::string arg;
                std::cin >> arg;
                count(polygons, arg, std::cout);
            }
            else if (cmd == "PERMS")
            {
                perms(polygons, std::cin, std::cout);
            }
            else if (cmd == "RIGHTSHAPES")
            {
                rightshapes(polygons, std::cout);
            }
            else
            {
                throw std::invalid_argument("");
            }
        }
        catch (...)
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
