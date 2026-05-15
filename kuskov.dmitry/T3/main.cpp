//Rects Sapm
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <algorithm>
#include <cmath>

struct Point
{
    int x;
    int y;

    bool operator==(const Point p)
    {
        if (p.x - x == p.y - y)
            return true;
        return false;
    }
    bool operator!=(const Point p)
    {
        return !(*this == p);
    }
    bool operator<(const Point& p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }
};

struct Polygon
{
    std::vector<Point> points;
    float centreX()
    {
        return (std::accumulate(points.begin(), points.end(), 0, [=](double acc, Point p){return (acc + p.x);})) / points.size();
    }

    float centreY()
    {
        return (std::accumulate(points.begin(), points.end(), 0, [=](double acc, Point p){return (acc + p.y);})) / points.size();
    }
};

struct SmthIO
{
    char exp;
};

struct LabelIO
{
    std::string exp;
};

struct cmdIO
{
    std::string exp;
    int n_;
    Polygon ppp_;
    bool poly_;
    bool num_;
    bool vv_;
};

std::istream& operator>>(std::istream& in, LabelIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    return in >> dest.exp;
}

std::istream& operator>>(std::istream& in, SmthIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c = '0';
    in >> c;
    if (in && c != dest.exp)
       in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Point& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    Point input{};
    in >> SmthIO{'('};
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    in >> input.x;
    in >> SmthIO{';'};
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    in >> input.y;
    in >> SmthIO{')'};
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (in)
    {
        dest = std::move(input);
        return in;
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& dest)
{
    std::istream::sentry sentry(in);
    if(!sentry)
        return in;
    Polygon input;
    std::size_t numPoint{0};
    in >> numPoint;
    if (numPoint < 3)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (in.get() != ' ')
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (in.peek() != '(')
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
    }
    for(std::size_t i{0}; i < numPoint; ++i)
    {
        Point a{0,0};
        in >> a;
        if (!in)
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        input.points.push_back(a);
        if (i != numPoint - 1)
        {
            if (in.peek() != ' ')
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            in.ignore(1);
            if (in.peek() == ' ')
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
        }
        else if (i == numPoint - 1)
        {
            if (in.peek() != '\n')
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            in.ignore(1);
        }
    }
    if (in)
    {
        dest = std::move(input);
        return in;
    }
    return in;
}

std::istream& operator>>(std::istream& in, cmdIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    cmdIO input{};
    LabelIO cmd{};
    in >> cmd;
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if(cmd.exp == "AREA" || cmd.exp == "COUNT")
    {
        if(in.get() != ' ')
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        if(in.peek() == ' ')
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        if(isdigit(in.peek()))
        {
            int o{0};
            in >> o;
            if (!in || (cmd.exp == "COUNT" && o < 3))
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            if (cmd.exp == "AREA" && o < 3)
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            input.n_ = o;
            input.exp = cmd.exp + " " + "num";
            input.num_ = true;
            if (cmd.exp == "COUNT")
                input.vv_ = true;
        }
        else
        {
            LabelIO cmd2{};
            in >> cmd2;
            if (!in || (cmd2.exp != "EVEN" && cmd2.exp != "ODD" && (cmd.exp != "AREA" && cmd2.exp == "MEAN")))
            {
                in.setstate(std::ios_base::failbit);
                return in;
            }
            input.exp = cmd.exp + " " + cmd2.exp;
            if (cmd.exp == "COUNT")
                input.vv_ = true;
        }
    }
    else if(cmd.exp == "MIN" || cmd.exp == "MAX")
    {
        LabelIO cmd2{};
        in >> cmd2;
        if (!in || (cmd2.exp != "AREA" && cmd2.exp != "VERTEXES"))
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        input.exp = cmd.exp + " " + cmd2.exp;
        if (cmd2.exp == "VERTEXES")
            input.vv_ = true;
    }
    else if(cmd.exp == "SAME")
    {
        in >> input.ppp_;
        if (!in)
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        input.exp = cmd.exp;
        input.poly_ = true;
        input.vv_ = true;
    }
    else if(cmd.exp == "RECTS")
    {
        if (!in)
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        input.exp = cmd.exp;
        input.vv_ = true;
    }
    else
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    dest = std::move(input);
    return in;
}

class iofmtguard
{
public:
    iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}

    ~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::ostream& operator<<(std::ostream& out, const Point& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
        return out;
    iofmtguard fmtguard(out);
    out << '(';
    out << src.x;
    out << ';';
    out << src.y;
    out << ')';
    return out;
}

std::ostream& operator<<(std::ostream& out, const Polygon& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
        return out;
    iofmtguard fmtguard(out);
    for(std::size_t i{0}; i < src.points.size(); ++i)
    {
        std::cout << src.points[i];
        if (i != src.points.size() - 1)
            std::cout << ' ';
    }
    return out;
}

bool comp(const Point& a, const Point& b, float x, float y)
{
    bool app = (a.y > y);
    bool bpp = (b.y > y);
    if (app != bpp)
        return app;
    float cross = (a.x - x) *  (b.y - y) - (a.y - y) * (b.x - x);
    return cross > 0;
}

float area(Polygon poly)
{
    if (poly.points.size() < 3)
        return 0.0;
    float cx = poly.centreX();
    float cy = poly.centreY();
    std::sort(poly.points.begin(), poly.points.end(), [cx, cy](const Point& a, const Point& b) -> bool {return comp(a, b, cx, cy);});
    float sum = std::accumulate(poly.points.begin(), poly.points.end(), 0.0,
            [&poly, i = 0](float acc, const Point& p) mutable
            {
                const Point& next = poly.points[(i + 1) % poly.points.size()];
                float term = p.x * next.y - next.x * p.y;
                ++i;
                return acc + term;
            });
    return static_cast<float>(std::abs(sum) * 0.5);
}

bool same(Polygon same11, Polygon same22)
{
    if (same11.points.size() != same22.points.size())
        return false;
    auto same1 = same11;
    auto same2 = same22;
    std::sort(same1.points.begin(), same1.points.end());
    std::sort(same2.points.begin(), same2.points.end());
    int n{0};
    if (same1.points[0] != same2.points[0])
        return false;
    else
        n = same1.points[0].x - same2.points[0].x;
    for (std::size_t i{0}; i < same1.points.size(); ++i)
    {
        if (same1.points[i] == same2.points[i])
        {
            if (same1.points[i].x - same2.points[i].x != n)
                return false;
        }
        else
            return false;
    }
    return true;
}

int dist(const Point& a, const Point& b)
{
    int x = a.x - b.x;
    int y = a.y - b.y;
    return x*x + y*y;
}

bool rect(const Polygon poly)
{
    if (poly.points.size() != 4)
        return false;
    std::vector<int> allDist;
    for (std::size_t i{0}; i < 4; ++i)
    {
        for (std::size_t j = i+1; j < 4; ++j)
        {
            allDist.push_back(dist(poly.points[i], poly.points[j]));
        }
    }
    std::sort(allDist.begin(), allDist.end());
    if (allDist[0] == allDist[1] && allDist[2] == allDist[3] && allDist[4] == allDist[5])
    {
        unsigned int side1 = allDist[0];
        unsigned int side2 = allDist[2];
        unsigned int diag = allDist[4];
        return side1 + side2 == diag;
    }
    return false;
}

int main(int argc, char* argv[])
{
    if (!argv[1] || argc != 2)
    {
        std::cout << "FILENAME REQUIRED";
        return 1;
    }
    std::ifstream ifs(argv[1]);
    std::vector<Polygon> v;
    if (ifs.is_open())
    {
        while(!ifs.eof())
        {
            std::copy
            (
                std::istream_iterator<Polygon>(ifs),
                std::istream_iterator<Polygon>(),
                std::back_inserter(v)
            );
            if (ifs.fail())
            {
                ifs.clear();
                ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    ifs.close();
    unsigned int n{0};
    Polygon ppp;
    std::unordered_map<std::string, std::function<float()>> table;
    table.insert(std::pair<std::string, std::function<float()>>("AREA EVEN", [&v](){return static_cast<float>(std::accumulate
                (
                    v.begin(),
                    v.end(),
                    0.0,
                    [](double acc, const Polygon& p){return acc + ((p.points.size()%2 == 0) ? area(p) : 0.0);}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("AREA ODD", [&v](){return static_cast<float>(std::accumulate
                (
                    v.begin(),
                    v.end(),
                    0.0,
                    [](double acc, const Polygon& p){return acc + ((p.points.size()%2 != 0) ? area(p) : 0.0);}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("AREA MEAN", [&v](){return static_cast<float>(std::accumulate
                (
                    v.begin(),
                    v.end(),
                    0.0,
                    [](double acc, const Polygon& p){return acc + area(p);}
                )/v.size());}));
    table.insert(std::pair<std::string, std::function<float()>>("AREA num", [&v, &n](){return static_cast<float>(std::accumulate
                (
                    v.begin(),
                    v.end(),
                    0.0,
                    [&n](double acc, const Polygon& p){return acc + ((p.points.size() == n) ? area(p) : 0.0);}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("MAX AREA", [&v](){return static_cast<float>(area(*std::max_element
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p, const Polygon& pp){return area(p) < area(pp);}
                )));}));
    table.insert(std::pair<std::string, std::function<float()>>("MAX VERTEXES", [&v](){return static_cast<float>(std::max_element
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p, const Polygon& pp){return p.points.size() < pp.points.size();}
                )->points.size());}));
    table.insert(std::pair<std::string, std::function<float()>>("MIN AREA", [&v](){return static_cast<float>(area(*std::min_element
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p, const Polygon& pp){return area(p) < area(pp);}
                )));}));
    table.insert(std::pair<std::string, std::function<float()>>("MIN VERTEXES", [&v](){return static_cast<float>(std::min_element
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p, const Polygon& pp){return p.points.size() < pp.points.size();}
                )->points.size());}));
    table.insert(std::pair<std::string, std::function<float()>>("COUNT EVEN", [&v](){return static_cast<float>(std::count_if
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p){return p.points.size()%2 == 0;}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("COUNT ODD", [&v](){return static_cast<float>(std::count_if
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p){return p.points.size()%2 != 0;}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("COUNT num", [&v, &n](){return static_cast<float>(std::count_if
                (
                    v.begin(),
                    v.end(),
                    [&n](const Polygon& p){return p.points.size() == n;}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("SAME", [&v, &ppp](){return static_cast<float>(std::count_if
                (
                    v.begin(),
                    v.end(),
                    [&ppp](const Polygon& p) {return same(p,ppp);}
                ));}));
    table.insert(std::pair<std::string, std::function<float()>>("RECTS", [&v](){return static_cast<float>(std::count_if
                (
                    v.begin(),
                    v.end(),
                    [](const Polygon& p) {return rect(p);}
                ));}));
    while (!std::cin.eof())
    {
        cmdIO command;
        std::cin >> command;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof())
                break;
            std::cout << "<INVALID COMMAND>" << '\n';
        }
        else
        {
            if (command.poly_ == true)
                ppp = command.ppp_;
            if (command.num_ == true)
            {
                n = command.n_;
            }
            if (!command.vv_)
            {
                if ((command.exp == "AREA MEAN" || command.exp.find("MAX") == 0 || command.exp.find("MIN") == 0) && v.empty())
                {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                std::cout << std::fixed << std::setprecision(1) << table.find(command.exp)->second() << '\n';
            }
            else
            {
                if ((command.exp == "AREA MEAN" || command.exp.find("MAX") == 0 || command.exp.find("MIN") == 0) && v.empty())
                {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                std::cout << static_cast<int>(table.find(command.exp))->second() << '\n';
            }
        }
    }
}
