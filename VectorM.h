#pragma once

#include <vector>
#include <array>


template <typename T>
struct MonadDetails<std::vector, T>
{
    static auto pure(T t) -> std::vector<T>
    {
        return std::vector({t});
    }

    template <typename O, typename F>
    requires CallableR<std::vector<O>, F, T>
    static auto bind(std::vector<T> m, F f) -> std::vector<O>
    {
        std::vector<O> result;
        result.reserve(m.size());

        for (auto& t: m)
        {
            std::vector<O> mapped = std::invoke(f,t);
            result.insert(std::end(result), std::begin(mapped), std::end(mapped));
        }

        return result;
    }
};
