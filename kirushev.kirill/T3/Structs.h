#ifndef STRUCTS_H
#define STRUCTS_H

#include <ios>

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

struct DelimiterIO {
    char exp;
};

std::istream& operator>>(std::istream&, DelimiterIO&&);

#endif //STRUCTS_H
