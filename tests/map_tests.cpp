#include <gtest/gtest.h>
#include "lib.h"

int double_int(const int &value)
{
    return value * 2;
}

// Simple test to check equality of two numbers
TEST(MapTests, Double) {
    std::vector<int> values = {1, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> expected = {2, 4, 6, 6, 6, 8, 8, 8, 10, 12, 14, 16, 18, 20};
    std::vector<int> actual;

    auto doubled_values = lib::map<std::vector<int>,int,int>(values, double_int);

    for (const auto &value : doubled_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}