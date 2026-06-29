#include "hash_table.hpp"
#include <stdexcept>

bool aydogan::HashTable::isPrime(std::size_t n)
{
  if (n < 2) {
    return false;
  }
  if (n == 2) {
    return true;
  }
  if (n % 2 == 0) {
    return false;
  }
  for (std::size_t i = 3; i * i <= n; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

std::size_t aydogan::HashTable::nextPrime(std::size_t n)
{
  while (!isPrime(n)) {
    ++n;
  }
  return n;
}

aydogan::HashTable::HashTable(std::size_t size):
  size_(nextPrime(size)),
  count_(0)
{
  table_.assign(size_, Entry());
}

std::size_t aydogan::HashTable::hash1(const std::string & key) const
{
  std::size_t hash = 0;
  for (char c : key) {
    hash = hash * 31 + static_cast< std::size_t >(c);
  }
  return hash % size_;
}

std::size_t aydogan::HashTable::hash2(const std::string & key) const
{
  std::size_t hash = 0;
  for (char c : key) {
    hash = hash * 37 + static_cast< std::size_t >(c);
  }
  return 1 + (hash % (size_ - 1));
}

std::size_t aydogan::HashTable::probe(const std::string & key, std::size_t i) const
{
  return (hash1(key) + i * hash2(key)) % size_;
}

void aydogan::HashTable::insert(const std::string & key, const std::string & value)
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
      table_[insertIdx].translations.clear();
      table_[insertIdx].translations.insertAfter(
        table_[insertIdx].translations.beforeBegin(),
        value
      );
      table_[insertIdx].occupied = true;
      table_[insertIdx].deleted = false;
      ++count_;
      return;
    }
  }

  if (foundDeleted) {
    table_[insertIdx].key = key;
    table_[insertIdx].translations.clear();
    table_[insertIdx].translations.insertAfter(
      table_[insertIdx].translations.beforeBegin(),
      value
    );
    table_[insertIdx].occupied = true;
    table_[insertIdx].deleted = false;
    ++count_;
    return;
  }

  throw std::logic_error("hash table is full");
}

void aydogan::HashTable::remove(const std::string & key)
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

void aydogan::HashTable::addTranslation(const std::string & key, const std::string & value)
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      aydogan::Iterator< std::string > tail = table_[idx].translations.beforeBegin();
      for (aydogan::Iterator< std::string > it = table_[idx].translations.begin();
          it != table_[idx].translations.end();
          ++it) {
        ++tail;
      }
      table_[idx].translations.insertAfter(tail, value);
      return;
    }
  }
  throw std::logic_error("word not found");
}

void aydogan::HashTable::removeTranslation(const std::string & key, const std::string & value)
{
  for (std::size_t i = 0; i < size_; ++i) {
    std::size_t idx = probe(key, i);
    if (!table_[idx].occupied && !table_[idx].deleted) {
      break;
    }
    if (table_[idx].occupied && !table_[idx].deleted && table_[idx].key == key) {
      aydogan::List< std::string > & translations = table_[idx].translations;
      aydogan::Iterator< std::string > previous = translations.beforeBegin();
      aydogan::Iterator< std::string > current = translations.begin();

      while (current != translations.end() && *current != value) {
        ++previous;
        ++current;
      }

      if (current == translations.end()) {
        throw std::logic_error("translation not found");
      }

      translations.eraseAfter(previous);
      if (translations.empty()) {
        table_[idx].deleted = true;
        --count_;
      }
      return;
    }
  }
  throw std::logic_error("word not found");
}

aydogan::List< std::string > aydogan::HashTable::get(const std::string & key) const
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

bool aydogan::HashTable::contains(const std::string & key) const
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

std::size_t aydogan::HashTable::count() const
{
  return count_;
}

std::vector< std::string > aydogan::HashTable::keys() const
{
  std::vector< std::string > result;
  for (const Entry & entry : table_) {
    if (entry.occupied && !entry.deleted) {
      result.push_back(entry.key);
    }
  }
  return result;
}

void aydogan::HashTable::clear()
{
  table_.assign(size_, Entry());
  count_ = 0;
}

void aydogan::HashTable::rehash()
{
  std::size_t newSize = nextPrime(size_ * 2);
  std::vector< Entry > oldTable = table_;
  table_.assign(newSize, Entry());
  size_ = newSize;
  count_ = 0;

  for (const Entry & entry : oldTable) {
    if (entry.occupied && !entry.deleted) {
      bool first = true;
      for (aydogan::ConstIterator< std::string > it = entry.translations.cbegin();
          it != entry.translations.cend();
          ++it) {
        if (first) {
          insert(entry.key, *it);
          first = false;
        } else {
          addTranslation(entry.key, *it);
        }
      }
    }
  }
}
