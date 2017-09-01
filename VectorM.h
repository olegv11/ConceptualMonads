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

    template <typename O, typename F>
    requires CallableR<std::vector<O, Args...>, F, T>
    static auto bind(std::vector<T> m, F f) -> std::vector<O, Args...>
    {
        std::vector<O, Args...> result;
        result.reserve(m.size());

        for (auto& t: m)
        {
            std::vector<O> mapped = std::invoke(f,t);
            result.insert(std::end(result), std::begin(mapped), std::end(mapped));
        }

        return result;
    }
};
