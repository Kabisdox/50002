#include <string>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include "Polygon.h"
iofmtguard::iofmtguard(std::basic_ios<char>& s) noexcept :
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}
iofmtguard::~iofmtguard() {
    s_.flags(fmt_);
    s_.precision(precision_);
    s_.fill(fill_);
}

std::istream& operator>>(std::istream& in, StrictDelim&& dest) {
    std::istream::sentry sentry(in);

    if (!sentry) {

        return in;

    }
    char ch = 0;
    if (!(in.get(ch) && ch == dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}


std::istream& operator>>(std::istream& in, Point& point) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    in >> StrictDelim{ '(' } >> point.x >> StrictDelim{ ';' } >> point.y >> StrictDelim{ ')' };
    return in;
}
std::ostream& operator<<(std::ostream& out, const Point& point) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    out << '(' << point.x << ';' << point.y << ')';
    return out;
}

std::istream& operator>>(std::istream& in, Polygon& polygon) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    size_t count = 0;
    if (!(in >> count) || count < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> std::noskipws;
    Polygon input;
    input.points.reserve(count);
    std::generate_n(std::back_inserter(input.points), count,
        [&in]() {
            Point point;
            char ch = 0;
            if (!(in.get(ch) && ch == ' ' && in >> point)){
                in.setstate(std::ios::failbit);
            }
            return point;
        }
    );
    char check_end = 0;
    if (in.get(check_end) && check_end != '\n'){
        in.setstate(std::ios::failbit);
    }
    in >> std::skipws;
    if (in) {
        polygon = std::move(input);
    }
    return in;
}
