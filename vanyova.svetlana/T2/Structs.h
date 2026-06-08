#ifndef STRUCTS_H
#define STRUCTS_H
#include <iostream>
#include <string>
namespace structs {
    struct DataStruct
    {
        double key1;
        char key2;
        std::string key3;
    };
    struct DelimiterIO
    {
        char exp;
    };
    struct DoubleIO
    {
        double& ref;
    };
    struct CharIO
    {
        char& ref;
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
        iofmtguard(std::basic_ios< char >& s);
        ~iofmtguard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::istream& operator>>(std::istream& in, CharIO&& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
    std::string toDblSci(double x);
    bool comparator(const DataStruct& a, const DataStruct& b);
}
#endif //STRUCTS_H
