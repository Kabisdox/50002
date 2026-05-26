#include "DataStruct.h"
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <sstream>

namespace nspace
{
    iofmtguard::iofmtguard(std::basic_ios<char>& s) noexcept :
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

    std::istream& operator>>(std::istream& in, UllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string token;
        char c;

        while (in.get(c) && std::isdigit(c))
        {
            token += c;
        }

        if (token.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string suffix;
        suffix += c;
        if (in.get(c))
        {
            suffix += c;
        }
        if (in.get(c))
        {
            suffix += c;
        }

        std::string lowerSuffix = suffix;
        std::transform(lowerSuffix.begin(), lowerSuffix.end(), lowerSuffix.begin(), ::tolower);

        if (lowerSuffix == "ull")
        {
            try
            {
                dest.ref = std::stoull(token);
                return in;
            }
            catch (const std::exception&)
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, CmpLspIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        double real = 0.0;
        double imag = 0.0;

        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' }
        >> real >> imag >> DelimiterIO{ ')' };

        dest.ref = std::complex<double>(real, imag);

        if (!in)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        DataStruct input;
        {
            using sep = DelimiterIO;
            using str = StringIO;
            using ullLit = UllLitIO;
            using cmpLsp = CmpLspIO;

            in >> sep{ '(' } >> sep{ ':' };

            std::string iKey = "";
            bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

            for (int i = 0; i < 3; i++)
            {
                in >> iKey;

                if (iKey == "key1" && !hasKey1)
                {
                    in >> ullLit{ input.key1 };
                    hasKey1 = true;
                }
                else if (iKey == "key2" && !hasKey2)
                {
                    in >> cmpLsp{ input.key2 };
                    hasKey2 = true;
                }
                else if (iKey == "key3" && !hasKey3)
                {
                    in >> str{ input.key3 };
                    hasKey3 = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                if (!in)
                {
                    return in;
                }

                in >> sep{ ':' };
            }

            in >> sep{ ')' };
        }

        if (in)
        {
            dest = input;
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

        out << "(:key1 " << src.key1 << "ull"
            << ":key2 " << std::fixed << std::setprecision(1)
            << "#c(" << src.key2.real() << " " << src.key2.imag() << ")"
            << ":key3 \"" << src.key3 << "\":)";

        return out;
    }

    bool sortDataStruct(const DataStruct& first, const DataStruct& second)
    {
        if (first.key1 != second.key1)
        {
            return first.key1 < second.key1;
        }

        double absFirst = std::abs(first.key2);
        double absSecond = std::abs(second.key2);

        if (std::abs(absFirst - absSecond) > 1e-10)
        {
            return absFirst < absSecond;
        }

        return first.key3.length() < second.key3.length();
    }
}