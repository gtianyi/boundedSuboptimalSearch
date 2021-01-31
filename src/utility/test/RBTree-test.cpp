#include "../../BoundedSuboptimalSearch.hpp"
#include "../../domain/SlidingTilePuzzle.h"
#include "../RBTree.h"
#include <gtest/gtest.h>

namespace test {
typedef BoundedSuboptimalSearch<SlidingTilePuzzle>::Node SearchNode;

TEST(RBTree, insertion)
{
    SearchNode* cur =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    bst.insert(cur);
    bst.prettyPrint();
    // EXPECT_EQ(bst.getRoot()->data, 8);
}

TEST(RBTree, print)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    SearchNode*         cur8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* cur80 =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(cur8);
    bst.insert(cur18);
    bst.insert(cur5);
    bst.insert(cur15);
    bst.insert(cur17);
    bst.insert(cur25);
    bst.insert(cur40);
    bst.insert(cur80);
    bst.deleteNode(cur25);
    bst.prettyPrint();
}
} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
