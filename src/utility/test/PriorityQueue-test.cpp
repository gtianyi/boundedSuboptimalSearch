#include "../../BoundedSuboptimalSearch.hpp"
#include "../../domain/SlidingTilePuzzle.h"
#include "../PriorityQueue.h"
#include <gtest/gtest.h>

namespace test {
typedef BoundedSuboptimalSearch<SlidingTilePuzzle>::Node SearchNode;

TEST(PriorityQueue, insertion)
{
    SearchNode* sNode =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    PriorityQueue<SearchNode*> pQueue(SearchNode::compareNodesF);
    pQueue.push(sNode);
    // bst.prettyPrint();
    EXPECT_EQ(pQueue.top()->getGValue(), 8);
}
} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
