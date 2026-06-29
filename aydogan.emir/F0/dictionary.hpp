#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <iosfwd>
#include <string>
#include <unordered_map>

#include "hash_table.hpp"

namespace aydogan
{
  using Dictionary = HashTable;
  using DictionaryStorage = std::unordered_map< std::string, Dictionary >;

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
