#include <iostream>
#include "lib.h"

class Thing
{
public:
    Thing(int value, std::string name) : value_(value), name_(name) {}

    int value() const { return value_; }
    std::string name() const { return name_; }

    void some_operation()
    {
        std::cout << "some_operation " << name() << " " << value() << std::endl;
    }

    void another_operation()
    {
        std::cout << "another_operation " << name() << " " << value() << std::endl;
    }

private:
    int value_;
    std::string name_;
};

class BunchesOfThingsLogic1
{
public:
    void add_thing(const Thing &thing)
    {
        things_.push_back(thing);
    }

    void do_something_with_all_things()
    {
        for (int i = 0; i < things_.size(); i++)
        {
            things_[i].some_operation();
        }
    }

    void do_something_else_with_all_things()
    {
        std::vector<Thing>::iterator it = things_.begin();
        while (it != things_.end())
        {
            it->another_operation();
            ++it;
        }
    }

    void do_something_with_odd_things()
    {
        for (Thing &t : things_)
        {
            if (t.value() % 2 == 1)
            {
                t.some_operation();
            }
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

private:
    std::vector<Thing> things_;
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
        execute_operation(all_things(), &Thing::some_operation);
    }

    void do_something_else_with_all_things()
    {
        execute_operation(all_things(), &Thing::another_operation);
    }

    void do_something_with_odd_things()
    {
        execute_operation(odd_things(), &Thing::some_operation);
    }

    void do_something_with_odd_things_named_bob()
    {
        execute_operation(odd_things_named_bob(), &Thing::some_operation);
    }

private:
    typedef lib::RefToContainer<std::vector<Thing>> ThingsRef;

    template <typename T>
    void execute_operation(T collections, void (Thing::*operation)())
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


int main(int, char *[])
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
