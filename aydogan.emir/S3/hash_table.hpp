#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <list>
#include <stdexcept>
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

    class Iterator
    {
    public:
      Iterator():
        table_(nullptr),
        bucket_(0)
      {}

      Entry& operator*() const
      {
        return *entry_;
      }

      Entry* operator->() const
      {
        return &(*entry_);
      }

      Iterator& operator++()
      {
        if (table_ == nullptr || bucket_ >= table_->buckets_.size())
        {
          return *this;
        }

        ++entry_;
        moveToValid();
        return *this;
      }

      bool operator==(const Iterator& other) const
      {
        if (table_ != other.table_ || bucket_ != other.bucket_)
        {
          return false;
        }

        if (table_ == nullptr || bucket_ >= table_->buckets_.size())
        {
          return true;
        }

        return entry_ == other.entry_;
      }

      bool operator!=(const Iterator& other) const
      {
        return !(*this == other);
      }

    private:
      friend class HashTable;

      Iterator(HashTable* table, std::size_t bucket):
        table_(table),
        bucket_(bucket)
      {
        if (table_ != nullptr && bucket_ < table_->buckets_.size())
        {
          entry_ = table_->buckets_[bucket_].begin();
          moveToValid();
        }
      }

      void moveToValid()
      {
        while (bucket_ < table_->buckets_.size())
        {
          if (entry_ != table_->buckets_[bucket_].end())
          {
            return;
          }

          ++bucket_;

          if (bucket_ < table_->buckets_.size())
          {
            entry_ = table_->buckets_[bucket_].begin();
          }
        }
      }

      HashTable* table_;
      std::size_t bucket_;
      typename std::list< Entry >::iterator entry_;
    };

    class ConstIterator
    {
    public:
      ConstIterator():
        table_(nullptr),
        bucket_(0)
      {}

      const Entry& operator*() const
      {
        return *entry_;
      }

      const Entry* operator->() const
      {
        return &(*entry_);
      }

      ConstIterator& operator++()
      {
        if (table_ == nullptr || bucket_ >= table_->buckets_.size())
        {
          return *this;
        }

        ++entry_;
        moveToValid();
        return *this;
      }

      bool operator==(const ConstIterator& other) const
      {
        if (table_ != other.table_ || bucket_ != other.bucket_)
        {
          return false;
        }

        if (table_ == nullptr || bucket_ >= table_->buckets_.size())
        {
          return true;
        }

        return entry_ == other.entry_;
      }

      bool operator!=(const ConstIterator& other) const
      {
        return !(*this == other);
      }

    private:
      friend class HashTable;

      ConstIterator(const HashTable* table, std::size_t bucket):
        table_(table),
        bucket_(bucket)
      {
        if (table_ != nullptr && bucket_ < table_->buckets_.size())
        {
          entry_ = table_->buckets_[bucket_].cbegin();
          moveToValid();
        }
      }

      void moveToValid()
      {
        while (bucket_ < table_->buckets_.size())
        {
          if (entry_ != table_->buckets_[bucket_].cend())
          {
            return;
          }

          ++bucket_;

          if (bucket_ < table_->buckets_.size())
          {
            entry_ = table_->buckets_[bucket_].cbegin();
          }
        }
      }

      const HashTable* table_;
      std::size_t bucket_;
      typename std::list< Entry >::const_iterator entry_;
    };

    explicit HashTable(std::size_t bucketCount = 16):
      buckets_(bucketCount == 0 ? 1 : bucketCount),
      size_(0),
      hash_(),
      equal_()
    {}

    HashTable(const HashTable& other) = default;
    HashTable(HashTable&& other) noexcept = default;
    ~HashTable() = default;

    HashTable& operator=(const HashTable& other) = default;
    HashTable& operator=(HashTable&& other) noexcept = default;

    void swap(HashTable& other) noexcept
    {
      buckets_.swap(other.buckets_);
      std::swap(size_, other.size_);
      std::swap(hash_, other.hash_);
      std::swap(equal_, other.equal_);
    }

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

    void add(const Key& key, const Value& value)
    {
      insert(key, value);
    }

    bool contains(const Key& key) const
    {
      return find(key) != nullptr;
    }

    bool has(const Key& key) const
    {
      return contains(key);
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
