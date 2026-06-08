#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <iterator>
#include "Structs.h"

// Variant №9 (key1: [SLL LIT]; key2: [CMP LSP])
int main() {
    std::vector<structs::DataStruct> data;

    while (!(std::cin.eof())) {
        std::copy(
            std::istream_iterator<structs::DataStruct>(std::cin),
            std::istream_iterator<structs::DataStruct>(),
            std::back_inserter(data)
        );

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), structs::comparator);
    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<structs::DataStruct>(std::cout, "\n")
    );

    return 0;
}
