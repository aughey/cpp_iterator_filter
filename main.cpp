#include <iostream>
#include <benchmark/benchmark.h>
#include "lib.h"
// include sleep functions
#include <chrono>
#include <thread>

class Thing
{
public:
    Thing(int value, std::string name) : value_(value), name_(name) {}

    int value() const { return value_; }
    std::string name() const { return name_; }

    void some_operation()
    {
        call_count += 1;
    }

    void another_operation()
    {
        call_count += 1;
    }

    ssize_t get_call_count()
    {
        return call_count;
    }

private:
    ssize_t call_count = 0;
    int value_;
    std::string name_;
};

class BunchesOfThingsLogic1
{
public:
    BunchesOfThingsLogic1() : things_ref_(things_), odd_things_(lib::filter(things_ref_, [](const Thing &thing)
                                                                            { return thing.value() % 2 == 1; }))
    {
    }
    void add_thing(const Thing &thing)
    {
        things_.push_back(thing);
    }

    void index_for_loop()
    {
        for (int i = 0; i < things_.size(); i++)
        {
            things_[i].some_operation();
        }
    }

    void iterator_while_loop()
    {
        std::vector<Thing>::iterator it = things_.begin();
        while (it != things_.end())
        {
            it->another_operation();
            ++it;
        }
    }

    void range_for_loop()
    {
        for (Thing &t : things_)
        {
            t.some_operation();
        }
    }

    void range_for_loop_internal_odd_filter()
    {
        for (Thing &t : things_)
        {
            if (t.value() % 2 == 1)
            {
                t.some_operation();
            }
        }
    }

    void no_filter_ref()
    {
        auto ref = lib::ref(things_);
        for (Thing &t : ref)
        {
            t.some_operation();
        }
    }

    void filter_odd_externally()
    {
        auto ref = lib::ref(things_);
        auto odd_things = lib::filter(ref, [](const Thing &thing)
                                      { return thing.value() % 2 == 1; });
        for (Thing &t : odd_things)
        {
            t.some_operation();
        }
    }

    void filter_odd_externally_cached()
    {
        for (Thing &t : odd_things_)
        {
            t.some_operation();
        }
    }

    void do_something_with_odd_things_named_bob()
    {
        std::vector<Thing>::iterator it = things_.begin();
        while (it != things_.end())
        {
            if (it->value() % 2 == 1 && (it->name() == "Bob" || it->name() == "bob"))
            {
                it->some_operation();
            }
            ++it;
        }
    }

    ssize_t call_count()
    {
        return things_[0].get_call_count();
    }

private:
    std::vector<Thing> things_;
    lib::RefToContainer<std::vector<Thing>> things_ref_;
    lib::FilterValues<lib::RefToContainer<std::vector<Thing>>> odd_things_;
};

class BunchesOfThingsLogic2
{
public:
    void add_thing(const Thing &thing)
    {
        things_.push_back(thing);
    }

    void do_something_with_all_things()
    {
        for (auto &thing : all_things())
        {
            thing.some_operation();
        }
    }

    void do_something_else_with_all_things()
    {
        for (auto &thing : all_things())
        {
            thing.another_operation();
        }
    }

    void do_something_with_odd_things()
    {
        for (auto &thing : odd_things())
        {
            thing.some_operation();
        }
    }

    void do_something_with_odd_things_named_bob()
    {
        for (auto &thing : odd_things_named_bob())
        {
            thing.some_operation();
        }
    }

private:
    typedef lib::RefToContainer<std::vector<Thing>> ThingsRef;

    ThingsRef all_things()
    {
        return lib::ref(things_);
    }

    lib::FilterValues<ThingsRef> odd_things()
    {
        return lib::filter(all_things(), [](const Thing &thing)
                           { return thing.value() % 2 == 1; });
    }

    lib::FilterValues<lib::FilterValues<ThingsRef>> odd_things_named_bob()
    {
        return lib::filter(odd_things(), [](const Thing &thing)
                           { return thing.name() == "Bob" || thing.name() == "bob"; });
    }

    std::vector<Thing> things_;
};

class BunchesOfThingsLogic
{
public:
    void add_thing(const Thing &thing)
    {
        things_.push_back(thing);
    }

    void do_something_with_all_things()
    {
        call_for_all(all_things(), &Thing::some_operation);
    }

    void do_something_else_with_all_things()
    {
        call_for_all(all_things(), &Thing::another_operation);
    }

    void do_something_with_odd_things()
    {
        call_for_all(odd_things(), &Thing::some_operation);
    }

    void do_something_with_odd_things_named_bob()
    {
        call_for_all(odd_things_named_bob(), &Thing::some_operation);
    }

private:
    typedef lib::RefToContainer<std::vector<Thing>> ThingsRef;

    template <typename T>
    void call_for_all(T collections, void (Thing::*operation)())
    {
        for (auto &thing : collections)
        {
            (thing.*operation)();
        }
    }

    ThingsRef all_things()
    {
        return lib::ref(things_);
    }

    lib::FilterValues<ThingsRef> odd_things()
    {
        return lib::filter(all_things(), [](const Thing &thing)
                           { return thing.value() % 2 == 1; });
    }

    lib::FilterValues<lib::FilterValues<ThingsRef>> odd_things_named_bob()
    {
        return lib::filter(odd_things(), [](const Thing &thing)
                           { return thing.name() == "Bob" || thing.name() == "bob"; });
    }

    std::vector<Thing> things_;
};

int oldmain(int, char *[])
{
    BunchesOfThingsLogic logic;
    logic.add_thing(Thing(1, "bob"));
    logic.add_thing(Thing(2, "Bob"));
    logic.add_thing(Thing(3, "Bob"));
    logic.add_thing(Thing(4, "Alice"));
    logic.add_thing(Thing(4, "Carol"));

    logic.do_something_with_all_things();
    logic.do_something_with_odd_things_named_bob();

    return 0;
}

static BunchesOfThingsLogic1 loadMillion()
{
    BunchesOfThingsLogic1 logic;
    for (int i = 0; i < 1000000; i++)
    {
        logic.add_thing(Thing(i, "bob"));
    }
    return logic;
}

static void BM_IndexForLoop(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.index_for_loop();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_IndexForLoop);

static void BM_IteratorForLoop(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.iterator_while_loop();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_IteratorForLoop);

static void BM_RangeForLoop(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.range_for_loop();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_RangeForLoop);

static void BM_RangeForLoopInternalOdd(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.range_for_loop_internal_odd_filter();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_RangeForLoopInternalOdd);

static void BM_NoFilterRef(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.no_filter_ref();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_NoFilterRef);

static void BM_FilterOddExternally(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.filter_odd_externally();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_FilterOddExternally);

static void BM_FilterOddExternallyCached(benchmark::State &state)
{
    auto logic = loadMillion();
    for (auto _ : state)
        logic.filter_odd_externally_cached();
    state.SetItemsProcessed(1000000);
}
BENCHMARK(BM_FilterOddExternallyCached);

static void
BM_StringCreation(benchmark::State &state)
{
    for (auto _ : state)
        std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State &state)
{
    std::string x = "hello";
    for (auto _ : state)
        std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();