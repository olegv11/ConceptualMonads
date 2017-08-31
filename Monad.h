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

template <template <typename> typename M, typename T>
struct MonadDetails
{
};

struct Dummy{};
struct Dummy2{};


template <template <typename> typename M, typename T>
concept bool PurableTo = requires(T t) {
    { MonadDetails<M,T>::pure(t) } -> M<T>;
};



template <template <typename> typename M>
concept bool Purable = PurableTo<M,Dummy>;

template<template <typename> typename M, typename T>
M<Dummy> ToDummy(T)
{
    return MonadDetails<M,Dummy>::pure(Dummy{});
}


template <template <typename> typename M, typename T, typename O, typename F>
concept bool BindableTo = requires(M<T> m, F f)
{
        { MonadDetails<M,T>::template bind<O,F>(m,f) };
};

template <template <typename> typename M>
concept bool Bindable = BindableTo<M, Dummy2, Dummy, decltype(ToDummy<M, Dummy2>)>;


template<template <typename> typename M>
concept bool Monad = Purable<M> && Bindable<M>;


template <template <typename> typename M, typename T, typename F>
auto operator>>=(M<T> m, F f) -> typename std::invoke_result<F,T>::type
{
    using Details = MonadDetails<M, T>;
    using Res = typename std::invoke_result<F,T>::type::value_type;

    return Details::template bind<Res, F>(m, f);
}

template<Monad M, typename T, typename F, typename O = typename std::invoke_result_t<F,T>>
requires Callable<F, T>
auto fmap(F f, M<T> m) -> M<T> 
{
    return m >>= [f](T t) {
        return MonadDetails<M,O>::pure(std::invoke(f, t));
    };
}
