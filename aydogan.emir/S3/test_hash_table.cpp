#define BOOST_TEST_MODULE S3Tests
#include <boost/test/included/unit_test.hpp>

#include "hash_table.hpp"

#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_CASE(insert_and_find_test)
{
  aydogan::HashTable< std::string, int > table;

  table.insert("one", 1);
  table.insert("two", 2);

  BOOST_CHECK(table.contains("one"));
  BOOST_CHECK(table.contains("two"));
  BOOST_CHECK(!table.contains("three"));
  BOOST_REQUIRE(table.find("one") != nullptr);
  BOOST_CHECK_EQUAL(*table.find("one"), 1);
}

BOOST_AUTO_TEST_CASE(update_test)
{
  aydogan::HashTable< std::string, int > table;

  table.insert("key", 10);
  table.insert("key", 20);

  BOOST_REQUIRE(table.find("key") != nullptr);
  BOOST_CHECK_EQUAL(*table.find("key"), 20);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  aydogan::HashTable< std::string, int > table;

  table.insert("a", 1);

  BOOST_CHECK(table.erase("a"));
  BOOST_CHECK(!table.contains("a"));
  BOOST_CHECK(!table.erase("missing"));
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  aydogan::HashTable< std::string, int > table;

  table.insert("x", 42);

  BOOST_CHECK_EQUAL(table.drop("x"), 42);
  BOOST_CHECK(!table.contains("x"));
  BOOST_CHECK_THROW(table.drop("x"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  aydogan::HashTable< std::string, int > table(2);

  table.insert("a", 1);
  table.insert("b", 2);
  table.insert("c", 3);

  table.rehash(16);

  BOOST_CHECK(table.contains("a"));
  BOOST_CHECK(table.contains("b"));
  BOOST_CHECK(table.contains("c"));
  BOOST_CHECK_EQUAL(table.bucketCount(), 16);
}

BOOST_AUTO_TEST_CASE(copy_independence_test)
{
  aydogan::HashTable< std::string, int > first;

  first.insert("a", 1);
  first.insert("b", 2);

  aydogan::HashTable< std::string, int > second(first);

  second.erase("a");

  BOOST_CHECK(first.contains("a"));
  BOOST_CHECK(!second.contains("a"));
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  aydogan::HashTable< std::string, int > table;

  table.insert("a", 1);
  table.insert("b", 2);
  table.insert("c", 3);

  std::size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
}
