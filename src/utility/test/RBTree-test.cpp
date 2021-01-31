#include "../RBTree.h"
#include <gtest/gtest.h>

namespace test {
TEST(RBTree, insertion)
{
    RBTree bst;
    bst.insert(8);
    EXPECT_EQ(bst.getRoot()->data, 8);
}
} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
