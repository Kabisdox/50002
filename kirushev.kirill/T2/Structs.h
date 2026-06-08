#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <complex>

namespace structs {
    struct DataStruct {
        long long key1;
        std::complex<double> key2;
        std::string key3;
    };
    struct DelimiterIO {
        char exp;
    };
    struct LongLongIO {
        long long& ref;
    };
    struct ComplexIO {
        std::complex<double>& ref;
    };
    struct StringIO {
        std::string& ref;
    };

    std::istream& operator>>(std::istream&, DelimiterIO&&);
    std::istream& operator>>(std::istream&, LongLongIO&&);
    std::istream& operator>>(std::istream&, ComplexIO&&);
    std::istream& operator>>(std::istream&, StringIO&&);

    std::istream& operator>>(std::istream&, DataStruct&);
    std::ostream& operator<<(std::ostream&, const DataStruct&);

    bool comparator(const DataStruct&, const DataStruct&);

    class StreamGuard {
    public:
        explicit StreamGuard(std::basic_ios<char>&);
        ~StreamGuard();
    private:
        std::basic_ios<char>& stream_;
        char fill_;
        std::streamsize width_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags flags_;
    };
}

#endif //STRUCTS_H
