#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <iosfwd>
#include <string>
#include <unordered_map>

#include "hash_table.hpp"

using Dictionary = HashTable;
using DictionaryStorage = std::unordered_map< std::string, Dictionary >;

void createDict(std::istream &, std::ostream &, DictionaryStorage &);
void showDict(std::istream &, std::ostream &, DictionaryStorage &);
void dropDict(std::istream &, std::ostream &, DictionaryStorage &);

#endif
