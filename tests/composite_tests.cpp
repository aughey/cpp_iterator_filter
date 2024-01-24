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

    auto odd_values = lib::filter(lib::ref(values), odd_int);
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

static auto filtered_list()
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

TEST(CompositeTests, AutoReturn) {
    s_list = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {1, 3, 5, 7, 9};
    std::vector<int> actual;

    auto odd_values = filtered_list();

    for (const auto &value : odd_values)
    {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}

class Thing {
    public:
    Thing(std::string name, int index) : name(name), index(index) {}
    std::string name;
    int index;
};

TEST(CompositeTests, ComplexType) {
    std::vector<Thing> things = {Thing("one", 1), Thing("two", 2), Thing("three", 3), Thing("four", 4), Thing("five", 5)};

    auto things_with_letter_e_in_name = lib::filter(things, [](const Thing &thing) {
        return thing.name.find("e") != std::string::npos;
    });

    auto names = lib::map(things_with_letter_e_in_name, std::function<std::string(const Thing &)>([](const Thing &thing) {
        return thing.name;
    }));

    std::vector<std::string> expected = {"one", "three", "five"};
    std::vector<std::string> actual;

    for(const auto &name : names) {
        actual.push_back(name);
    }

    ASSERT_EQ(expected, actual);
}