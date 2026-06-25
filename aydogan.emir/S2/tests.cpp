#define BOOST_TEST_MODULE S2_Tests
#include <boost/test/included/unit_test.hpp>

#include "stack.hpp"
#include "queue.hpp"
#include "expression.hpp"

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

BOOST_AUTO_TEST_CASE(simple_expression_test)
{
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("1 + 3"), 4);
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("4 * 7 - 3"), 25);
}

BOOST_AUTO_TEST_CASE(parentheses_test)
{
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("( 1 + 2 ) * ( 3 - 4 )"), -3);
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("( 10 / ( 2 + 3 ) % 4 )"), 2);
}

BOOST_AUTO_TEST_CASE(power_test)
{
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("2 ** 3"), 8);
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("( 2 + 1 ) ** 3"), 27);
}

BOOST_AUTO_TEST_CASE(power_associativity_test)
{
  BOOST_CHECK_EQUAL(aydogan::calculateExpression("2 ** 3 ** 2"), 512);
}

BOOST_AUTO_TEST_CASE(invalid_expression_test)
{
  BOOST_CHECK_THROW(aydogan::calculateExpression("1 +"), std::runtime_error);
  BOOST_CHECK_THROW(aydogan::calculateExpression("( 1 + 2"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(zero_division_test)
{
  BOOST_CHECK_THROW(aydogan::calculateExpression("10 / 0"), std::runtime_error);
  BOOST_CHECK_THROW(aydogan::calculateExpression("10 % 0"), std::runtime_error);
}
