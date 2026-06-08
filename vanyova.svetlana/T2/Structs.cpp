#include "Structs.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <iterator>
#include <cctype>
namespace structs {
    iofmtguard::iofmtguard(std::basic_ios< char >& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {
    }
    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }
    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = "";
        if ((in >> StringIO{ data }) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
    std::istream& operator>>(std::istream& in, CharIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        char open_q;
        char close_q;
        in >> open_q;
        in.get(dest.ref);
        in >> close_q;
        if (open_q != '\'' || close_q != '\'') {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
    std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        int BeforPoint = 0;
        if (!(in >> BeforPoint)) {
            return in;
        }
        char dot = '0';
        if (in.peek() != '.') {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.get(dot);
        if (!isdigit(in.peek())) {
            in.setstate(std::ios::failbit);
            return in;
        }
        int digitCount = 0;
        int after = 0;
        while (isdigit(in.peek())) {
            after = after * 10 + (in.get() - '0');
            ++digitCount;
        }
        char e = in.peek();
        if (e != 'e' && e != 'E') {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.ignore();
        int exponent = 0;
        if (!(in >> exponent)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        double mantissa = std::abs(BeforPoint) + (after / std::pow(10.0, digitCount));
        dest.ref = mantissa * std::pow(10.0, exponent);
        return in;
    }
    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(:key1 " << toDblSci(src.key1) << ":key2 '" << src.key2 << "':key3 \"" << src.key3 << "\":)";
        return out;
    }
    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        DataStruct temp;
        if (!(in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' })) return in;
        bool f1 = false;
        bool f2 = false;
        bool f3 = false;
        for (int i = 0; i < 3; ++i) {
            std::string key;
            if (!(in >> key)) break;
            char space;
            in.get(space);
            if (space != ' ') {
                in.setstate(std::ios::failbit);
                break;
            }
            if (std::isspace(in.peek())) {
                in.setstate(std::ios::failbit);
                break;
            }
            if (key == "key1") {
                if (!(in >> DoubleIO{ temp.key1 })) break;
                f1 = true;
            }
            else if (key == "key2") {
                if (!(in >> CharIO{ temp.key2 })) break;
                f2 = true;
            }
            else if (key == "key3") {
                if (!(in >> StringIO{ temp.key3 })) break;
                f3 = true;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
            if (std::isspace(in.peek())) {
                in.setstate(std::ios::failbit);
                break;
            }
            if (!(in >> DelimiterIO{ ':' })) break;
        }
        if (in >> DelimiterIO{ ')' } && f1 && f2 && f3) {
            dest = temp;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
    bool comparator(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        else if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        else {
            return a.key3.length() < b.key3.length();
        }
    }
    std::string toDblSci(double x) {
        if (x == 0.0) {
            return "0.0e+0";
        }
        int exponent = 0;
        bool isNegative = false;
        double mantissa = 0;
        if (x < 0) {
            isNegative = true;
            mantissa = -x;
        }
        else {
            isNegative = false;
            mantissa = x;
        }
        if (mantissa >= 10.0) {
            while (mantissa >= 10.0) {
                mantissa = mantissa / 10.0;
                exponent = exponent + 1;
            }
        }
        else if (mantissa < 1.0) {
            while (mantissa < 1.0) {
                mantissa = mantissa * 10.0;
                exponent = exponent - 1;
            }
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << (isNegative ? -mantissa : mantissa);
        ss << "e";
        if (exponent >= 0) {
            ss << "+";
        }
        ss << exponent;
        return ss.str();
    }
}
