#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>

namespace nspace
{
    struct DataStruct
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    struct DelimiterIO { char exp; };
    struct StringDelimiterIO { std::string exp; };
    struct StringIO { std::string& ref; };
    struct DoubleLiteralIO { double& ref; };
    struct UllLiteralIO { unsigned long long& ref; };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s) :
            s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {
        }
        ~iofmtguard() {
            s_.width(width_); s_.fill(fill_); s_.precision(precision_); s_.flags(fmt_);
        }
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringDelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        for (char exp_c : dest.exp) {
            char c = '0';
            in >> c;
            if (!in || c != exp_c) {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        double val;
        in >> val;
        if (!in) return in;

        char suffix;
        in >> suffix;
        if (suffix != 'd' && suffix != 'D') {
            in.setstate(std::ios::failbit);
        }
        else {
            dest.ref = val;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UllLiteralIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        unsigned long long val;
        in >> val;
        if (!in) return in;

        char u, l1, l2;
        in >> u >> l1 >> l2;

        bool isU = (u == 'u' || u == 'U');
        bool isL1 = (l1 == 'l' || l1 == 'L');
        bool isL2 = (l2 == 'l' || l2 == 'L');

        if (isU && isL1 && isL2) {
            dest.ref = val;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct input;
        in >> StringDelimiterIO{ "(:" };

        for (int i = 0; i < 3; ++i) {
            std::string key_label;
            if (std::getline(in, key_label, ' ')) {
                if (key_label == "key1") {
                    in >> DoubleLiteralIO{ input.key1 };
                }
                else if (key_label == "key2") {
                    in >> UllLiteralIO{ input.key2 };
                }
                else if (key_label == "key3") {
                    in >> StringIO{ input.key3 };
                }
                else {
                    in.setstate(std::ios::failbit);
                }
            }
            if (i < 2) in >> DelimiterIO{ ':' };
        }
        in >> StringDelimiterIO{ ":)" };

        if (in) {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        iofmtguard fmtguard(out);

        out << std::fixed << std::setprecision(1);
        out << "(:key1 " << src.key1 << "d"
            << ":key2 " << src.key2 << "ull"
            << ":key3 \"" << src.key3 << "\":)";

        return out;
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.length() < b.key3.length();
    }
}

int main()
{
    using nspace::DataStruct;

    std::vector<DataStruct> data;

    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );

        if (std::cin.fail() && !std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(1);
        }
    }

    std::sort(data.begin(), data.end(), nspace::compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
