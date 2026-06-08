#include <iostream>
#include <iomanip>
#include <string>
#include "Structs.h"

namespace structs {
    StreamGuard::StreamGuard(std::basic_ios<char>& stream):
        stream_(stream),
        fill_(stream.fill()),
        width_(stream.width()),
        precision_(stream.precision()),
        flags_(stream.flags())
    {}

    StreamGuard::~StreamGuard() {
        stream_.fill(fill_);
        stream_.width(width_);
        stream_.precision(precision_);
        stream_.flags(flags_);
    }

    std::istream& operator>>(std::istream& is, DelimiterIO&& dest) {
        std::istream::sentry sentry(is);
        if (!sentry) {
            return is;
        }

        char c = '\0';
        is >> c;

        if (is && (c != dest.exp)) {
            is.setstate(std::ios_base::failbit);
        }
        return is;
    }

    std::istream& operator>>(std::istream& is, LongLongIO&& dest) {
        std::istream::sentry sentry(is);
        if (!sentry) {
            return is;
        }

        long long number;

        if (!(is >> number)) {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        char s1 = '\0';
        char s2 = '\0';

        is >> s1 >> s2;

        if (!((s1 == 'l' && s2 == 'l') || (s1 == 'L' && s2 == 'L'))) {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        dest.ref = number;
        return is;
    }

    std::istream& operator>>(std::istream& is, ComplexIO&& dest) {
        std::istream::sentry sentry(is);
        if (!sentry) {
            return is;
        }

        is >> DelimiterIO{'#'};
        is >> DelimiterIO{'c'};
        is >> DelimiterIO{'('};

        double real, imag;
        if (!(is >> real >> imag)) {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        is >> DelimiterIO{')'};

        if (is) {
            dest.ref = std::complex<double>(real, imag);
        }
        return is;
    }

    std::istream& operator>>(std::istream& is, StringIO&& dest) {
        std::istream::sentry sentry(is);
        if (!sentry) {
            return is;
        }
        return std::getline(is >> DelimiterIO{'"'}, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& is, DataStruct& dest) {
        std::istream::sentry sentry(is);
        if (!sentry) {
            return is;
        }

        DataStruct input;
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;

        if (!(is >> DelimiterIO{'('})) {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        for (std::size_t i = 0; i < 3; ++ i) {
            if (!(is >> DelimiterIO{':'})) {
                is.setstate(std::ios_base::failbit);
                return is;
            }

            std::string label;
            if (!(is >> label)) {
                is.setstate(std::ios_base::failbit);
                return is;
            }

            if (label == "key1" && !hasKey1) {
                is >> LongLongIO{input.key1};
                hasKey1 = true;
            } else if (label == "key2" && !hasKey2) {
                is >> ComplexIO{input.key2};
                hasKey2 = true;
            } else if (label == "key3" && !hasKey3) {
                is >> StringIO{input.key3};
                hasKey3 = true;
            } else {
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }

        if (!(is >> DelimiterIO{':'} >> DelimiterIO{')'})) {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        if (is && hasKey1 && hasKey2 && hasKey3) {
            dest = std::move(input);
        } else {
            is.setstate(std::ios_base::failbit);
        }
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const DataStruct& dest) {
        std::ostream::sentry sentry(os);
        if (!sentry) {
            return os;
        }
        StreamGuard guard(os);

        os << "(:key1 " << dest.key1 << "ll";
        os << std::fixed << std::setprecision(1) << ":key2 #c(" << dest.key2.real() << " " << dest.key2.imag() << ")";
        os << ":key3 \"" << dest.key3 << "\":)";
        return os;
    }

    bool comparator(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return std::abs(a.key2) < std::abs(b.key2);
        }
        return a.key3.length() < b.key3.length();
    }
}
