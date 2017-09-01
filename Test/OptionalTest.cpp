#include <gtest/gtest.h> 
#include <string>
#include "../Monad.h"
#include "../OptionalM.h"

TEST(OptionalMonadTest, OptionalMonadShouldCorrectlyBind)
{
    std::optional<int> x = 1;
    auto result = bindAll(x,
            [](int y) { return std::optional<int>(y + 2); },
            [](int z) { return std::optional<int>(z * -4); }
    );
    
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value(), -12);
}


TEST(OptionalMonadTest, OptionalMonadShouldCorrectlyPassThroughNullopt)
{
    std::optional<int> x = 10;
    auto result = bindAll(x,
            [](int y) { return std::optional<int>(y + 2); },
            [](int z) -> std::optional<int> { return std::nullopt; },
            [](int w) { return std::optional<int>(w % 2); }
    );
    
    ASSERT_FALSE(result.has_value());
}


TEST(OptionalMonadTest, OptionalMonadShouldCorrectlyPropagateTyps)
{
    std::optional<int> x = 123;
    auto result = x >>=
            ([](int y) { return std::optional<std::string>(std::to_string(y)); });
    
    
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value(), "123");
}

TEST(OptionalMonadTest, OptionalMonadShouldCorrectlyFmap)
{
    std::optional<std::string> x = "hello";
    
    std::optional<char> result = fmap([](std::string s) -> char { return s[0]; }, x);
    
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value(), 'h');
}
