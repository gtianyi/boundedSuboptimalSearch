#include "../../BoundedSuboptimalSearch.hpp"
#include "../../domain/SlidingTilePuzzle.h"
#include "../RBTree.h"
#include <gtest/gtest.h>

namespace test {
typedef BoundedSuboptimalSearch<SlidingTilePuzzle>::Node SearchNode;

TEST(RBTree, insertion)
{
    SearchNode* sNode =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    bst.insert(sNode);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getRoot()->data->getGValue(), 8);
}

TEST(RBTree, print)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);
    // bst.prettyPrint();
    EXPECT_EQ(bst.searchTree(sNode25), bst.getTNULL());
}

TEST(RBTree, initilizeCursorLessThanMin)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    SearchNode* dummyfhatmin =
      new SearchNode(6, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    bst.insert(sNode8);
    bst.insert(sNode15);
    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursor()->data->getGValue(), 8);
}

TEST(RBTree, initilizeCursorGreaterThanMax)
{
    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    SearchNode* sNode15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    SearchNode* dummyfhatmin =
      new SearchNode(20, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    bst.insert(sNode8);
    bst.insert(sNode15);
    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursor()->data->getGValue(), 15);
}

TEST(RBTree, initilizeCursorInBetweenMinMax)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    SearchNode*         sNode8 =
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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);

    SearchNode* dummyfhatmin =
      new SearchNode(14, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    // bst.prettyPrint();

    EXPECT_EQ(bst.getCursor()->data->getGValue(), 15);
}

TEST(RBTree, deleteCursorNode)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);

    SearchNode* dummyfhatmin =
      new SearchNode(14, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    bst.deleteNode(sNode15);
    // bst.prettyPrint();

    EXPECT_EQ(bst.getCursor()->data->getGValue(), 17);
}

TEST(RBTree, deleteCursorNodeAtMostLeft)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);

    SearchNode* dummyfhatmin =
      new SearchNode(4, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    bst.deleteNode(sNode5);
    // bst.prettyPrint();

    EXPECT_EQ(bst.getCursor()->data->getGValue(), 8);
}

TEST(RBTree, deleteCursorNodeAtMostRight)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);

    SearchNode* dummyfhatmin =
      new SearchNode(85, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    bst.deleteNode(sNode80);
    // bst.prettyPrint();

    EXPECT_EQ(bst.getCursor()->data->getGValue(), 40);
}

TEST(RBTree, updateCursorInEmptyTree)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* dummyfhatmin =
      new SearchNode(85, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursor(), bst.getTNULL());
}

TEST(RBTree, increaseCursor)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    SearchNode* dummyfhatmin14 =
      new SearchNode(14, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* dummyfhatmin26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin14, isIncrease);
    auto items = bst.updateCursor(dummyfhatmin26, isIncrease);

    EXPECT_TRUE(isIncrease);
    EXPECT_EQ(items.size(), 4);

    /*bst.prettyPrint();*/
    // for (const auto& i : items) {
    // cout << *i << endl;
    /*}*/
}

TEST(RBTree, decreaseCursor)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    SearchNode* dummyfhatmin14 =
      new SearchNode(14, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* dummyfhatmin26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin26, isIncrease);
    auto items = bst.updateCursor(dummyfhatmin14, isIncrease);

    EXPECT_FALSE(isIncrease);
    EXPECT_EQ(items.size(), 4);

    /*bst.prettyPrint();*/
    // for (const auto& i : items) {
    // cout << *i << endl;
    /*}*/
}

TEST(RBTree, setCursorAsOneTreeNodeValue)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    SearchNode* dummyfhatmin15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin15, isIncrease);

    // because we want all node on the left of the cursor, not include the
    // cursor
    EXPECT_EQ(bst.getCursor()->data->getGValue(), 17);
}

TEST(RBTree, decreaseCursorEmptyReturn)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    SearchNode* dummyfhatmin27 =
      new SearchNode(27, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* dummyfhatmin26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin27, isIncrease);
    auto items = bst.updateCursor(dummyfhatmin26, isIncrease);

    EXPECT_FALSE(isIncrease);
    EXPECT_EQ(items.size(), 0);
}

TEST(RBTree, increaseCursorEmptyReturn)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    SearchNode* dummyfhatmin27 =
      new SearchNode(27, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* dummyfhatmin26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin26, isIncrease);
    auto items = bst.updateCursor(dummyfhatmin27, isIncrease);

    // because the real cursor for 26 is 40,
    // so it is really decrease to 27 from 40
    EXPECT_FALSE(isIncrease);
    EXPECT_EQ(items.size(), 0);
}

TEST(RBTree, sizeAfterInsertion)
{
    SearchNode* sNode =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);
    EXPECT_TRUE(bst.empty());
    bst.insert(sNode);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getRoot()->data->getGValue(), 8);
    EXPECT_EQ(bst.getSize(), 1);
}

TEST(RBTree, sizeAfterDeleteion)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

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

    bst.insert(sNode8);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    EXPECT_EQ(bst.getSize(), 8);

    SearchNode* dummyfhatmin27 =
      new SearchNode(27, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* dummyfhatmin26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease;
    bst.updateCursor(dummyfhatmin26, isIncrease);
    auto items = bst.updateCursor(dummyfhatmin27, isIncrease);

    EXPECT_EQ(bst.getSize(), 8);
    bst.deleteNode(sNode25);
    EXPECT_EQ(bst.getSize(), 7);
    EXPECT_FALSE(bst.empty());
}

TEST(RBTree, sameValueTreeNode)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
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

    bst.insert(sNode8a);
    bst.insert(sNode8b);
    bst.insert(sNode5);
    bst.insert(sNode15);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);

    bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 8);
    bst.deleteNode(sNode8a);

    bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 7);
    EXPECT_FALSE(bst.empty());
}

} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
