#ifndef COMMANDS_H
#define COMMANDS_H
#include "Polygon.h"
#include <vector>
#include <string>
#include <iostream>

bool isOrthogonal(const Point& a, const Point& b, const Point& c);
bool rightAngle(const Polygon& poly);
void rightshapes(const std::vector<Polygon>& polygon, std::ostream& out);

void echo(std::vector<Polygon>& polygon, std::ostream& out);
void min(const std::vector<Polygon>& polygon, std::string name, std::ostream& out);
void max(const std::vector<Polygon>& polygon, std::string name, std::ostream& out);
void area(const std::vector<Polygon>& polygon, std::string name, std::ostream& out);
void count(const std::vector<Polygon>& polygon, std::string name, std::ostream& out);

#endif // !
