#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <string>
#include <iostream>
#include "Polygon.h"

void area(std::vector<Polygon> polygons, std::string parameter, std::ostream& out);
void max(std::vector<Polygon> polygons, std::string parameter, std::ostream& out);
void min(std::vector<Polygon> polygons, std::string parameter, std::ostream& out);
void ccount(std::vector<Polygon> polygons, std::string parameter, std::ostream& out);
void rmecho(std::vector<Polygon> polygons, const Polygon& polygon, std::ostream& out);
void same(std::vector<Polygon> polygons, const Polygon& polygon, std::ostream& out);

#endif
