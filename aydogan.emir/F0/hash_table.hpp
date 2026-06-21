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
};

#endif
