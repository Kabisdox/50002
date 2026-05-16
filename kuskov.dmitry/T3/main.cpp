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
#include "envy.hpp"

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

    using type = std::function<float(const std::vector<Polygon>&, unsigned int n, Polygon p)>;

    std::unordered_map<std::string, type> table;
    table.insert(std::pair<std::string, type>("AREA EVEN",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return AREA(v, aEVEN);}));
    table.insert(std::pair<std::string, type>("AREA ODD",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return AREA(v, aODD);}));
    table.insert(std::pair<std::string, type>("AREA MEAN",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return AREA(v, aMEAN);}));
    table.insert(std::pair<std::string, type>("AREA num",
                [](const std::vector<Polygon>& v, unsigned int m, Polygon){return AREA(v,
                    [&m](double acc, const Polygon& p){return acc + ((p.points.size() == m) ? area(p) : 0.0);});}));
    table.insert(std::pair<std::string, type>("MAX AREA",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return area(*MAX(v, mAREA));}));
    table.insert(std::pair<std::string, type>("MIN AREA",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return area(*MIN(v, mAREA));}));
    table.insert(std::pair<std::string, type>("MAX VERTEXES",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return MAX(v, mAREA)->points.size();}));
    table.insert(std::pair<std::string, type>("MIN VERTEXES",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return MIN(v, mAREA)->points.size();}));
    table.insert(std::pair<std::string, type>("COUNT EVEN",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return COUNT(v, cEVEN);}));
    table.insert(std::pair<std::string, type>("COUNT ODD",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return COUNT(v, cODD);}));
    table.insert(std::pair<std::string, type>("COUNT num",
                [](const std::vector<Polygon>& v, unsigned int m, Polygon){return COUNT(v,
                    [&m](const Polygon& p){return (p.points.size() == m);});}));
    table.insert(std::pair<std::string, type>("SAME",
                [](const std::vector<Polygon>& v, unsigned int, Polygon ppp){return COUNT(v,
                    [&ppp](const Polygon& p){return same(p, ppp);});}));
    table.insert(std::pair<std::string, type>("RECTS",
                [](const std::vector<Polygon>& v, unsigned int, Polygon){return COUNT(v, RECTS);}));

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
                std::cout << std::fixed << std::setprecision(1) << table.find(command.exp)->second(v,n,ppp) << '\n';
            }
            else
            {
                if ((command.exp == "AREA MEAN" || command.exp.find("MAX") == 0 || command.exp.find("MIN") == 0) && v.empty())
                {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                std::cout << static_cast<int>(table.find(command.exp)->second(v, n, ppp)) << '\n';
            }
        }
    }
}
