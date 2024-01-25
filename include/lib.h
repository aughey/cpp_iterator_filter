#pragma once
#include <vector>
#include <functional>

namespace lib
{
    template <typename IT>
    class FilterIterator
    {
    public:
        typedef typename IT::value_type value_type;

        FilterIterator(IT iterator, IT end,
                       std::function<bool(const typename IT::value_type &)>
                           &allow_fn) : iterator_(iterator), end_(end), allow_fn_(allow_fn)
        {
            while (iterator_ != end_ && allow_fn_(*iterator_) == false)
            {
                ++iterator_;
            }
        }

        bool operator!=(const FilterIterator<IT> &other) const
        {
            return iterator_ != other.iterator_;
        }

        const typename IT::value_type &operator*() const
        {
            return *iterator_;
        }

        typename IT::value_type &operator*()
        {
            return *iterator_;
        }

        FilterIterator<IT> &operator++()
        {
            do
            {
                ++iterator_;
            } while (iterator_ != end_ && allow_fn_(*iterator_) == false);
            return *this;
        }

    private:
        IT iterator_, end_;
        std::function<bool(const typename IT::value_type &)> &allow_fn_;
    };

    template <typename T>
    class FilterValues
    {
    public:
        typedef FilterIterator<typename T::iterator> iterator;

        FilterValues(T container, std::function<bool(const typename T::iterator::value_type &)> allow_fn)
            : container_(container), allow_fn_(allow_fn) {}

        FilterValues<T>::iterator begin()
        {
            return FilterIterator<typename T::iterator>(container_.begin(), container_.end(), allow_fn_);
        }

        FilterValues<T>::iterator end()
        {
            return FilterIterator<typename T::iterator>(container_.end(), container_.end(), allow_fn_);
        }

    private:
        T container_;
        std::function<bool(const typename T::iterator::value_type &)> allow_fn_;
    };

    // Because c++11 doesn't deduce template arguments right, we have to create a helper function
    // to create the FilterValues object without having to specify the template arguments.
    template <typename T>
    FilterValues<T> filter(T container, std::function<bool(const typename T::iterator::value_type &)> allow_fn)
    {
        return FilterValues<T>(container, allow_fn);
    }

    // A similar class to filter, but to map values from one type to another
    template <typename IT, typename OUT>
    class MapIterator
    {
    public:
        typedef OUT value_type;

        MapIterator(IT iterator, IT end,
                    std::function<OUT(const typename IT::value_type &)> map_fn) : iterator_(iterator), end_(end), map_fn_(map_fn)
        {
        }

        bool operator!=(const MapIterator<IT, OUT> &other) const
        {
            return iterator_ != other.iterator_;
        }

        OUT operator*() const
        {
            return map_fn_(*iterator_);
        }

        MapIterator<IT, OUT> &operator++()
        {
            ++iterator_;
            return *this;
        }

    private:
        IT iterator_, end_;
        std::function<OUT(const typename IT::value_type &)> map_fn_;
    };

    template <typename T, typename IN, typename OUT>
    class MapValues
    {
    public:
        typedef MapIterator<typename T::iterator, OUT> iterator;
        MapValues(T container, std::function<OUT(const IN &)> map_fn)
            : container_(container), map_fn_(map_fn) {}

        MapValues<T, IN, OUT>::iterator begin()
        {
            return MapIterator<typename T::iterator, OUT>(container_.begin(),container_.end(), map_fn_);
        }

        MapValues<T, IN, OUT>::iterator end()
        {
            return MapIterator<typename T::iterator, OUT>(container_.end(),container_.end(), map_fn_);
        }

    private:
        T container_;
        std::function<OUT(const IN &)> map_fn_;
    };

    // Because c++11 doesn't deduce template arguments right, we have to create a helper function
    // to create the MapValues object without having to specify the template arguments.
    template <typename T, typename IN, typename OUT>
    MapValues<T, IN, OUT> map(T container, std::function<OUT(const IN &)> map_fn)
    {
        return MapValues<T, IN, OUT>(container, map_fn);
    }

    template <typename T>
    class RefToContainer
    {
    public:
        typedef typename T::iterator iterator;
        RefToContainer(T &container) : container_(container) {}
        // begin and end
        typename T::iterator begin() { return container_.begin(); }
        typename T::iterator end() { return container_.end(); }

    private:
        T &container_;
    };

    template <typename T>
    RefToContainer<T> ref(T &container)
    {
        return RefToContainer<T>(container);
    }
}