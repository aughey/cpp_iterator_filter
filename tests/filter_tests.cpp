#include <gtest/gtest.h>
#include "lib.h"

bool is_odd(const int &value)
{
    return value % 2 == 1;
}

// Simple test to check equality of two numbers
TEST(FilterTests, OddFilter) {
    std::vector<int> values = {1, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> expected = {1, 3, 3, 3, 5, 7, 9};
    std::vector<int> actual;

    auto odd_values = lib::filter(values, is_odd);

    for (const auto &value : odd_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}

// Simple test to check equality of two numbers
TEST(FilterTests, OddFilterLambda) {
    std::vector<int> values = {1, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 8, 9, 10};

    auto is_odd_lambda = [](const int &value)
    { return value % 2 == 1; };

    std::vector<int> expected = {1, 3, 3, 3, 5, 7, 9};
    std::vector<int> actual;

    auto odd_values = lib::filter(values, is_odd_lambda);

    for (const auto &value : odd_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}