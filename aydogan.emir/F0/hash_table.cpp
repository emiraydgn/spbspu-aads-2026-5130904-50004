#include "hash_table.hpp"
#include <algorithm>
#include <iterator>
#include <stdexcept>

bool HashTable::isPrime(std::size_t n)
{
  if (n < 2) return false;
  if (n == 2) return true;
  if (n % 2 == 0) return false;
  for (std::size_t i = 3; i * i <= n; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
}

std::size_t HashTable::nextPrime(std::size_t n)
{
  while (!isPrime(n)) {
    ++n;
  }
  return n;
}

HashTable::HashTable(std::size_t size) :
  size_(nextPrime(size)),
  count_(0)
{
  table_.assign(size_, Entry());
}

std::size_t HashTable::hash1(const std::string & key) const
{
  std::size_t hash = 0;
  for (char c : key) {
    hash = hash * 31 + static_cast< std::size_t >(c);
  }
  return hash % size_;
}

std::size_t HashTable::hash2(const std::string & key) const
{
  std::size_t hash = 0;
  for (char c : key) {
    hash = hash * 37 + static_cast< std::size_t >(c);
  }
  return 1 + (hash % (size_ - 1));
}

std::size_t HashTable::probe(const std::string & key, std::size_t i) const
{
  return (hash1(key) + i * hash2(key)) % size_;
}
