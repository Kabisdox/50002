#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <limits>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

struct UllIO
{
    unsigned long long& ref;
};

struct DelimiterIO
{
    char exp;
};

struct DoubleIO
{
    double& ref;
};

struct StringIO
{
    std::string& ref;
};

struct LabelIO
{
    std::string exp;
};

struct SmthIO
{
    char exp;
};

// scope guard
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

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
        in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, DoubleIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c;
    bool dot{false};
    std::string num;
    while (in.get(c))
    {
        if (isdigit(c) || c == '.' || c == '-' || c == '+')
        {
            num += c;
            if (c == '.')
                dot = true;
        }
        else if (c == 'd' || c == 'D')
        {
            break;
        }
        else
        {
            in.putback(c);
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    if (!dot || num.empty() || num.front() == '.' || num.back() == '.')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    try
    {
        dest.ref = std::stod(num);
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UllIO&& dest)
{
    std::istream::sentry sentry(in);
    if(!sentry)
        return in;
    std::string suf;
    long long test;
    if(!(in >> test) || test < 0)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    char delim{':'};
    if (std::getline(in, suf, delim))
    {
        in.putback(delim);
    }
    if (suf  != "ull" && suf != "ULL")
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    dest.ref = test;
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    return in >> dest.exp;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct input{};

    in >> SmthIO{'('};
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    bool k1 = false;
    bool k2 = false;
    bool k3 = false;
    for (std::size_t i{0}; i < 3; ++i)
    {
        in >> DelimiterIO{':'};
        if (!in)
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        LabelIO a{};
        in >> a;
        if (!in)
        {
            break;
        }
        if (a.exp == "key1")
        {
            in >> DoubleIO{input.key1};
            k1 = true;
        }
        else if (a.exp == "key2")
        {
            in >> UllIO{input.key2};
            k2 = true;
        }
        else if (a.exp == "key3")
        {
            in >> StringIO{input.key3};
            k3 = true;
        }
        else
        {
            break;
        }

        if (!in)
        {
            break;
        }
    }
    in >> DelimiterIO{':'} >> SmthIO{')'};
    if (!in)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    if (k1 && k2 && k3)
    {
        if (in)
        {
            dest = std::move(input);
            return in;
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
        return out;
    iofmtguard fmtguard(out);
    out << "(";
    out << ":key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 " << src.key2 << "ull";
    out << ":key3 " << "\"" << src.key3 << "\"";
    out << ":";
    out << ")";
    return out;
}

bool cmp(const DataStruct& f, const DataStruct& ff)
{
    if(f.key1 < ff.key1)
        return true;
    else if (f.key1 == ff.key1 && f.key2 < ff.key2)
        return true;
    else if (f.key1 == ff.key1 && f.key2 == ff.key2 && f.key3.length() < ff.key3.length())
        return true;
    return false;
}

int main()
{
    std::vector<DataStruct> v;
    while (!std::cin.eof())
    {
        std::copy
        (
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(v)
        );
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort
    (
        std::begin(v),
        std::end(v),
        cmp
    );

    std::copy
    (
        std::begin(v),
        std::end(v),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
