#include "Commands.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

void area(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "<INVALID COMMAND>" << std::endl;
        return;
    }

    if (param == "MEAN")
    {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double s, const Polygon& p) { return s + p.area(); });
        out << std::fixed << std::setprecision(1) << sum / polygons.size() << std::endl;
    }
    else if (param == "EVEN")
    {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double s, const Polygon& p) {
                return s + ((p.points.size() % 2 == 0) ? p.area() : 0.0);
            });
        out << std::fixed << std::setprecision(1) << sum << std::endl;
    }
    else if (param == "ODD")
    {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double s, const Polygon& p) {
                return s + ((p.points.size() % 2 == 1) ? p.area() : 0.0);
            });
        out << std::fixed << std::setprecision(1) << sum << std::endl;
    }
    else
    {
        size_t n = std::stoul(param);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [n](double s, const Polygon& p) {
                return s + ((p.points.size() == n) ? p.area() : 0.0);
            });
        out << std::fixed << std::setprecision(1) << sum << std::endl;
    }
}

void max(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "<INVALID COMMAND>" << std::endl;
        return;
    }

    if (param == "AREA")
    {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.area() < b.area(); });
        out << std::fixed << std::setprecision(1) << it->area() << std::endl;
    }
    else if (param == "VERTEXES")
    {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
        out << it->points.size() << std::endl;
    }
    else
    {
        out << "<INVALID COMMAND>" << std::endl;
    }
}

void min(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "<INVALID COMMAND>" << std::endl;
        return;
    }

    if (param == "AREA")
    {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.area() < b.area(); });
        out << std::fixed << std::setprecision(1) << it->area() << std::endl;
    }
    else if (param == "VERTEXES")
    {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
        out << it->points.size() << std::endl;
    }
    else
    {
        out << "<INVALID COMMAND>" << std::endl;
    }
}

void count(const std::vector<Polygon>& polygons, const std::string& param, std::ostream& out)
{
    if (param == "EVEN")
    {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) { return p.points.size() % 2 == 0; }) << std::endl;
    }
    else if (param == "ODD")
    {
        out << std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p) { return p.points.size() % 2 == 1; }) << std::endl;
    }
    else
    {
        size_t n = std::stoul(param);
        out << std::count_if(polygons.begin(), polygons.end(),
            [n](const Polygon& p) { return p.points.size() == n; }) << std::endl;
    }
}

void perms(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target))
    {
        out << "<INVALID COMMAND>" << std::endl;
        return;
    }

    out << std::count_if(polygons.begin(), polygons.end(),
        [&target](const Polygon& p) { return p.isPermutationOf(target); }) << std::endl;
}

void rightshapes(const std::vector<Polygon>& polygons, std::ostream& out)
{
    out << std::count_if(polygons.begin(), polygons.end(),
        [](const Polygon& p) { return p.hasRightAngle(); }) << std::endl;
}
