#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>
#include <string>
#include "Polygon.h"

void area(const std::vector<Polygon>&, const std::string &, std::ostream&);
void min(const std::vector<Polygon>&, const std::string &, std::ostream&);
void max(const std::vector<Polygon>&, const std::string &, std::ostream&);
void count(const std::vector<Polygon>&, const std::string &, std::ostream&);

void rmecho(std::vector<Polygon>&, const Polygon&, std::ostream&);
void intersections(const std::vector<Polygon>&, const Polygon&, std::ostream&);

#endif //COMMANDS_H
