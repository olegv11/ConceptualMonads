#include "Monad.h"
#include "OptionalM.h"
#include "VectorM.h"
#include "StateM.h"

#include <string>
#include <iostream>
#include <vector>
#include <deque>

std::optional<int> opt_to_string(std::string s)
{
    try
    {
        return std::stoi(s);
    }
    catch(...)
    {
        return std::nullopt;
    }
}

using Stack = std::deque<int>;
using Empty = std::tuple<>;

State<Stack, int> pop()
{
    return State<Stack, int>([](Stack stack) {
        int result = stack.front();
        stack.pop_front();

        return std::make_pair(result, stack);
    });
}

State<Stack, Empty> push(int x)
{
    return State<Stack, Empty>([x](Stack stack) {
        stack.push_front(x);

        return std::make_pair(std::make_tuple(), stack);
    });
}


State<Stack, int> stackManip()
{
    return bindAll(push(10),
            [](auto) {return push(20);},
            [](auto) {return pop();}
            );
}


int main()
{
    fmap([](int x) -> int { std::cout << x << std::endl; return x; }, std::make_optional<std::string>("hello") >>= opt_to_string);
    fmap([](int x) -> int { std::cout << x << std::endl; return x; }, std::make_optional<std::string>("1234") >>= opt_to_string);

    std::vector<int> v({2,3});

    auto res = v >>= [](int x) {
        std::vector<int> result;
        for (int i = 0; i < x; i++)
        {
            result.push_back(i);
        }
        return result;
    };


    for (auto& x : res)
    {
        std::cout << x << std::endl;
    }

    int x = stackManip().evalState(std::deque<int>({100}));
    
    std::cout << "Stack value (should be 20): " << x << std::endl;


    return 0;
}
