#define BOOST_TEST_MODULE S2_Tests
#include <boost/test/included/unit_test.hpp>

#include "stack.hpp"
#include "queue.hpp"
#include "logic.hpp"

BOOST_AUTO_TEST_CASE(stack_test)
{
  aydogan::Stack< int > st;
  BOOST_CHECK(st.empty());

  st.push(1);
  st.push(2);
  st.push(3);

  BOOST_CHECK_EQUAL(st.top(), 3);
  BOOST_CHECK_EQUAL(st.drop(), 3);
  BOOST_CHECK_EQUAL(st.drop(), 2);
  BOOST_CHECK_EQUAL(st.drop(), 1);
  BOOST_CHECK(st.empty());
}

BOOST_AUTO_TEST_CASE(queue_test)
{
  aydogan::Queue< int > q;
  BOOST_CHECK(q.empty());

  q.push(10);
  q.push(20);
  q.push(30);

  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.drop(), 10);
  BOOST_CHECK_EQUAL(q.drop(), 20);
  BOOST_CHECK_EQUAL(q.drop(), 30);
  BOOST_CHECK(q.empty());
}
