#define BOOST_TEST_MODULE F0_HashTable_Tests
#include <boost/test/included/unit_test.hpp>

#include "hash_table.hpp"
#include <stdexcept>
#include <string>

static bool containsOnlyOne(const aydogan::List< std::string > & list, const std::string & value)
{
  aydogan::ConstIterator< std::string > it = list.cbegin();

  if (it == list.cend()) {
    return false;
  }
  if (*it != value) {
    return false;
  }

  ++it;
  return it == list.cend();
}

static bool containsTwo(
    const aydogan::List< std::string > & list,
    const std::string & first,
    const std::string & second
)
{
  aydogan::ConstIterator< std::string > it = list.cbegin();

  if (it == list.cend() || *it != first) {
    return false;
  }

  ++it;
  if (it == list.cend() || *it != second) {
    return false;
  }

  ++it;
  return it == list.cend();
}

BOOST_AUTO_TEST_CASE(empty_hash_table_test)
{
  aydogan::HashTable table;

  BOOST_CHECK_EQUAL(table.count(), 0);
  BOOST_CHECK(!table.contains("apple"));
  BOOST_CHECK_THROW(table.get("apple"), std::logic_error);
  BOOST_CHECK_THROW(table.remove("apple"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(insert_and_get_test)
{
  aydogan::HashTable table;

  table.insert("apple", "яблоко");

  BOOST_CHECK_EQUAL(table.count(), 1);
  BOOST_CHECK(table.contains("apple"));
  BOOST_CHECK(containsOnlyOne(table.get("apple"), "яблоко"));
}

BOOST_AUTO_TEST_CASE(duplicate_insert_test)
{
  aydogan::HashTable table;

  table.insert("apple", "яблоко");

  BOOST_CHECK_THROW(table.insert("apple", "плод"), std::logic_error);
  BOOST_CHECK_EQUAL(table.count(), 1);
  BOOST_CHECK(containsOnlyOne(table.get("apple"), "яблоко"));
}

BOOST_AUTO_TEST_CASE(add_and_remove_translation_test)
{
  aydogan::HashTable table;

  table.insert("apple", "яблоко");
  table.addTranslation("apple", "плод");

  BOOST_CHECK_EQUAL(table.count(), 1);
  BOOST_CHECK(containsTwo(table.get("apple"), "яблоко", "плод"));

  table.removeTranslation("apple", "плод");

  BOOST_CHECK_EQUAL(table.count(), 1);
  BOOST_CHECK(containsOnlyOne(table.get("apple"), "яблоко"));

  table.removeTranslation("apple", "яблоко");

  BOOST_CHECK_EQUAL(table.count(), 0);
  BOOST_CHECK(!table.contains("apple"));
  BOOST_CHECK_THROW(table.get("apple"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(remove_word_test)
{
  aydogan::HashTable table;

  table.insert("apple", "яблоко");
  table.remove("apple");

  BOOST_CHECK_EQUAL(table.count(), 0);
  BOOST_CHECK(!table.contains("apple"));
  BOOST_CHECK_THROW(table.get("apple"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(remove_missing_translation_test)
{
  aydogan::HashTable table;

  table.insert("apple", "яблоко");

  BOOST_CHECK_THROW(table.removeTranslation("apple", "плод"), std::logic_error);
  BOOST_CHECK_EQUAL(table.count(), 1);
  BOOST_CHECK(containsOnlyOne(table.get("apple"), "яблоко"));
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  aydogan::HashTable table(8);

  for (int i = 0; i < 50; ++i) {
    table.insert("word" + std::to_string(i), "translation" + std::to_string(i));
  }

  BOOST_CHECK_EQUAL(table.count(), 50);

  for (int i = 0; i < 50; ++i) {
    std::string key = "word" + std::to_string(i);
    std::string value = "translation" + std::to_string(i);

    BOOST_CHECK(table.contains(key));
    BOOST_CHECK(containsOnlyOne(table.get(key), value));
  }
}
