#define BOOST_TEST_MODULE S4Tests
#include <boost/test/included/unit_test.hpp>

#include "bst.hpp"

#include <stdexcept>
#include <string>

using Tree = aydogan::BSTree< unsigned int, std::string >;

BOOST_AUTO_TEST_CASE(empty_tree_test)
{
  Tree tree;

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(push_and_find_test)
{
  Tree tree;

  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_CHECK(!tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_REQUIRE(tree.find(1) != tree.end());
  BOOST_CHECK_EQUAL(tree.find(1)->second, "one");
  BOOST_CHECK(tree.find(10) == tree.end());
}

BOOST_AUTO_TEST_CASE(update_existing_key_test)
{
  Tree tree;

  tree.push(5, "five");
  tree.push(5, "FIVE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(5), "FIVE");
}

BOOST_AUTO_TEST_CASE(iteration_order_test)
{
  Tree tree;

  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(5, "five");
  tree.push(1, "one");
  tree.push(3, "three");

  unsigned int expected[] = {1, 2, 3, 4, 5};
  std::size_t index = 0;

  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK_EQUAL(it->first, expected[index]);
    ++index;
  }

  BOOST_CHECK_EQUAL(index, 5);
}

BOOST_AUTO_TEST_CASE(drop_leaf_test)
{
  Tree tree;

  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_CHECK_EQUAL(tree.drop(1), "one");
  BOOST_CHECK(tree.find(1) == tree.end());
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(drop_node_with_two_children_test)
{
  Tree tree;

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");

  BOOST_CHECK_EQUAL(tree.drop(7), "seven");
  BOOST_CHECK(tree.find(7) == tree.end());
  BOOST_CHECK(tree.find(6) != tree.end());
  BOOST_CHECK(tree.find(8) != tree.end());
  BOOST_CHECK_EQUAL(tree.size(), 4);
}

BOOST_AUTO_TEST_CASE(copy_independence_test)
{
  Tree first;

  first.push(1, "one");
  first.push(2, "two");

  Tree second(first);

  second.push(3, "three");
  second.drop(1);

  BOOST_CHECK(first.find(1) != first.end());
  BOOST_CHECK(first.find(3) == first.end());
  BOOST_CHECK(second.find(1) == second.end());
  BOOST_CHECK(second.find(3) != second.end());
}

BOOST_AUTO_TEST_CASE(height_test)
{
  Tree tree;

  BOOST_CHECK_EQUAL(tree.height(), 0);

  tree.push(2, "two");
  BOOST_CHECK_EQUAL(tree.height(), 1);

  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_left_test)
{
  Tree tree;

  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(30, "thirty");

  auto pivot = tree.find(20);
  tree.rotateLeft(pivot);

  BOOST_CHECK_EQUAL(tree.height(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
  BOOST_CHECK_EQUAL(tree.get(30), "thirty");
}

BOOST_AUTO_TEST_CASE(rotate_right_test)
{
  Tree tree;

  tree.push(30, "thirty");
  tree.push(20, "twenty");
  tree.push(10, "ten");

  auto pivot = tree.find(20);
  tree.rotateRight(pivot);

  BOOST_CHECK_EQUAL(tree.height(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
  BOOST_CHECK_EQUAL(tree.get(30), "thirty");
}

BOOST_AUTO_TEST_CASE(rotate_large_left_test)
{
  Tree tree;

  tree.push(10, "ten");
  tree.push(30, "thirty");
  tree.push(20, "twenty");

  auto pivot = tree.find(20);
  tree.rotateLargeLeft(pivot);

  BOOST_CHECK_EQUAL(tree.height(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
  BOOST_CHECK_EQUAL(tree.get(30), "thirty");

  unsigned int expected[] = {10, 20, 30};
  std::size_t index = 0;

  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK_EQUAL(it->first, expected[index]);
    ++index;
  }

  BOOST_CHECK_EQUAL(index, 3);
}

BOOST_AUTO_TEST_CASE(rotate_large_right_test)
{
  Tree tree;

  tree.push(30, "thirty");
  tree.push(10, "ten");
  tree.push(20, "twenty");

  auto pivot = tree.find(20);
  tree.rotateLargeRight(pivot);

  BOOST_CHECK_EQUAL(tree.height(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
  BOOST_CHECK_EQUAL(tree.get(30), "thirty");

  unsigned int expected[] = {10, 20, 30};
  std::size_t index = 0;

  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK_EQUAL(it->first, expected[index]);
    ++index;
  }

  BOOST_CHECK_EQUAL(index, 3);
}

BOOST_AUTO_TEST_CASE(invalid_rotation_test)
{
  Tree tree;

  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.rotateLeft(tree.end()), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateRight(tree.end()), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateLeft(tree.find(1)), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateRight(tree.find(1)), std::logic_error);
}
