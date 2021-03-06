#include "../../BoundedSuboptimalSearch.hpp"
#include "../../domain/SlidingTilePuzzle.h"
#include "../../search/DPS.hpp"
#include "../PriorityQueue.h"
#include <gtest/gtest.h>

namespace test {
typedef BoundedSuboptimalSearch<SlidingTilePuzzle>::Node SearchNode;
typedef DPS<SlidingTilePuzzle, SearchNode>::Bucket       Bucket;

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

    EXPECT_NE(pQueue.find(sNode18), pQueue.end());

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

    EXPECT_EQ(pQueue.find(sNode18), pQueue.end());

    EXPECT_EQ(pQueue.size(), 5);
}

TEST(PriorityQueue, mapSize)
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

    EXPECT_NE(pQueue.find(sNode18), pQueue.end());

    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.getItem2IndexMapValue(sNode5), 0);
    EXPECT_EQ(pQueue.size(), 8);
    EXPECT_EQ(pQueue.getItem2IndexMapSize(), 8);

    pQueue.remove(sNode8);
    EXPECT_EQ(pQueue.top()->getGValue(), 5);
    EXPECT_EQ(pQueue.size(), 7);
    EXPECT_EQ(pQueue.getItem2IndexMapSize(), 7);

    pQueue.pop();

    EXPECT_EQ(pQueue.top()->getGValue(), 15);
    EXPECT_EQ(pQueue.size(), 6);
    EXPECT_EQ(pQueue.getItem2IndexMapSize(), 6);
    EXPECT_EQ(pQueue.getItem2IndexMapValue(sNode15), 0);

    pQueue.remove(sNode18);
    EXPECT_EQ(pQueue.top()->getGValue(), 15);

    EXPECT_EQ(pQueue.find(sNode18), pQueue.end());

    EXPECT_EQ(pQueue.size(), 5);
    EXPECT_EQ(pQueue.getItem2IndexMapSize(), 5);
}

TEST(PriorityQueue, update)
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
    EXPECT_EQ(pQueue.getItem2IndexMapValue(sNode5), 0);

    sNode18->setGValue(4);
    pQueue.update(sNode18);
    EXPECT_EQ(pQueue.top()->getGValue(), 4);
    EXPECT_EQ(pQueue.getItem2IndexMapValue(sNode18), 0);
}

TEST(PriorityQueue, pushBucket)
{
    SearchNode::weight = 1;

    Bucket* bucket8  = new Bucket(8, 1, nullptr, 100);
    Bucket* bucket18 = new Bucket(18, 1, nullptr, 100);
    Bucket* bucket5  = new Bucket(5, 1, nullptr, 100);
    Bucket* bucket15 = new Bucket(15, 1, nullptr, 100);
    Bucket* bucket17 = new Bucket(17, 1, nullptr, 100);
    Bucket* bucket25 = new Bucket(25, 1, nullptr, 100);
    Bucket* bucket40 = new Bucket(40, 1, nullptr, 100);
    Bucket* bucket80 = new Bucket(80, 1, nullptr, 100);

    PriorityQueue<Bucket*> pQueue(Bucket::compareBucketDPS);
    pQueue.push(bucket8);
    pQueue.push(bucket18);
    pQueue.push(bucket5);
    pQueue.push(bucket15);
    pQueue.push(bucket17);
    pQueue.push(bucket25);
    pQueue.push(bucket40);
    pQueue.push(bucket80);

    EXPECT_EQ(pQueue.top()->getG(), 5);
}

TEST(PriorityQueue, copyConstructorBucket)
{
    SearchNode::weight = 1;

    Bucket* bucket8  = new Bucket(8, 1, nullptr, 100);
    Bucket* bucket18 = new Bucket(18, 1, nullptr, 100);
    Bucket* bucket5  = new Bucket(5, 1, nullptr, 100);
    Bucket* bucket15 = new Bucket(15, 1, nullptr, 100);
    Bucket* bucket17 = new Bucket(17, 1, nullptr, 100);
    Bucket* bucket25 = new Bucket(25, 1, nullptr, 100);
    Bucket* bucket40 = new Bucket(40, 1, nullptr, 100);
    Bucket* bucket80 = new Bucket(80, 1, nullptr, 100);

    PriorityQueue<Bucket*> pQueue(Bucket::compareBucketDPS);
    pQueue.push(bucket8);
    pQueue.push(bucket18);
    pQueue.push(bucket5);
    pQueue.push(bucket15);
    pQueue.push(bucket17);
    pQueue.push(bucket25);
    pQueue.push(bucket40);
    pQueue.push(bucket80);

    EXPECT_EQ(pQueue.top()->getG(), 5);

    PriorityQueue<Bucket*> newBucketPq(Bucket::compareBucketDPS);
    newBucketPq.push(bucket18);
    newBucketPq.push(bucket15);
    newBucketPq.push(bucket17);

    while (!pQueue.empty()) {
        pQueue.pop();
    }

    pQueue = newBucketPq;

    EXPECT_EQ(pQueue.top()->getG(), 15);
}

} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
