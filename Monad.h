#pragma once
#include <type_traits>
#include <utility>
#include <functional>

// thanks to http://cukic.co/2017/03/23/cxx-concepts-for-receiving-functions/
template <typename R, typename F, typename ...Args>
concept bool CallableR =
    std::is_invocable_r<R, F, Args...>::value;

template <typename F, typename ...Args>
concept bool Callable =
    std::is_invocable<F, Args...>::value;

template <template <typename, typename...> typename M, typename T, typename ...Args>
struct MonadDetails;

struct Dummy{};
struct Dummy2{};


template <template <typename, typename...> typename M, typename T, typename ...Args>
concept bool PurableTo = requires(T t) {
    { MonadDetails<M,T,Args...>::pure(t) } -> M<T, Args...>;
};

template <template <typename, typename...> typename M, typename ...Args>
concept bool Purable = PurableTo<M,Dummy, Args...>;

template<template <typename, typename...> typename M, typename T, typename ...Args>
M<Args..., Dummy> ToDummy(T)
{
    return MonadDetails<M,Dummy,Args...>::pure(Dummy{});
}


template <template <typename, typename...> typename M, typename T, typename O, typename F, typename ...Args>
concept bool BindableTo = requires(M<T, Args...> m, F f)
{
        { MonadDetails<M,T,Args...>::template bind<O,F>(m,f) };
};

template <template <typename, typename...> typename M, typename ...Args>
concept bool Bindable = BindableTo<M, Dummy2, Dummy, decltype(ToDummy<M, Dummy2>), Args...>;


template<template <typename, typename...> typename M, typename ...Args>
concept bool Monad = Purable<M, Args...> && Bindable<M, Args...>;

template <template <typename, typename...> typename M, typename T, typename F, typename ...Args>
requires Callable<F, T>
auto operator>>=(M<T, Args...> m, F f) -> typename std::invoke_result<F,T>::type
{
    using Details = MonadDetails<M, T, Args...>;
    using Res = typename std::invoke_result<F,T>::type::value_type;
    return Details::template bind<Res, F>(m, f);
}

// It's not a replacement for a do-notation, but it'll do.
template <typename ...Args>
auto bindAll(Args... args)
{
    return (... >>= args);
}


template<Monad M, typename T, typename F, typename ...Args>
requires Callable<F, T>
auto fmap(F f, M<T, Args...> m) -> M<typename std::invoke_result_t<F,T>>
{
    return m >>= [f](T t) {
        return MonadDetails<M, typename std::invoke_result_t<F,T>>::pure(std::invoke(f, t));
    };
}
