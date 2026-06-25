#define BOOST_TEST_MODULE S1_List_Tests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include <stdexcept>
#include <utility>

BOOST_AUTO_TEST_CASE(empty_list_test)
{
  aydogan::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  aydogan::List< int > list;
  list.push_front(10);
  list.push_front(20);

  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.front(), 20);

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  aydogan::List< int > list;
  auto it = list.beforeBegin();

  it = list.insertAfter(it, 1);
  it = list.insertAfter(it, 2);
  list.insertAfter(it, 3);

  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK(check == list.end());
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  aydogan::List< int > list;
  auto it = list.beforeBegin();

  it = list.insertAfter(it, 5);
  it = list.insertAfter(it, 10);
  list.insertAfter(it, 15);

  list.eraseAfter(list.beforeBegin());

  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 10);
  ++check;
  BOOST_CHECK_EQUAL(*check, 15);
  ++check;
  BOOST_CHECK(check == list.end());
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  aydogan::List< int > list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);

  BOOST_CHECK_EQUAL(list.front(), 1);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 2);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 3);
  list.pop_front();

  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  aydogan::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  BOOST_CHECK(!list.empty());
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  aydogan::List< int > first;
  auto tail = first.beforeBegin();

  tail = first.insertAfter(tail, 1);
  tail = first.insertAfter(tail, 2);
  first.insertAfter(tail, 3);

  aydogan::List< int > second(first);

  auto it1 = first.cbegin();
  auto it2 = second.cbegin();

  while (it1 != first.cend() && it2 != second.cend())
  {
    BOOST_CHECK_EQUAL(*it1, *it2);
    ++it1;
    ++it2;
  }

  BOOST_CHECK(it1 == first.cend());
  BOOST_CHECK(it2 == second.cend());
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  aydogan::List< int > first;
  auto tail = first.beforeBegin();

  tail = first.insertAfter(tail, 7);
  tail = first.insertAfter(tail, 8);
  first.insertAfter(tail, 9);

  aydogan::List< int > second(std::move(first));

  BOOST_CHECK(first.empty());

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 7);
  ++it;
  BOOST_CHECK_EQUAL(*it, 8);
  ++it;
  BOOST_CHECK_EQUAL(*it, 9);
  ++it;
  BOOST_CHECK(it == second.end());
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  aydogan::List< int > first;
  first.push_front(2);
  first.push_front(1);

  aydogan::List< int > second;
  second.push_front(100);

  second = first;

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == second.end());
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  aydogan::List< int > first;
  first.push_front(2);
  first.push_front(1);

  aydogan::List< int > second;
  second.push_front(50);

  second = std::move(first);

  BOOST_CHECK(first.empty());

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == second.end());
}

BOOST_AUTO_TEST_CASE(front_on_empty_throws_test)
{
  aydogan::List< int > list;
  BOOST_CHECK_THROW(list.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_front_on_empty_throws_test)
{
  aydogan::List< int > list;
  BOOST_CHECK_THROW(list.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(erase_after_invalid_throws_test)
{
  aydogan::List< int > list;

  BOOST_CHECK_THROW(list.eraseAfter(list.beforeBegin()), std::out_of_range);

  list.push_front(1);
  BOOST_CHECK_THROW(list.eraseAfter(list.begin()), std::out_of_range);
}
