#pragma once

template <typename A, typename S>
struct ImplState
{
    using value_type = A;

    std::function<std::pair<A,S>(S)> stateFunction;

    ImplState(std::function<std::pair<A,S>(S)> stateFun)
        :stateFunction(stateFun)
    {
    }

    ImplState(const ImplState<A,S>& other) 
        :stateFunction(other.stateFunction)
    {
    }

    ImplState(ImplState<A,S>&& other)
        :stateFunction(std::move(other.stateFunction))
    {
    }

    ImplState(std::pair<A,S> initialState)
        :ImplState([initialState](S s) { return initialState; })
    {
    }

    auto runState(S s) -> std::pair<A,S>
    {
        return std::invoke(stateFunction, s);
    }

    auto evalState(S s) -> A
    {
        return runState(s).first;
    }

    auto execState(S s) -> S
    {
        return runState(s).second;
    }
};

template <typename S>
auto put(S s) -> ImplState<std::tuple<>, S>
{
    return ImplState<std::tuple<>, S>([s](S s) -> std::pair<std::tuple<>,S> { return std::make_pair(std::make_tuple(), s); });
}

template <typename S>
auto get() -> ImplState<S,S>
{
    return ImplState<S,S>([](S s) { return std::make_pair(s,s); });
}


template <typename A, typename S>
struct MonadDetails<ImplState, A, S>
{
    static auto pure(A a) -> ImplState<A,S>
    {
        return ImplState<A, S>([a](S s) { return std::make_pair(a,s); });
    }

    template <typename B, typename F>
    requires CallableR<ImplState<B,S>, F, A>
    static auto bind(ImplState<A,S> m, F f) -> ImplState<B,S>
    {
        return ImplState<B,S>([m,f](S s0) mutable -> std::pair<B,S> {
            auto [x, s1] = m.runState(s0);
            ImplState<B,S> processed = std::invoke(f, x);

            return processed.runState(s1);
        });
    }
};


template <typename S, typename A>
using State = ImplState<A,S>;
