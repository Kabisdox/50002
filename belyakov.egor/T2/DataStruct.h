#ifndef _DATASTRUCT_H
#define _DATASTRUCT_H

#include <iostream>
#include <string>
#include <iomanip>
#include <complex>

namespace nspace
{
    struct DataStruct
    {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct UllHexIO
    {
        unsigned long long& ref;
    };

    struct CmpLspIO
    {
        std::complex<double>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s) noexcept;

        ~iofmtguard();

    private:
        std::basic_ios<char>& s_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, UllHexIO&& dest);
    std::istream& operator>>(std::istream& in, CmpLspIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);

    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool sortDataStruct(const DataStruct& firstElement, const DataStruct& secondElement);
}
#endif
