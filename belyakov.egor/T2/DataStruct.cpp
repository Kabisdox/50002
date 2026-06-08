#include "DataStruct.h"

namespace nspace
{
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

    std::istream& operator>>(std::istream& in, UllHexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> std::hex >> dest.ref;
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
        in >> std::fixed >> DelimiterIO{ '#' }
            >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' }
        >> real >> imag >> DelimiterIO{ ')' };
        dest.ref = std::complex <double>(real, imag);
        if (!in)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string DataStruct = "";
        if ((in >> StringIO{ DataStruct }) && (DataStruct != dest.exp))
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
            using ullHex = UllHexIO;
            using cmpLsp = CmpLspIO;
            std::string iKey = "";
            in >> sep{ '(' } >> sep{ ':' };
            for (int i = 0; i < 3; i++) {
                in >> iKey;
                if (iKey == "key1")
                {
                    in >> std::hex >> ullHex{ input.key1 } >> std::dec;
                }
                else if (iKey == "key2")
                {
                    in >> cmpLsp{ input.key2 };
                }
                else if (iKey == "key3")
                {
                    in >> str{ input.key3 };
                }
                else
                {
                    in.setstate(std::ios::failbit);
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
        out << "(:key1 0x" << std::uppercase << std::hex << src.key1
            << ":key2 " << std::fixed << std::setprecision(1)
            << "#c(" << src.key2.real() << " " << src.key2.imag() << ")"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool sortDataStruct(const DataStruct& first, const DataStruct& second)
    {
        if (first.key1 != second.key1)
        {
            return second.key1 > first.key1;
        }
        if (std::abs(second.key2) != std::abs(first.key2))
        {
            return std::abs(second.key2) > std::abs(first.key2);
        }
        return second.key3.length() > first.key3.length();
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) noexcept :
        s_(s),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {
    }

    iofmtguard::~iofmtguard()
    {
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}
