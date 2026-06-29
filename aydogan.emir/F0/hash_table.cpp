#include "hash_table.hpp"
#include <stdexcept>

std::size_t aydogan::HashTable::normalizeCapacity(std::size_t capacity)
{
  const std::size_t minCapacity = 8;

  if (capacity < minCapacity) {
    capacity = minCapacity;
  }

  std::size_t result = 1;
  while (result < capacity) {
    result *= 2;
  }

  return result;
}

aydogan::HashTable::HashTable(std::size_t size):
  size_(normalizeCapacity(size)),
  count_(0)
{
  table_.assign(size_, Entry());
}

std::size_t aydogan::HashTable::hash1(const std::string & key) const
{
  const std::size_t base = 31;
  std::size_t hash = 0;

  for (char c : key) {
    hash = hash * base + static_cast< std::size_t >(c);
  }

  return hash % size_;
}

std::size_t aydogan::HashTable::hash2(const std::string & key) const
{
  const std::size_t base = 37;
  std::size_t hash = 0;

  for (char c : key) {
    hash = hash * base + static_cast< std::size_t >(c);
  }

  return 2 * (hash % (size_ / 2)) + 1;
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

      std::string newKey(key);
      List< std::string > newTranslations;
      newTranslations.insertAfter(newTranslations.beforeBegin(), value);

      table_[insertIdx].key.swap(newKey);
      table_[insertIdx].translations.swap(newTranslations);
      table_[insertIdx].occupied = true;
      table_[insertIdx].deleted = false;
      ++count_;
      return;
    }
  }

  if (foundDeleted) {
    std::string newKey(key);
    List< std::string > newTranslations;
    newTranslations.insertAfter(newTranslations.beforeBegin(), value);

    table_[insertIdx].key.swap(newKey);
    table_[insertIdx].translations.swap(newTranslations);
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
      Iterator< std::string > tail = table_[idx].translations.beforeBegin();

      for (Iterator< std::string > it = table_[idx].translations.begin();
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
      List< std::string > & translations = table_[idx].translations;
      Iterator< std::string > previous = translations.beforeBegin();
      Iterator< std::string > current = translations.begin();

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

aydogan::List< std::string > aydogan::HashTable::keys() const
{
  List< std::string > result;
  Iterator< std::string > tail = result.beforeBegin();

  for (const Entry & entry : table_) {
    if (entry.occupied && !entry.deleted) {
      tail = result.insertAfter(tail, entry.key);
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
  std::size_t newSize = size_ * 2;
  std::vector< Entry > oldTable = table_;
  table_.assign(newSize, Entry());
  size_ = newSize;
  count_ = 0;

  for (const Entry & entry : oldTable) {
    if (entry.occupied && !entry.deleted) {
      bool first = true;

      for (ConstIterator< std::string > it = entry.translations.cbegin();
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
