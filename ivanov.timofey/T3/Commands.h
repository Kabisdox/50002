#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>
#include <string>
#include "Polygon.h"

void area(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out);
void max(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out);
void min(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out);
void count(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out);
void perms(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out);
void rightshapes(const std::vector<Polygon>& polygons, std::ostream& out);

#endif
