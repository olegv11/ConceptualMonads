#pragma once

#include <vector>
#include <array>


template <typename T, typename ...Args>
struct MonadDetails<std::vector, T, Args...>
{
    static auto pure(T t) -> std::vector<T, Args...>
    {
        return std::vector<T, Args...>({t});
    }


    // TODO: Find a way to construct a vector<O> with custom allocator
    template <typename O, typename F>
    requires CallableR<std::vector<O>, F, T>
    static auto bind(std::vector<T, Args...> m, F f) -> std::vector<O>
    {
        std::vector<O> result;
        result.reserve(m.size());

        for (auto& t: m)
        {
            std::vector<O> mapped = std::invoke(f, t);
            result.insert(std::end(result), std::begin(mapped), std::end(mapped));
        }

        return result;
    }
};
