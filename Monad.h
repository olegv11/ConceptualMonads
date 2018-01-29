#pragma once
#include <type_traits>
#include <utility>
#include <functional>

// thanks to http://cukic.co/2017/03/23/cxx-concepts-for-receiving-functions/
template <typename R, typename F, typename ...Args>
concept bool CallableR =
    std::is_invocable_r_v<R, F, Args...>;

template <typename F, typename ...Args>
concept bool Callable =
    std::is_invocable_v<F, Args...>;

// A class which, when specialized on M, should contain the definitions
// for pure and bind of a monad M.
template <template <typename, typename...> typename M, typename T, typename ...Args>
struct MonadDetails;

// Dummy classes that represent "any" type
struct Dummy{};
struct Dummy2{};

// pure/return :: a -> m a
// Something is PurableTo<M,T,Args...> if there exists a function "pure"
// that takes in T and produces an M<T,Args...>
template <template <typename, typename...> typename M, typename T, typename ...Args>
concept bool PurableTo = requires(T t)
{
    { MonadDetails<M,T,Args...>::pure(t) } -> M<T, Args...>;
};

// M is Purable if we can construct M A from any A.
// As C++ can't really express a concept of "any A", we use a Dummy class.
template <template <typename, typename...> typename M, typename ...Args>
concept bool Purable = PurableTo<M,Dummy, Args...>;


// T -> M Dummy
template<template <typename, typename...> typename M, typename T, typename ...Args>
M<Dummy, Args...> ToDummy(T)
{
    return MonadDetails<M,Dummy,Args...>::pure(Dummy{});
}

// bind/>>= :: m a -> (a -> m b) -> m b
// Something is BindableTo<M,T,O,F,Args...> if there exists a function bind
// that takes in M<T, Args...> and a function F and produces an M<O>
template <template <typename, typename...> typename M, typename T, typename O, typename F, typename ...Args>
concept bool BindableTo = requires(M<T, Args...> m, F f)
{
        { MonadDetails<M,T,Args...>::template bind<O,F>(m,f) } -> M<O>;
};

// M is Bindable if for any A and B (here Dummy2 and Dummy) and any
// function A -> M B there is a function "bind"
template <template <typename, typename...> typename M, typename ...Args>
concept bool Bindable = BindableTo<M, Dummy2, Dummy, decltype(ToDummy<M, Dummy2>), Args...>;

// A monad M is something that is Purable and Bindable. That is, in Haskell:
// class Monad m where
//   (>>=)  :: m a -> (  a -> m b) -> m b
//   return ::   a                 -> m a
template<template <typename, typename...> typename M, typename ...Args>
concept bool Monad = Purable<M, Args...> && Bindable<M, Args...>;

// Default implementation of >>=
template <template <typename, typename...> typename M, typename T, typename F, typename ...Args>
requires Callable<F, T>
auto operator>>=(M<T, Args...> m, F f) -> typename std::invoke_result_t<F,T>
{
    using Details = MonadDetails<M, T, Args...>;
    using Res = typename std::invoke_result_t<F,T>::value_type;
    return Details::template bind<Res, F>(m, f);
}

// Something that resembles do-notation
template <typename ...Args>
auto bindAll(Args... args)
{
    return (... >>= args);
}

// Default implementation of fmap
template<Monad M, typename T, typename F, typename ...Args>
requires Callable<F, T>
auto fmap(F f, M<T, Args...> m) -> M<typename std::invoke_result_t<F,T>>
{
    return m >>= [f](T t) {
        return MonadDetails<M, typename std::invoke_result_t<F,T>>::pure(std::invoke(f, t));
    };
}
