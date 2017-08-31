#pragma once

#include <optional>

template <typename T>
struct MonadDetails<std::optional, T>
{
    static auto pure(T t) -> std::optional<T>
    {
        return std::make_optional(t);
    }

    template <typename O, typename F>
    requires CallableR<std::optional<O>, F, T>
    static auto bind(std::optional<T> m, F f) -> std::optional<O>
    {
        if (m.has_value())
        {
            return std::invoke(f, m.value());
        }

        return std::nullopt;
    }
};
