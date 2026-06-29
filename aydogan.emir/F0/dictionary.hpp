#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <cstddef>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <utility>

#include "hash_table.hpp"

namespace aydogan
{
  using Dictionary = HashTable;

  class DictionaryStorage
  {
  public:
    std::size_t count(const std::string & name) const
    {
      return contains(name) ? 1 : 0;
    }

    bool contains(const std::string & name) const
    {
      for (ConstIterator< Entry > it = data_.cbegin(); it != data_.cend(); ++it) {
        if ((*it).name == name) {
          return true;
        }
      }

      return false;
    }

    Dictionary & at(const std::string & name)
    {
      for (Iterator< Entry > it = data_.begin(); it != data_.end(); ++it) {
        if ((*it).name == name) {
          return (*it).dict;
        }
      }

      throw std::logic_error("dictionary not found");
    }

    const Dictionary & at(const std::string & name) const
    {
      for (ConstIterator< Entry > it = data_.cbegin(); it != data_.cend(); ++it) {
        if ((*it).name == name) {
          return (*it).dict;
        }
      }

      throw std::logic_error("dictionary not found");
    }

    void emplace(const std::string & name, const Dictionary & dict)
    {
      if (contains(name)) {
        throw std::logic_error("dictionary already exists");
      }

      data_.insertAfter(data_.beforeBegin(), Entry(name, dict));
    }

    void emplace(const std::string & name, Dictionary && dict)
    {
      if (contains(name)) {
        throw std::logic_error("dictionary already exists");
      }

      data_.insertAfter(data_.beforeBegin(), Entry(name, std::move(dict)));
    }

    void erase(const std::string & name)
    {
      Iterator< Entry > prev = data_.beforeBegin();
      Iterator< Entry > cur = data_.begin();

      while (cur != data_.end()) {
        if ((*cur).name == name) {
          data_.eraseAfter(prev);
          return;
        }

        ++prev;
        ++cur;
      }

      throw std::logic_error("dictionary not found");
    }

  private:
    struct Entry
    {
      Entry():
        name(),
        dict()
      {}

      Entry(const std::string & entryName, const Dictionary & entryDict):
        name(entryName),
        dict(entryDict)
      {}

      Entry(const std::string & entryName, Dictionary && entryDict):
        name(entryName),
        dict(std::move(entryDict))
      {}

      std::string name;
      Dictionary dict;
    };

    List< Entry > data_;
  };

  void createDict(std::istream &, std::ostream &, DictionaryStorage &);
  void showDict(std::istream &, std::ostream &, DictionaryStorage &);
  void dropDict(std::istream &, std::ostream &, DictionaryStorage &);

  void addWord(std::istream &, std::ostream &, DictionaryStorage &);
  void removeWord(std::istream &, std::ostream &, DictionaryStorage &);

  void addTranslation(std::istream &, std::ostream &, DictionaryStorage &);
  void removeTranslation(std::istream &, std::ostream &, DictionaryStorage &);

  void translate(std::istream &, std::ostream &, DictionaryStorage &);

  void mergeDict(std::istream &, std::ostream &, DictionaryStorage &);
  void unionDict(std::istream &, std::ostream &, DictionaryStorage &);
  void differenceDict(std::istream &, std::ostream &, DictionaryStorage &);

  void countDict(std::istream &, std::ostream &, DictionaryStorage &);
  void help(std::istream &, std::ostream &, DictionaryStorage &);

  void saveDict(std::istream &, std::ostream &, DictionaryStorage &);
  void loadDict(std::istream &, std::ostream &, DictionaryStorage &);
}

#endif
