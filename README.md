# cpp_iterator_filter

cpp_iterator_filter is a C++11 compatible library designed to provide computational filtering and mapping of iterators, inspired by Rust's iterators. It's particularly useful for projects that can't use C++20's ranges due to legacy compiler requirements. By computational, we mean that elements are processed on-the-fly as requested, without pre-computing and storing them in a temporary container.

This library is an alternative to [RangeV3](https://github.com/ericniebler/range-v3), which, while a superior solution, requires C++14 and is not compatible with my current environment. Hence, cpp_iterator_filter offers a convenient workaround for those with similar constraints.

## Usage

The following example demonstrates how to use the library. It filters odd values from a vector and then doubles them:

```cpp
{
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {2, 6, 10, 14, 18};
    std::vector<int> actual;

    // Filter odd values
    auto odd_values = lib::filter(values, [](const int &value) { return value % 2 == 1; });
    // Map values by doubling them
    auto doubled_values = lib::map(odd_values, [](const int &value) { return value * 2; });

    // At this point, nothing has been computed yet.  The filtering and mapping has been
    // declared, but not yet executed.  The actual computation happens in-place when the values are
    // requested.

    for (const auto &value : doubled_values) {
        actual.push_back(value);
    }

    ASSERT_EQ(expected, actual);
}
```

The library currently supports two operations: `filter` and `map`. Convenience functions help improve syntax readability, but explicit template argument specification might be necessary depending on your compiler.

## Patterns

### Traditional vs. Recommended Approach

Traditionally, filtering and operations are often intertwined in loops, as shown below:

```cpp
for(int i = 0; i < MAX_THINGS; ++i) {
    auto &thing = things[i];
    if(thing.is_valid()) {
        thing.do_something();
    }
}
```

The problem with this is that you have to read the entire block to understand what it does.  The compact narriative sentence that describes the operation above is, "For each valid thing, do something." However, the code doesn't read like that. Instead, it reads like, "for all possible indicies, get the thing in the array at that index, and if it's valid, do something." The code is more complex than the operation it performs, which is a problem.

I recommend a different approach, adhering strictly to the single-responsibility principle:

```cpp
auto valid_things = lib::filter(things, [](const auto &thing) { return thing.is_valid(); });
for(const auto &thing : valid_things) {
    thing.do_something();
}
```

The partly upside-down description of the first expression is, "generate a new value `valid_things` that represents a filtering of things.  The filtering is defined as things with a valid property."  The second expression is, "for each valid thing, do something."

This might initially seem more complex due to lambda syntax, but the benefits are substantial:

- **Separation of Concerns**: Filtering is distinct from iteration and operation, simplifying code comprehension.
- **Reusability**: The filtering logic can be reused without duplication.
- **Testability**: Filtering logic can be independently tested from iteration and operation.
- **Clarity**: The loop makes it evident that an operation is performed on each valid item without needing to parse conditional statements.

### Addressing Repetition/Reusability

To handle common filtering/mapping operations on collections, convenience functions can be created:

```cpp
lib::FilterValues<Thing> valid_things() {
    return lib::filter(things, [](const auto &thing) { return thing.is_valid(); });
}

void do_something_with_things() {
    for(const auto &thing : valid_things()) {
        thing.do_something();
    }
}
```

In this example, the lambda expression is encapsulated outside the business logic, improving testability, reusability, and readability in `do_something_with_things()`.  In this final version, the business logic is entirely self-documenting because the code reads like the narrative sentence that describes it: "for each valid thing, do something."
