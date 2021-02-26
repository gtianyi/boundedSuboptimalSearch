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
    bst.checkTreePropertyRedKidsAreRed();
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
    EXPECT_EQ(bst.getCursorNode()->data, sNode8);
    EXPECT_EQ(bst.getCursorValue(), 6);
    EXPECT_EQ(bst.getCursorStatus(), 1);
    bst.checkTreePropertyRedKidsAreRed();
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
    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 20);
    EXPECT_EQ(bst.getCursorStatus(), 2);
    bst.checkTreePropertyRedKidsAreRed();
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode15);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);
    bst.checkTreePropertyRedKidsAreRed();
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode17);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.checkTreePropertyRedKidsAreRed();
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
    EXPECT_EQ(bst.getCursorValue(), 4);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getCursorStatus(), 1);
    EXPECT_EQ(bst.getCursorNode()->data, sNode8);
}

TEST(RBTree, deleteCursorNodeAtMostLeftExact)
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
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode8);
    EXPECT_EQ(bst.getCursorValue(), 5);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode5);
    // bst.prettyPrint();
    // this is a special case where when delete the most left node
    // if the cursor is the successor of the most left
    // we keep it there, instead of move it to the left outside of the tree
    // otherwise would be expansive to chase this specitial case
    EXPECT_EQ(bst.getCursorNode()->data, sNode8);
    EXPECT_EQ(bst.getCursorValue(), 5);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getCursorStatus(), 1);
}

TEST(RBTree, deleteNodeAtMostRight)
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

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 85);
    EXPECT_EQ(bst.getCursorStatus(), 2);

    bst.deleteNode(sNode80);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 85);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getCursorStatus(), 2);
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
      new SearchNode(79, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode80);
    EXPECT_EQ(bst.getCursorValue(), 79);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode80);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 79);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getCursorStatus(), 2);
}

TEST(RBTree, deleteDupNodeAndSetCursor)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8 =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
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
    bst.insert(sNode15a);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode40);
    bst.insert(sNode80);
    bst.deleteNode(sNode25);

    SearchNode* dummyfhatmin =
      new SearchNode(14, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode15a);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.insert(sNode15b);

    EXPECT_EQ(bst.getCursorNode()->data, sNode15a);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode15a);

    EXPECT_EQ(bst.getCursorNode()->data, sNode15b);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode15b);

    EXPECT_EQ(bst.getCursorNode()->data, sNode17);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.checkTreePropertyRedKidsAreRed();
}

TEST(RBTree, insertNodeAndSetCursorAtRightMost)
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
      new SearchNode(79, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode80);
    EXPECT_EQ(bst.getCursorValue(), 79);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode80);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 79);
    EXPECT_EQ(bst.getCursorStatus(), 2);

    bst.insert(sNode80);

    EXPECT_EQ(bst.getCursorNode()->data, sNode80);
    EXPECT_EQ(bst.getCursorValue(), 79);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.checkTreePropertyRedKidsAreRed();
}

TEST(RBTree, insertNodeAndUpdateCursorInMiddle)
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode15);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode15);

    EXPECT_EQ(bst.getCursorNode()->data, sNode17);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.insert(sNode15);

    EXPECT_EQ(bst.getCursorNode()->data, sNode15);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.checkTreePropertyRedKidsAreRed();
}

TEST(RBTree, insertNodeAndUpdateCursorAtLeftMost)
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode5);
    EXPECT_EQ(bst.getCursorValue(), 4);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(sNode5);

    EXPECT_EQ(bst.getCursorNode()->data, sNode8);
    EXPECT_EQ(bst.getCursorValue(), 4);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.insert(sNode5);

    EXPECT_EQ(bst.getCursorNode()->data, sNode5);
    EXPECT_EQ(bst.getCursorValue(), 4);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.checkTreePropertyRedKidsAreRed();
}

TEST(RBTree, updateCursorInEmptyTree)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* dummyfhatmin =
      new SearchNode(85, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), -1);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getCursorStatus(), 0);
}

TEST(RBTree, deleteLastNodeAndUnsetCursor)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* node17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* node15 =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    SearchNode* dummyfhatmin =
      new SearchNode(16, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(node17);

    bool isIncrease = true;
    bst.updateCursor(dummyfhatmin, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, node17);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(node17);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 0);

    bst.insert(node17);

    EXPECT_EQ(bst.getCursorNode()->data, node17);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    bst.deleteNode(node17);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 0);

    bst.insert(node15);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 2);

    bst.deleteNode(node15);

    EXPECT_EQ(bst.getCursorNode(), nullptr);
    EXPECT_EQ(bst.getCursorValue(), 16);
    EXPECT_EQ(bst.getCursorStatus(), 0);

    bst.checkTreePropertyRedKidsAreRed();
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode15);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    auto items = bst.updateCursor(dummyfhatmin26, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 26);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    EXPECT_TRUE(isIncrease);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(items.size(), 4);
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

    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 26);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    auto items = bst.updateCursor(dummyfhatmin14, isIncrease);

    // bst.prettyPrint();
    EXPECT_EQ(bst.getCursorNode()->data, sNode15);
    EXPECT_EQ(bst.getCursorValue(), 14);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    EXPECT_FALSE(isIncrease);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(items.size(), 4);
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
    EXPECT_EQ(bst.getCursorNode()->data, sNode17);
    EXPECT_EQ(bst.getCursorValue(), 15);
    EXPECT_EQ(bst.getCursorStatus(), 1);
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

    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 27);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    auto items = bst.updateCursor(dummyfhatmin26, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 26);
    EXPECT_EQ(bst.getCursorStatus(), 1);

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

    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 26);
    EXPECT_EQ(bst.getCursorStatus(), 1);

    auto items = bst.updateCursor(dummyfhatmin27, isIncrease);

    EXPECT_EQ(bst.getCursorNode()->data, sNode40);
    EXPECT_EQ(bst.getCursorValue(), 27);
    EXPECT_EQ(bst.getCursorStatus(), 1);

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

    bst.checkTreePropertyRedKidsAreRed();
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

    bst.checkTreePropertyRedKidsAreRed();
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

    // bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 8);
    bst.deleteNode(sNode8a);

    // bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 7);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_FALSE(bst.empty());
}

TEST(RBTree, allsameValueTreeNode)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8c =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8d =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8e =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8f =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8g =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8h =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode8b);
    bst.insert(sNode8c);
    bst.insert(sNode8d);
    bst.insert(sNode8e);
    bst.insert(sNode8f);
    bst.insert(sNode8g);
    bst.insert(sNode8h);

    // cout << sNode8a << endl;
    // cout << sNode8b << endl;
    // cout << sNode8c << endl;
    // cout << sNode8d << endl;
    // cout << sNode8e << endl;
    // cout << sNode8f << endl;
    // cout << sNode8g << endl;
    // cout << sNode8h << endl;

    // bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 8);
    bst.deleteNode(sNode8h);
    EXPECT_EQ(bst.getSize(), 7);
    bst.deleteNode(sNode8g);
    EXPECT_EQ(bst.getSize(), 6);
    bst.deleteNode(sNode8a);
    EXPECT_EQ(bst.getSize(), 5);
    bst.deleteNode(sNode8d);
    EXPECT_EQ(bst.getSize(), 4);
    bst.deleteNode(sNode8d);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getSize(), 4);
}

TEST(RBTree, insertAnExistingNode)
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
    // bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 8);
    bst.insert(sNode17);
    // bst.prettyPrint();

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(bst.getSize(), 8);
}

TEST(RBTree, deleteWholeTree)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80a =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80b =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15a);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode15b);
    bst.insert(sNode40);
    bst.insert(sNode80a);
    bst.insert(sNode8b);
    bst.insert(sNode80b);
    // bst.prettyPrint();
    EXPECT_EQ(bst.getSize(), 11);
    bst.deleteNode(sNode8a);
    bst.deleteNode(sNode18);
    bst.deleteNode(sNode40);
    bst.deleteNode(sNode5);
    bst.deleteNode(sNode25);
    bst.deleteNode(sNode15b);
    bst.deleteNode(sNode80a);
    bst.deleteNode(sNode8b);
    bst.deleteNode(sNode80b);
    bst.deleteNode(sNode17);
    bst.deleteNode(sNode15a);

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_TRUE(bst.empty());
}

TEST(RBTree, elementList)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17 =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80a =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80b =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15a);
    bst.insert(sNode17);
    bst.insert(sNode25);
    bst.insert(sNode15b);
    bst.insert(sNode40);
    bst.insert(sNode80a);
    bst.insert(sNode8b);
    bst.insert(sNode80b);

    auto elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 11);
}

TEST(RBTree, chainAtRoot)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17a =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17b =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80a =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80b =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15a);
    bst.insert(sNode17a);
    bst.insert(sNode17b);
    bst.insert(sNode25);
    bst.insert(sNode15b);
    bst.insert(sNode40);
    bst.insert(sNode80a);
    bst.insert(sNode8b);
    bst.insert(sNode80b);

    bst.checkTreePropertyRedKidsAreRed();

    auto elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 12);
    // bst.prettyPrint();
    bst.deleteNode(sNode17b);
    // bst.prettyPrint();
    elementList = bst.getList();

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(elementList.size(), 11);
}

TEST(RBTree, deepTreeRootDeletion)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18 =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17a =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17b =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80a =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80b =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode90 =
      new SearchNode(90, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode91 =
      new SearchNode(91, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode92 =
      new SearchNode(92, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode93 =
      new SearchNode(93, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode94 =
      new SearchNode(94, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode95 =
      new SearchNode(95, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode96 =
      new SearchNode(96, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode97 =
      new SearchNode(97, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode18);
    bst.insert(sNode5);
    bst.insert(sNode15a);
    bst.insert(sNode17a);
    bst.insert(sNode17b);
    bst.insert(sNode25);
    bst.insert(sNode15b);
    bst.insert(sNode40);
    bst.insert(sNode80a);
    bst.insert(sNode8b);
    bst.insert(sNode80b);
    bst.insert(sNode90);
    bst.insert(sNode91);
    bst.insert(sNode92);
    bst.insert(sNode93);
    bst.insert(sNode94);
    bst.insert(sNode95);
    bst.insert(sNode96);
    bst.insert(sNode97);

    bst.checkTreePropertyRedKidsAreRed();

    auto elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 20);
    // bst.prettyPrint();
    bst.deleteNode(sNode80a);
    // bst.prettyPrint();
    bst.deleteNode(sNode80b);
    // bst.prettyPrint();
    elementList = bst.getList();

    bst.checkTreePropertyRedKidsAreRed();
    EXPECT_EQ(elementList.size(), 18);
}

TEST(RBTree, deepTreeRootDeletRoot)
{
    RBTree<SearchNode*> bst(SearchNode::compareNodesF);

    SearchNode* sNode8a =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode8b =
      new SearchNode(8, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18a =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode18b =
      new SearchNode(18, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode5 =
      new SearchNode(5, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15a =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode15b =
      new SearchNode(15, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17a =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode17b =
      new SearchNode(17, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode25 =
      new SearchNode(25, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode40 =
      new SearchNode(40, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80a =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode80b =
      new SearchNode(80, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode26 =
      new SearchNode(26, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode27 =
      new SearchNode(27, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode28 =
      new SearchNode(28, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode29 =
      new SearchNode(29, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode30 =
      new SearchNode(30, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);
    SearchNode* sNode31 =
      new SearchNode(31, 0, 0, 0, 0, 0, SlidingTilePuzzle::State(), NULL);

    bst.insert(sNode8a);
    bst.insert(sNode18a);
    bst.insert(sNode18b);
    bst.insert(sNode5);
    bst.insert(sNode15a);
    bst.insert(sNode17a);
    bst.insert(sNode17b);
    bst.insert(sNode25);
    bst.insert(sNode15b);
    bst.insert(sNode40);
    bst.insert(sNode80a);
    bst.insert(sNode8b);
    bst.insert(sNode80b);
    bst.insert(sNode26);
    bst.insert(sNode27);
    bst.insert(sNode28);
    bst.insert(sNode29);
    bst.insert(sNode30);
    bst.insert(sNode31);

    bst.checkTreePropertyRedKidsAreRed();

    auto elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 19);
    bst.deleteNode(sNode26);
    // bst.prettyPrint();

    bst.checkTreePropertyRedKidsAreRed();
    bst.deleteNode(sNode17a);
    // bst.prettyPrint();

    bst.checkTreePropertyRedKidsAreRed();
    bst.deleteNode(sNode17b);
    // bst.prettyPrint();
    elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 16);
    bst.deleteNode(sNode18a);
    bst.deleteNode(sNode18b);
    // bst.prettyPrint();
    elementList = bst.getList();
    EXPECT_EQ(elementList.size(), 14);
    bst.checkTreePropertyRedKidsAreRed();
}

} // namespace test

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
