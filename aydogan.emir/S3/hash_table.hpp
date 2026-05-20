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
