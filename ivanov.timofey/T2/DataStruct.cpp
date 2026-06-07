#include "DataStruct.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <limits>

namespace datastruct
{
    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}

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

    std::istream& operator>>(std::istream& in, UllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string token;
        char c;

        in >> std::ws;

        while (in.get(c))
        {
            if (std::isdigit(c) || std::isalpha(c))
            {
                token += c;
            }
            else
            {
                in.putback(c);
                break;
            }
        }

        if (token.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        try
        {
            std::string numStr = token;
            const std::string suffixes[] = { "ull", "ULL", "u11", "U11", "ll", "LL", "u", "U" };

            for (const auto& suffix : suffixes)
            {
                if (numStr.length() > suffix.length() &&
                    numStr.substr(numStr.length() - suffix.length()) == suffix)
                {
                    numStr = numStr.substr(0, numStr.length() - suffix.length());
                    break;
                }
            }

            for (char digit : numStr)
            {
                if (!std::isdigit(digit))
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
            }

            dest.ref = std::stoull(numStr);
        }
        catch (const std::exception&)
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, CmpLspIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char c;
        in >> std::ws;

        if (!in.get(c) || c != '#' ||
            !in.get(c) || c != 'c' ||
            !in.get(c) || c != '(')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        double real = 0.0, imag = 0.0;
        in >> std::ws;

        if (!(in >> real))
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> std::ws;

        if (!(in >> imag))
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> std::ws;

        if (!in.get(c) || c != ')')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = std::complex<double>(real, imag);
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        in >> std::ws;

        char c1, c2;
        if (!in.get(c1) || c1 != '(' || !in.get(c2) || c2 != ':')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        DataStruct temp{};
        bool f1 = false, f2 = false, f3 = false;

        while (!(f1 && f2 && f3))
        {
            in >> std::ws;

            if (in.peek() == ':')
            {
                in.get();
                if (in.peek() == ')')
                {
                    in.get();
                    break;
                }
            }

            std::string key;
            if (!(in >> key))
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            in >> std::ws;

            if (key == "key1" && !f1)
            {
                if (!(in >> UllLitIO{ temp.key1 }))
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f1 = true;
            }
            else if (key == "key2" && !f2)
            {
                if (!(in >> CmpLspIO{ temp.key2 }))
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f2 = true;
            }
            else if (key == "key3" && !f3)
            {
                if (!(in >> StringIO{ temp.key3 }))
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                f3 = true;
            }
            else
            {
                std::string tempString;
                if (!(in >> tempString))
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
            }
        }

        if (f1 && f2 && f3)
        {
            dest = temp;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }

        iofmtguard fmtguard(out);

        out << "(:key1 " << src.key1 << "ULL";
        out << ":key2 #c(" << std::fixed << std::setprecision(1) << src.key2.real() << " " << src.key2.imag() << ")";
        out << ":key3 \"" << src.key3 << "\":)";

        return out;
    }

    bool comparator(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }

        double absA = std::abs(a.key2);
        double absB = std::abs(b.key2);

        if (std::abs(absA - absB) > 1e-10)
        {
            return absA < absB;
        }

        return a.key3.length() < b.key3.length();
    }
}
