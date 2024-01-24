#include <iostream>
#include "lib.h"
int main(int, char *[])
{
    std::vector<int> values = {1, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 8, 9, 10};

    auto is_odd = [](const int &value)
    { return value % 2 == 1; };

    for (auto &value : lib::filter(values, is_odd))
    {
        std::cout << value << "\n";
    }

    return 0;
}
