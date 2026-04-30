#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

struct DataStruct
{
    unsigned long long key1;
    unsigned long long key2;
    std::string key2Text;
    std::string key3;
};

class FmtGuard
{
public:
    explicit FmtGuard(std::basic_ios<char>& stream) :
        stream_(stream),
        precision_(stream.precision()),
        flags_(stream.flags()),
        fill_(stream.fill())
    {
    }

    ~FmtGuard()
    {
        stream_.precision(precision_);
        stream_.flags(flags_);
        stream_.fill(fill_);
    }

private:
    std::basic_ios<char>& stream_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
    char fill_;
};

namespace details
{
    void setFail(std::istream& input)
    {
        input.setstate(std::ios::failbit);
    }

    bool readChar(std::istream& input, char expected)
    {
        char c = 0;
        if (!input.get(c) || c != expected)
        {
            setFail(input);
            return false;
        }
        return true;
    }

    bool readFieldName(std::istream& input, std::string& fieldName)
    {
        fieldName.clear();

        char c = 0;
        while (input.get(c))
        {
            if (c == ' ')
            {
                if (fieldName.empty())
                {
                    setFail(input);
                    return false;
                }
                return true;
            }

            const unsigned char uc = static_cast<unsigned char>(c);
            if (c == ':' || c == '(' || c == ')' || std::isspace(uc))
            {
                setFail(input);
                return false;
            }

            fieldName += c;
        }

        setFail(input);
        return false;
    }

    bool appendDecimalDigit(unsigned long long& value, int digit)
    {
        const unsigned long long max =
            std::numeric_limits<unsigned long long>::max();
        if (value > (max - static_cast<unsigned long long>(digit)) / 10ULL)
        {
            return false;
        }
        value = value * 10ULL + static_cast<unsigned long long>(digit);
        return true;
    }

    bool appendBinaryDigit(unsigned long long& value, int digit)
    {
        const unsigned long long max =
            std::numeric_limits<unsigned long long>::max();
        if (value > (max - static_cast<unsigned long long>(digit)) / 2ULL)
        {
            return false;
        }
        value = value * 2ULL + static_cast<unsigned long long>(digit);
        return true;
    }

    bool readUllLiteral(std::istream& input, unsigned long long& value)
    {
        value = 0;

        char c = 0;
        if (!input.get(c) || !std::isdigit(static_cast<unsigned char>(c)))
        {
            setFail(input);
            return false;
        }

        while (std::isdigit(static_cast<unsigned char>(c)))
        {
            const int digit = c - '0';

            if (!appendDecimalDigit(value, digit))
            {
                setFail(input);
                return false;
            }

            if (!input.get(c))
            {
                setFail(input);
                return false;
            }
        }

        char second = 0;
        char third = 0;
        if (!input.get(second) || !input.get(third))
        {
            setFail(input);
            return false;
        }

        const std::string suffix{ c, second, third };
        if (suffix != "ull" && suffix != "ULL")
        {
            setFail(input);
            return false;
        }

        return true;
    }

    bool readUllBinary(
        std::istream& input,
        unsigned long long& value,
        std::string& text)
    {
        value = 0;
        text.clear();

        if (!readChar(input, '0'))
        {
            return false;
        }

        char b = 0;
        if (!input.get(b) || (b != 'b' && b != 'B'))
        {
            setFail(input);
            return false;
        }

        bool hasDigit = false;
        while (input)
        {
            const int next = input.peek();
            if (next != '0' && next != '1')
            {
                break;
            }

            char digitChar = 0;
            input.get(digitChar);
            text += digitChar;

            const int digit = digitChar - '0';
            if (!appendBinaryDigit(value, digit))
            {
                setFail(input);
                return false;
            }

            hasDigit = true;
        }

        if (!hasDigit)
        {
            setFail(input);
            return false;
        }

        return true;
    }

    bool readQuotedString(std::istream& input, std::string& value)
    {
        if (!readChar(input, '"'))
        {
            return false;
        }

        std::string result;
        char c = 0;
        while (input.get(c))
        {
            if (c == '\\')
            {
                char escaped = 0;
                if (!input.get(escaped))
                {
                    setFail(input);
                    return false;
                }
                result += escaped;
            }
            else if (c == '"')
            {
                value = result;
                return true;
            }
            else
            {
                result += c;
            }
        }

        setFail(input);
        return false;
    }

    bool readLineEnd(std::istream& input)
    {
        while (input)
        {
            const int next = input.peek();
            if (next == std::char_traits<char>::eof())
            {
                return true;
            }

            if (next == '\n')
            {
                input.get();
                return true;
            }

            if (next == '\r')
            {
                input.get();
                if (input.peek() == '\n')
                {
                    input.get();
                }
                return true;
            }

            if (next == ' ' || next == '\t')
            {
                input.get();
                continue;
            }

            setFail(input);
            return false;
        }

        return true;
    }

    bool readDataStruct(std::istream& input, DataStruct& data)
    {
        DataStruct temp{ 0ULL, 0ULL, "", "" };
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;

        if (!readChar(input, '('))
        {
            return false;
        }

        for (int i = 0; i < 3; ++i)
        {
            if (!readChar(input, ':'))
            {
                return false;
            }

            std::string fieldName;
            if (!readFieldName(input, fieldName))
            {
                return false;
            }

            if (fieldName == "key1" && !hasKey1)
            {
                if (!readUllLiteral(input, temp.key1))
                {
                    return false;
                }
                hasKey1 = true;
            }
            else if (fieldName == "key2" && !hasKey2)
            {
                if (!readUllBinary(input, temp.key2, temp.key2Text))
                {
                    return false;
                }
                hasKey2 = true;
            }
            else if (fieldName == "key3" && !hasKey3)
            {
                if (!readQuotedString(input, temp.key3))
                {
                    return false;
                }
                hasKey3 = true;
            }
            else
            {
                setFail(input);
                return false;
            }
        }

        if (!readChar(input, ':') || !readChar(input, ')'))
        {
            return false;
        }

        if (!(hasKey1 && hasKey2 && hasKey3))
        {
            setFail(input);
            return false;
        }

        if (!readLineEnd(input))
        {
            return false;
        }

        data = temp;
        return true;
    }
}

std::istream& operator>>(std::istream& input, DataStruct& data)
{
    std::istream::sentry sentry(input);
    if (!sentry)
    {
        return input;
    }

    while (input)
    {
        DataStruct temp{ 0ULL, 0ULL, "", "" };
        if (details::readDataStruct(input, temp))
        {
            data = temp;
            return input;
        }

        if (input.eof() || input.bad())
        {
            return input;
        }

        input.clear();
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        input >> std::ws;
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& data)
{
    std::ostream::sentry sentry(output);
    if (sentry)
    {
        FmtGuard guard(output);
        output << "(:key1 " << std::dec << data.key1 << "ull"
            << ":key2 0b" << data.key2Text
            << ":key3 " << std::quoted(data.key3)
            << ":)";
    }
    return output;
}

bool compareData(const DataStruct& left, const DataStruct& right)
{
    if (left.key1 != right.key1)
    {
        return left.key1 < right.key1;
    }
    if (left.key2 != right.key2)
    {
        return left.key2 < right.key2;
    }
    return left.key3.size() < right.key3.size();
}

int main()
{
    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), compareData);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
