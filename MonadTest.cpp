#include "Monad.h"
#include "OptionalM.h"
#include "VectorM.h"

#include <string>
#include <iostream>

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

    return 0;
}
