#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <string>
#include <list>
#include <vector>

class HashTable
{
public:
  explicit HashTable(std::size_t size = 101);

  void insert(const std::string & key, const std::string & value);
  void remove(const std::string & key);
  void addTranslation(const std::string & key, const std::string & value);
  void removeTranslation(const std::string & key, const std::string & value);

  std::list< std::string > get(const std::string & key) const;
  bool contains(const std::string & key) const;
  std::size_t count() const;

  std::vector< std::string > keys() const;

  void clear();

private:
  struct Entry {
    std::string key;
    std::list< std::string > translations;
    bool occupied;
    bool deleted;

    Entry() : occupied(false), deleted(false) {}
  };

  std::vector< Entry > table_;
  std::size_t size_;
  std::size_t count_;

  std::size_t hash1(const std::string & key) const;
  std::size_t hash2(const std::string & key) const;
  std::size_t probe(const std::string & key, std::size_t i) const;

  void rehash();

  static bool isPrime(std::size_t n);
  static std::size_t nextPrime(std::size_t n);
};

#endif
