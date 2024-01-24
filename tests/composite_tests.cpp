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

// Simple test to check equality of two numbers
TEST(CompositeTests, DoubleWithLambdas)
{
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The operation will filter odd values and then double them, so the
    // expected is the doubling of 1,3,5,7,9
    std::vector<int> expected = {2, 6, 10, 14, 18};
    std::vector<int> actual;

    auto odd_values = lib::filter(values, [](const int &value)
                                  { return value % 2 == 1; });
    auto doubled_values = lib::map(odd_values, std::function<int(const int &)>([](const int &value)
                                                                               { return value * 2; }));

    for (const auto &value : doubled_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}

// auto return type
static std::vector<int> s_list;

auto filtered_list()
{
    return lib::filter(s_list, [](const int &value)
                       { return value % 2 == 1; });
}

// Simple test to check equality of two numbers
TEST(CompositeTests, TypeChanging)
{
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The operation will filter odd values and then multiply by 3.14.
    std::vector<double> expected = {1 * 3.14, 3 * 3.14, 5 * 3.14, 7 * 3.14, 9 * 3.14};
    std::vector<double> actual;

    auto odd_values = lib::filter(values, [](const int &value)
                                  { return value % 2 == 1; });
    auto doubled_values = lib::map(odd_values, std::function<double(const int &)>([](const int &value)
                                                                               { return value * 3.14; }));

    for (const auto &value : doubled_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}