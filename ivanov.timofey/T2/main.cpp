#include "DataStruct.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

int main()
{
    using namespace datastruct;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    if (data.empty()) {
        std::cout << "Atleast one supported record type" << std::endl;
        return 0;
    }

    std::sort(data.begin(), data.end(), comparator);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
