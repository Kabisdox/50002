#include <iostream>
#include "Structs.h"

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
    const std::istream::sentry sentry(is);

    if (!sentry) {
        return is;
    }

    char c = '\0';
    if (!(is.get(c) && c == dest.exp)) {
        is.setstate(std::ios::failbit);
    }
    return is;
}
