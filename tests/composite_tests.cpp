#include <gtest/gtest.h>
#include <vector>
#include "lib.h"

static int double_int(const int &value)
{
    return value * 2;
}

static bool odd_int(const int &value)
{
    return value % 2 == 1;
}

// Simple test to check equality of two numbers
TEST(CompositeTests, Double)
{
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The operation will filter odd values and then double them, so the
    // expected is the doubling of 1,3,5,7,9
    std::vector<int> expected = {2, 6, 10, 14, 18};
    std::vector<int> actual;

    auto odd_values = lib::filter(values, odd_int);
    auto doubled_values = lib::map(odd_values, std::function<int(const int &)>(double_int));

    for (const auto &value : doubled_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}