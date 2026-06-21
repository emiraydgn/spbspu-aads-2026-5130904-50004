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

void HashTable::insert(const std::string & key, const std::string & value)
{
  if (count_ >= size_ / 2) {
    rehash();
  }

  std::size_t insertIdx = size_;
  bool foundDeleted = false;

  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);

    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      throw std::logic_error("word already exists");
    }

    if (table_[idx].occupied && table_[idx].deleted && !foundDeleted) {
      insertIdx = idx;
      foundDeleted = true;
    }

    if (!table_[idx].occupied) {
      if (!foundDeleted) {
        insertIdx = idx;
      }
      table_[insertIdx].key = key;
      table_[insertIdx].translations = { value };
      table_[insertIdx].occupied = true;
      table_[insertIdx].deleted = false;
      ++count_;
      return;
    }
  }

  if (foundDeleted) {
    table_[insertIdx].key = key;
    table_[insertIdx].translations = { value };
    table_[insertIdx].occupied = true;
    table_[insertIdx].deleted = false;
    ++count_;
    return;
  }

  throw std::logic_error("hash table is full");
}

void HashTable::remove(const std::string & key)
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      table_[idx].deleted = true;
      --count_;
      return;
    }
  }
  throw std::logic_error("word not found");
}

void HashTable::addTranslation(const std::string & key, const std::string & value)
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      table_[idx].translations.push_back(value);
      return;
    }
  }
  throw std::logic_error("word not found");
}

void HashTable::removeTranslation(const std::string & key, const std::string & value)
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      auto & lst = table_[idx].translations;
      auto it = std::find(lst.begin(), lst.end(), value);
      if (it == lst.end()) {
        throw std::logic_error("translation not found");
      }
      lst.erase(it);
      if (lst.empty()) {
        table_[idx].deleted = true;
        --count_;
      }
      return;
    }
  }
  throw std::logic_error("word not found");
}

std::list< std::string > HashTable::get(const std::string & key) const
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      return table_[idx].translations;
    }
  }
  throw std::logic_error("word not found");
}

bool HashTable::contains(const std::string & key) const
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      return false;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      return true;
    }
  }
  return false;
}

std::size_t HashTable::count() const
{
  return count_;
}

std::vector< std::string > HashTable::keys() const
{
  std::vector< std::string > result;
  for (const auto & entry : table_) {
    if (entry.occupied && !entry.deleted) {
      result.push_back(entry.key);
    }
  }
  return result;
}

void HashTable::clear()
{
  table_.assign(size_, Entry());
  count_ = 0;
}

void HashTable::rehash()
{
  std::size_t newSize = nextPrime(size_ * 2);
  std::vector< Entry > oldTable = table_;
  table_.assign(newSize, Entry());
  size_ = newSize;
  count_ = 0;
  for (const auto & entry : oldTable) {
    if (entry.occupied && !entry.deleted) {
      bool first = true;
      for (const auto & translation : entry.translations) {
        if (first) {
          insert(entry.key, translation);
          first = false;
        } else {
          addTranslation(entry.key, translation);
        }
      }
    }
  }
}
