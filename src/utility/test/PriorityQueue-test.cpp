#include "../../BoundedSuboptimalSearch.hpp"
#include "../../domain/SlidingTilePuzzle.h"
#include "../PriorityQueue.h"
#include <gtest/gtest.h>

namespace test {
typedef BoundedSuboptimalSearch<SlidingTilePuzzle>::Node SearchNode;

TEST(PriorityQueue, push)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80 =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    PriorityQueue<SearchNode*> pQueue(SearchNode::compareNodesF);
    pQueue.push(sNode8);
    pQueue.push(sNode18);
    pQueue.push(sNode5);
    pQueue.push(sNode15);
    pQueue.push(sNode17);
    pQueue.push(sNode25);
    pQueue.push(sNode40);
    pQueue.push(sNode80);

    EXPECT_EQ(pQueue.top()->getGValue(), 5);
}

TEST(PriorityQueue, pop)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80 =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    PriorityQueue<SearchNode*> pQueue(SearchNode::compareNodesF);
    pQueue.push(sNode8);
    pQueue.push(sNode18);
    pQueue.push(sNode5);
    pQueue.push(sNode15);
    pQueue.push(sNode17);
    pQueue.push(sNode25);
    pQueue.push(sNode40);
    pQueue.push(sNode80);

    // bst.prettyPrint();
    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 8);

    pQueue.pop();

    EXPECT_EQ(pQueue.top()->getGValue(), 8);
    EXPECT_EQ(pQueue.size(), 7);
}

TEST(PriorityQueue, remove)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80 =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    PriorityQueue<SearchNode*> pQueue(SearchNode::compareNodesF);
    pQueue.push(sNode8);
    pQueue.push(sNode18);
    pQueue.push(sNode5);
    pQueue.push(sNode15);
    pQueue.push(sNode17);
    pQueue.push(sNode25);
    pQueue.push(sNode40);
    pQueue.push(sNode80);

    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 8);

    pQueue.remove(sNode8);
    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 7);

    pQueue.pop();

    EXPECT_EQ(pQueue.top()->getGValue(), 15);
    EXPECT_EQ(pQueue.size(), 6);
}

TEST(PriorityQueue, find)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80 =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    PriorityQueue<SearchNode*> pQueue(SearchNode::compareNodesF);
    pQueue.push(sNode8);
    pQueue.push(sNode18);
    pQueue.push(sNode5);
    pQueue.push(sNode15);
    pQueue.push(sNode17);
    pQueue.push(sNode25);
    pQueue.push(sNode40);
    pQueue.push(sNode80);


    EXPECT_NE(pQueue.find(sNode18),pQueue.end());

    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 8);

    pQueue.remove(sNode8);
    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 7);

    pQueue.pop();

    EXPECT_EQ(pQueue.top()->getGValue(), 15);
    EXPECT_EQ(pQueue.size(), 6);

    pQueue.remove(sNode18);
    EXPECT_EQ(pQueue.top()->getGValue(), 15);

    EXPECT_EQ(pQueue.find(sNode18),pQueue.end());

    EXPECT_EQ(pQueue.size(), 5);
}

} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
