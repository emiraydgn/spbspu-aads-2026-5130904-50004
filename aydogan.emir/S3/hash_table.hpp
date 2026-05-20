#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <list>
#include <utility>
#include <vector>

namespace aydogan
{
  template< class Key >
  struct SipHash
  {
    std::size_t operator()(const Key& key) const
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, key);
      return static_cast< std::size_t >(hasher.result());
    }
  };

  template< class Key >
  struct DefaultEqual
  {
    bool operator()(const Key& lhs, const Key& rhs) const
    {
      return lhs == rhs;
    }
  };

  template<
    class Key,
    class Value,
    class Hash = SipHash< Key >,
    class Equal = DefaultEqual< Key >
  >
  class HashTable
  {
  public:
    struct Entry
    {
      Key key;
      Value value;

      Entry(const Key& newKey, const Value& newValue):
        key(newKey),
        value(newValue)
      {}
    };

    void insert(const Key& key, const Value& value)
{
  std::size_t index = getIndex(key);

  for (Entry& entry: buckets_[index])
  {
    if (equal_(entry.key, key))
    {
      entry.value = value;
      return;
    }
  }

  buckets_[index].push_back(Entry(key, value));
  ++size_;
}

Value* find(const Key& key)
{
  std::size_t index = getIndex(key);

  for (Entry& entry: buckets_[index])
  {
    if (equal_(entry.key, key))
    {
      return &entry.value;
    }
  }

  return nullptr;
}

const Value* find(const Key& key) const
{
  std::size_t index = getIndex(key);

  for (const Entry& entry: buckets_[index])
  {
    if (equal_(entry.key, key))
    {
      return &entry.value;
    }
  }

  return nullptr;
}

bool erase(const Key& key)
{
  std::size_t index = getIndex(key);

  for (auto it = buckets_[index].begin(); it != buckets_[index].end(); ++it)
  {
    if (equal_(it->key, key))
    {
      buckets_[index].erase(it);
      --size_;
      return true;
    }
  }

  return false;
}

void rehash(std::size_t bucketCount)
{
  HashTable temp(bucketCount == 0 ? 1 : bucketCount);

  for (ConstIterator it = cbegin(); it != cend(); ++it)
  {
    temp.insert(it->key, it->value);
  }

  swap(temp);
}

void swap(HashTable& other) noexcept
{
  buckets_.swap(other.buckets_);
  std::swap(size_, other.size_);
  std::swap(hash_, other.hash_);
  std::swap(equal_, other.equal_);
}

Value drop(const Key& key)
{
  std::size_t index = getIndex(key);

  for (auto it = buckets_[index].begin(); it != buckets_[index].end(); ++it)
  {
    if (equal_(it->key, key))
    {
      Value value = it->value;
      buckets_[index].erase(it);
      --size_;
      return value;
    }
  }

  throw std::out_of_range("Key not found");
}

void rehash(std::size_t bucketCount)
{
  HashTable temp(bucketCount == 0 ? 1 : bucketCount);

  for (ConstIterator it = cbegin(); it != cend(); ++it)
  {
    temp.insert(it->key, it->value);
  }

  swap(temp);
}

std::size_t size() const noexcept
{
  return size_;
}

std::size_t getSize() const noexcept
{
  return size_;
}

std::size_t bucketCount() const noexcept
{
  return buckets_.size();
}

bool empty() const noexcept
{
  return size_ == 0;
}

Iterator begin()
{
  return Iterator(this, 0);
}

Iterator end()
{
  return Iterator(this, buckets_.size());
}

ConstIterator begin() const
{
  return cbegin();
}

ConstIterator end() const
{
  return cend();
}

ConstIterator cbegin() const
{
  return ConstIterator(this, 0);
}

ConstIterator cend() const
{
  return ConstIterator(this, buckets_.size());
}

bool contains(const Key& key) const
{
  return find(key) != nullptr;
}

    explicit HashTable(std::size_t bucketCount = 16):
      buckets_(bucketCount == 0 ? 1 : bucketCount),
      size_(0),
      hash_(),
      equal_()
    {}

  private:
    std::size_t getIndex(const Key& key) const
    {
      return hash_(key) % buckets_.size();
    }

    std::vector< std::list< Entry > > buckets_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;
  };
}

#endif
