#include <algorithm>
#include <cmath>
#include <cctype>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
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
    std::string text;
};

struct ParsedPolygon
{
    Polygon polygon;
    bool isCorrect;
};

bool operator==(const Point& left, const Point& right)
{
    return left.x == right.x && left.y == right.y;
}

bool operator==(const Polygon& left, const Polygon& right)
{
    return left.points == right.points;
}

bool readExact(std::istream& in, char expected)
{
    char symbol = 0;
    if (in.get(symbol) && symbol == expected)
    {
        return true;
    }
    in.setstate(std::ios::failbit);
    return false;
}

bool isDigitChar(char symbol)
{
    return std::isdigit(static_cast<unsigned char>(symbol)) != 0;
}

bool isNextDigit(std::istream& in)
{
    const int next = in.peek();
    return next != std::char_traits< char >::eof() &&
        isDigitChar(static_cast<char>(next));
}

std::istream& operator>>(std::istream& in, Line& line)
{
    if (!std::getline(in, line.text))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Point& point)
{
    const std::istream::fmtflags flags = in.flags();
    in >> std::noskipws;

    int x = 0;
    int y = 0;
    const bool isCorrect =
        readExact(in, '(') &&
        (in >> x) &&
        readExact(in, ';') &&
        (in >> y) &&
        readExact(in, ')');

    if (isCorrect)
    {
        point.x = x;
        point.y = y;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    in.flags(flags);
    return in;
}

struct PointReader
{
    explicit PointReader(std::istream& input) :
        input_(&input),
        isCorrect_(true)
    {
    }

    Point operator()()
    {
        Point point{ 0, 0 };
        if (!readExact(*input_, ' ') || !(*input_ >> point))
        {
            isCorrect_ = false;
        }
        return point;
    }

    bool isCorrect() const
    {
        return isCorrect_;
    }

private:
    std::istream* input_;
    bool isCorrect_;
};

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    const std::istream::fmtflags flags = in.flags();
    in >> std::noskipws;

    std::size_t count = 0;
    if (!isNextDigit(in) || !(in >> count) || count < 3)
    {
        in.setstate(std::ios::failbit);
        in.flags(flags);
        return in;
    }

    std::vector< Point > points(count);
    PointReader reader(in);
    std::generate(points.begin(), points.end(), std::ref(reader));

    if (!reader.isCorrect())
    {
        in.setstate(std::ios::failbit);
        in.flags(flags);
        return in;
    }

    polygon.points = points;
    in.flags(flags);
    return in;
}

bool isOnlySpacesToEnd(std::istream& in)
{
    in >> std::ws;
    return in.eof();
}

bool readPolygonFromLine(const std::string& line, Polygon& polygon)
{
    std::istringstream input(line);
    Polygon temp;
    if (!(input >> temp) || !isOnlySpacesToEnd(input))
    {
        return false;
    }
    polygon = temp;
    return true;
}

ParsedPolygon parsePolygonLine(const Line& line)
{
    ParsedPolygon result{ Polygon(), false };
    result.isCorrect = readPolygonFromLine(line.text, result.polygon);
    return result;
}

bool isParsedPolygonInvalid(const ParsedPolygon& polygon)
{
    return !polygon.isCorrect;
}

Polygon getParsedPolygon(const ParsedPolygon& polygon)
{
    return polygon.polygon;
}

std::vector< Polygon > readPolygons(std::istream& input)
{
    std::vector< Line > lines{
        std::istream_iterator< Line >(input),
        std::istream_iterator< Line >()
    };

    std::vector< ParsedPolygon > parsed(lines.size());
    std::transform(lines.cbegin(), lines.cend(), parsed.begin(), parsePolygonLine);

    parsed.erase(
        std::remove_if(parsed.begin(), parsed.end(), isParsedPolygonInvalid),
        parsed.end());

    std::vector< Polygon > polygons(parsed.size());
    std::transform(
        parsed.cbegin(),
        parsed.cend(),
        polygons.begin(),
        getParsedPolygon);

    return polygons;
}

std::size_t getVertexCount(const Polygon& polygon)
{
    return polygon.points.size();
}

int getX(const Point& point)
{
    return point.x;
}

int getY(const Point& point)
{
    return point.y;
}

double getCrossProduct(const Point& left, const Point& right)
{
    return static_cast<double>(left.x) * right.y -
        static_cast<double>(left.y) * right.x;
}

double getArea(const Polygon& polygon)
{
    const std::vector< Point >& points = polygon.points;

    double sum = std::inner_product(
        points.cbegin(),
        std::prev(points.cend()),
        std::next(points.cbegin()),
        0.0,
        std::plus< double >(),
        getCrossProduct);

    sum += getCrossProduct(points.back(), points.front());
    return std::abs(sum) / 2.0;
}

double addArea(double sum, const Polygon& polygon)
{
    return sum + getArea(polygon);
}

double addAreaIfEven(double sum, const Polygon& polygon, bool needEven)
{
    const bool isEven = getVertexCount(polygon) % 2 == 0;
    return (isEven == needEven) ? (sum + getArea(polygon)) : sum;
}

double addAreaIfVertexCount(
    double sum,
    const Polygon& polygon,
    std::size_t count)
{
    return (getVertexCount(polygon) == count) ? (sum + getArea(polygon)) : sum;
}

bool readSize(const std::string& text, std::size_t& value)
{
    if (text.empty() ||
        !std::all_of(text.cbegin(), text.cend(), isDigitChar))
    {
        return false;
    }

    std::istringstream input(text);
    input >> value;
    return !input.fail();
}

std::string toString(double value)
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(1) << value;
    return output.str();
}

std::string toString(std::size_t value)
{
    std::ostringstream output;
    output << value;
    return output.str();
}

bool isPointLess(const Point& left, const Point& right)
{
    return std::tie(left.x, left.y) < std::tie(right.x, right.y);
}

Point shiftPoint(const Point& point, int minX, int minY)
{
    return Point{ point.x - minX, point.y - minY };
}

std::vector< Point > getNormalPoints(const Polygon& polygon)
{
    using namespace std::placeholders;

    std::vector< Point > result(polygon.points.size());

    const auto minX = std::min_element(
        polygon.points.cbegin(),
        polygon.points.cend(),
        std::bind(
            std::less< int >(),
            std::bind(getX, _1),
            std::bind(getX, _2)));

    const auto minY = std::min_element(
        polygon.points.cbegin(),
        polygon.points.cend(),
        std::bind(
            std::less< int >(),
            std::bind(getY, _1),
            std::bind(getY, _2)));

    std::transform(
        polygon.points.cbegin(),
        polygon.points.cend(),
        result.begin(),
        std::bind(shiftPoint, _1, minX->x, minY->y));

    std::sort(result.begin(), result.end(), isPointLess);
    return result;
}

bool isSameByMove(const Polygon& left, const Polygon& right)
{
    if (getVertexCount(left) != getVertexCount(right))
    {
        return false;
    }
    return getNormalPoints(left) == getNormalPoints(right);
}

bool makeAreaResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(input >> parameter) || !isOnlySpacesToEnd(input))
    {
        return false;
    }

    if (parameter == "EVEN")
    {
        result = toString(std::accumulate(
            polygons.cbegin(),
            polygons.cend(),
            0.0,
            std::bind(addAreaIfEven, _1, _2, true)));
        return true;
    }

    if (parameter == "ODD")
    {
        result = toString(std::accumulate(
            polygons.cbegin(),
            polygons.cend(),
            0.0,
            std::bind(addAreaIfEven, _1, _2, false)));
        return true;
    }

    if (parameter == "MEAN")
    {
        if (polygons.empty())
        {
            return false;
        }

        const double sum =
            std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, addArea);
        result = toString(sum / polygons.size());
        return true;
    }

    std::size_t count = 0;
    if (!readSize(parameter, count) || count < 3)
    {
        return false;
    }

    result = toString(std::accumulate(
        polygons.cbegin(),
        polygons.cend(),
        0.0,
        std::bind(addAreaIfVertexCount, _1, _2, count)));

    return true;
}

bool makeMaxResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(input >> parameter) || !isOnlySpacesToEnd(input) || polygons.empty())
    {
        return false;
    }

    if (parameter == "AREA")
    {
        const auto iter = std::max_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::less< double >(),
                std::bind(getArea, _1),
                std::bind(getArea, _2)));

        result = toString(getArea(*iter));
        return true;
    }

    if (parameter == "VERTEXES")
    {
        const auto iter = std::max_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::less< std::size_t >(),
                std::bind(getVertexCount, _1),
                std::bind(getVertexCount, _2)));

        result = toString(getVertexCount(*iter));
        return true;
    }

    return false;
}

bool makeMinResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(input >> parameter) || !isOnlySpacesToEnd(input) || polygons.empty())
    {
        return false;
    }

    if (parameter == "AREA")
    {
        const auto iter = std::min_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::less< double >(),
                std::bind(getArea, _1),
                std::bind(getArea, _2)));

        result = toString(getArea(*iter));
        return true;
    }

    if (parameter == "VERTEXES")
    {
        const auto iter = std::min_element(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::less< std::size_t >(),
                std::bind(getVertexCount, _1),
                std::bind(getVertexCount, _2)));

        result = toString(getVertexCount(*iter));
        return true;
    }

    return false;
}

bool makeCountResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    std::string parameter;
    if (!(input >> parameter) || !isOnlySpacesToEnd(input))
    {
        return false;
    }

    if (parameter == "EVEN")
    {
        result = toString(static_cast<std::size_t>(std::count_if(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::equal_to< std::size_t >(),
                std::bind(
                    std::modulus< std::size_t >(),
                    std::bind(getVertexCount, _1),
                    2),
                0))));
        return true;
    }

    if (parameter == "ODD")
    {
        result = toString(static_cast<std::size_t>(std::count_if(
            polygons.cbegin(),
            polygons.cend(),
            std::bind(
                std::equal_to< std::size_t >(),
                std::bind(
                    std::modulus< std::size_t >(),
                    std::bind(getVertexCount, _1),
                    2),
                1))));
        return true;
    }

    std::size_t count = 0;
    if (!readSize(parameter, count) || count < 3)
    {
        return false;
    }

    result = toString(static_cast<std::size_t>(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(
            std::equal_to< std::size_t >(),
            std::bind(getVertexCount, _1),
            count))));

    return true;
}

bool makeLessAreaResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    Polygon polygon;
    if (!readExact(input, ' ') || !(input >> polygon) || !isOnlySpacesToEnd(input))
    {
        return false;
    }

    const double area = getArea(polygon);
    result = toString(static_cast<std::size_t>(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(std::less< double >(), std::bind(getArea, _1), area))));

    return true;
}

bool makeSameResult(
    const std::vector< Polygon >& polygons,
    std::istream& input,
    std::string& result)
{
    using namespace std::placeholders;

    Polygon polygon;
    if (!readExact(input, ' ') || !(input >> polygon) || !isOnlySpacesToEnd(input))
    {
        return false;
    }

    result = toString(static_cast<std::size_t>(std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        std::bind(isSameByMove, _1, polygon))));

    return true;
}

std::string executeCommand(
    const Line& line,
    const std::vector< Polygon >& polygons)
{
    std::istringstream input(line.text);
    std::string command;
    std::string result;
    bool isCorrect = false;

    if (!(input >> command))
    {
        return "<INVALID COMMAND>";
    }

    if (command == "AREA")
    {
        isCorrect = makeAreaResult(polygons, input, result);
    }
    else if (command == "MAX")
    {
        isCorrect = makeMaxResult(polygons, input, result);
    }
    else if (command == "MIN")
    {
        isCorrect = makeMinResult(polygons, input, result);
    }
    else if (command == "COUNT")
    {
        isCorrect = makeCountResult(polygons, input, result);
    }
    else if (command == "LESSAREA")
    {
        isCorrect = makeLessAreaResult(polygons, input, result);
    }
    else if (command == "SAME")
    {
        isCorrect = makeSameResult(polygons, input, result);
    }

    return isCorrect ? result : "<INVALID COMMAND>";
}

void processCommands(const std::vector< Polygon >& polygons)
{
    using namespace std::placeholders;

    std::transform(
        std::istream_iterator< Line >(std::cin),
        std::istream_iterator< Line >(),
        std::ostream_iterator< std::string >(std::cout, "\n"),
        std::bind(executeCommand, _1, std::cref(polygons)));
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: filename is not set\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input)
    {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }

    const std::vector< Polygon > polygons = readPolygons(input);
    processCommands(polygons);

    return 0;
}
