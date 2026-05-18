#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector< Point > points;
};

struct Line
{
    std::string value;
};

const char* INVALID_COMMAND = "<INVALID COMMAND>";

std::istream& operator>>(std::istream& in, Line& line)
{
    std::getline(in, line.value);
    return in;
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator<(const Point& lhs, const Point& rhs)
{
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
    return lhs.points == rhs.points;
}

std::istream& operator>>(std::istream& in, Point& point)
{
    char left = 0;
    char semicolon = 0;
    char right = 0;
    int x = 0;
    int y = 0;

    if ((in >> left) && left == '(' && (in >> x) &&
        (in >> semicolon) && semicolon == ';' &&
        (in >> y) && (in >> right) && right == ')')
    {
        point.x = x;
        point.y = y;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

bool readPoints(std::istream& in, std::vector< Point >& points, std::size_t count)
{
    if (count == 0)
    {
        return true;
    }
    Point point;
    if (!(in >> point))
    {
        return false;
    }
    points.push_back(point);
    return readPoints(in, points, count - 1);
}

bool readPolygon(std::istream& in, Polygon& polygon)
{
    long long count = 0;
    if (!(in >> count) || count < 3)
    {
        return false;
    }

    Polygon temp;
    temp.points.reserve(static_cast<std::size_t>(count));
    if (!readPoints(in, temp.points, static_cast<std::size_t>(count)))
    {
        return false;
    }

    in >> std::ws;
    if (!in.eof())
    {
        return false;
    }

    polygon = temp;
    return true;
}

bool readPolygon(const std::string& text, Polygon& polygon)
{
    std::istringstream in(text);
    return readPolygon(in, polygon);
}

bool isPolygonLine(const Line& line)
{
    Polygon polygon;
    return readPolygon(line.value, polygon);
}

Polygon makePolygon(const Line& line)
{
    Polygon polygon;
    readPolygon(line.value, polygon);
    return polygon;
}

long long getCross(const Point& lhs, const Point& rhs)
{
    return static_cast<long long>(lhs.x) * rhs.y - static_cast<long long>(lhs.y) * rhs.x;
}

long long getAbsLongLong(long long value)
{
    return value < 0 ? -value : value;
}

double getArea(const Polygon& polygon)
{
    if (polygon.points.size() < 3)
    {
        return 0.0;
    }

    std::vector< Point > shifted(polygon.points.size());
    std::rotate_copy(
        polygon.points.cbegin(),
        std::next(polygon.points.cbegin()),
        polygon.points.cend(),
        shifted.begin()
    );

    long long doubleArea = std::inner_product(
        polygon.points.cbegin(),
        polygon.points.cend(),
        shifted.cbegin(),
        0LL,
        std::plus< long long >(),
        getCross
    );

    return getAbsLongLong(doubleArea) / 2.0;
}

std::size_t getVertexCount(const Polygon& polygon)
{
    return polygon.points.size();
}

bool isEvenVertexCount(const Polygon& polygon)
{
    return getVertexCount(polygon) % 2 == 0;
}

bool isOddVertexCount(const Polygon& polygon)
{
    return getVertexCount(polygon) % 2 != 0;
}

double addArea(double result, const Polygon& polygon)
{
    return result + getArea(polygon);
}

template< typename Predicate >
double getAreaSumIf(const std::vector< Polygon >& polygons, Predicate predicate)
{
    std::vector< Polygon > filtered;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtered), predicate);
    return std::accumulate(filtered.cbegin(), filtered.cend(), 0.0, addArea);
}

double getAreaSum(const std::vector< Polygon >& polygons)
{
    return std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, addArea);
}

std::string formatDouble(double value)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << value;
    return out.str();
}

bool isFinished(std::istream& in)
{
    in >> std::ws;
    return in.eof();
}

bool readVertexNumber(const std::string& text, std::size_t& value)
{
    long long number = 0;
    char extra = 0;
    std::istringstream in(text);
    if (!(in >> number) || (in >> extra) || number < 3)
    {
        return false;
    }
    value = static_cast<std::size_t>(number);
    return true;
}

Point subtractPoint(const Point& point, const Point& base)
{
    Point result;
    result.x = point.x - base.x;
    result.y = point.y - base.y;
    return result;
}

std::vector< Point > getNormalizedPoints(const Polygon& polygon)
{
    using namespace std::placeholders;

    std::vector< Point > result(polygon.points.size());
    Point base = *std::min_element(polygon.points.cbegin(), polygon.points.cend());
    std::transform(
        polygon.points.cbegin(),
        polygon.points.cend(),
        result.begin(),
        std::bind(subtractPoint, _1, base)
    );
    std::sort(result.begin(), result.end());
    return result;
}

bool isSameByMoving(const Polygon& sample, const Polygon& polygon)
{
    return getVertexCount(sample) == getVertexCount(polygon) &&
        getNormalizedPoints(sample) == getNormalizedPoints(polygon);
}

bool hasLessAreaThan(const Polygon& sample, const Polygon& polygon)
{
    return getArea(polygon) < getArea(sample);
}

std::string processArea(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(in >> parameter) || !isFinished(in))
    {
        return INVALID_COMMAND;
    }

    if (parameter == "ODD")
    {
        return formatDouble(getAreaSumIf(polygons, isOddVertexCount));
    }
    if (parameter == "EVEN")
    {
        return formatDouble(getAreaSumIf(polygons, isEvenVertexCount));
    }
    if (parameter == "MEAN")
    {
        if (polygons.empty())
        {
            return INVALID_COMMAND;
        }
        return formatDouble(getAreaSum(polygons) / polygons.size());
    }

    std::size_t vertexCount = 0;
    if (!readVertexNumber(parameter, vertexCount))
    {
        return INVALID_COMMAND;
    }

    return formatDouble(getAreaSumIf(
        polygons,
        std::bind(std::equal_to< std::size_t >(), std::bind(getVertexCount, _1), vertexCount)
    ));
}

std::string processMax(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(in >> parameter) || !isFinished(in) || polygons.empty())
    {
        return INVALID_COMMAND;
    }

    if (parameter == "AREA")
    {
        std::vector< Polygon >::const_iterator it = std::max_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(std::less< double >(), std::bind(getArea, _1), std::bind(getArea, _2))
        );
        return formatDouble(getArea(*it));
    }
    if (parameter == "VERTEXES")
    {
        std::vector< Polygon >::const_iterator it = std::max_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(std::less< std::size_t >(), std::bind(getVertexCount, _1), std::bind(getVertexCount, _2))
        );
        return std::to_string(getVertexCount(*it));
    }

    return INVALID_COMMAND;
}

std::string processMin(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(in >> parameter) || !isFinished(in) || polygons.empty())
    {
        return INVALID_COMMAND;
    }

    if (parameter == "AREA")
    {
        std::vector< Polygon >::const_iterator it = std::min_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(std::less< double >(), std::bind(getArea, _1), std::bind(getArea, _2))
        );
        return formatDouble(getArea(*it));
    }
    if (parameter == "VERTEXES")
    {
        std::vector< Polygon >::const_iterator it = std::min_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(std::less< std::size_t >(), std::bind(getVertexCount, _1), std::bind(getVertexCount, _2))
        );
        return std::to_string(getVertexCount(*it));
    }

    return INVALID_COMMAND;
}

std::string processCount(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(in >> parameter) || !isFinished(in))
    {
        return INVALID_COMMAND;
    }

    if (parameter == "ODD")
    {
        return std::to_string(std::count_if(polygons.cbegin(), polygons.cend(), isOddVertexCount));
    }
    if (parameter == "EVEN")
    {
        return std::to_string(std::count_if(polygons.cbegin(), polygons.cend(), isEvenVertexCount));
    }

    std::size_t vertexCount = 0;
    if (!readVertexNumber(parameter, vertexCount))
    {
        return INVALID_COMMAND;
    }

    return std::to_string(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(std::equal_to< std::size_t >(), std::bind(getVertexCount, _1), vertexCount)
    ));
}

std::string processLessArea(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    Polygon sample;
    if (!readPolygon(in, sample))
    {
        return INVALID_COMMAND;
    }

    return std::to_string(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(hasLessAreaThan, std::cref(sample), _1)
    ));
}

std::string processSame(const std::vector< Polygon >& polygons, std::istream& in)
{
    using namespace std::placeholders;

    Polygon sample;
    if (!readPolygon(in, sample))
    {
        return INVALID_COMMAND;
    }

    return std::to_string(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(isSameByMoving, std::cref(sample), _1)
    ));
}

std::string processCommand(const std::vector< Polygon >& polygons, const Line& line)
{
    std::istringstream in(line.value);
    std::string command;
    if (!(in >> command))
    {
        return INVALID_COMMAND;
    }

    if (command == "AREA")
    {
        return processArea(polygons, in);
    }
    if (command == "MAX")
    {
        return processMax(polygons, in);
    }
    if (command == "MIN")
    {
        return processMin(polygons, in);
    }
    if (command == "COUNT")
    {
        return processCount(polygons, in);
    }
    if (command == "LESSAREA")
    {
        return processLessArea(polygons, in);
    }
    if (command == "SAME")
    {
        return processSame(polygons, in);
    }

    return INVALID_COMMAND;
}

std::vector< Polygon > readPolygons(std::istream& in)
{
    std::vector< Line > lines;
    std::vector< Line > goodLines;
    std::vector< Polygon > polygons;

    std::copy(
        std::istream_iterator< Line >(in),
        std::istream_iterator< Line >(),
        std::back_inserter(lines)
    );
    std::copy_if(lines.cbegin(), lines.cend(), std::back_inserter(goodLines), isPolygonLine);
    std::transform(goodLines.cbegin(), goodLines.cend(), std::back_inserter(polygons), makePolygon);

    return polygons;
}

int main(int argc, char* argv[])
{
    using namespace std::placeholders;

    if (argc != 2)
    {
        std::cerr << "Error: filename is not specified\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input)
    {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }

    std::vector< Polygon > polygons = readPolygons(input);

    std::transform(
        std::istream_iterator< Line >(std::cin),
        std::istream_iterator< Line >(),
        std::ostream_iterator< std::string >(std::cout, "\n"),
        std::bind(processCommand, std::cref(polygons), _1)
    );

    return 0;
}
