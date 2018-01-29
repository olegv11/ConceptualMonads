#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "../Monad.h"
#include "../VectorM.h"

using ::testing::ElementsAre;

TEST(VectorMonadTest, VectorShouldCorrectlyBind)
{
    std::vector<int> v{1, 2};

    auto result = bindAll(v,
        [](int x) { return std::vector<int>{x, x*10}; },
        [](int x) { return std::vector<std::string>{std::to_string(x)}; }
    );

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result[0], "1");
    ASSERT_EQ(result[1], "10");
    ASSERT_EQ(result[2], "2");
    ASSERT_EQ(result[3], "20");
}


TEST(VectorMonadTest, VectorShouldCorrectlyFmap)
{
    std::vector<int> v{4,5};

    auto result = fmap([](int x) { return x * -1;}, v);

    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], -4);
    ASSERT_EQ(result[1], -5);
}
