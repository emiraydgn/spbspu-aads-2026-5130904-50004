#include "dictionary.hpp"
#include <iomanip>
#include <stdexcept>

void createDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (storage.count(name)) {
    throw std::logic_error("dictionary already exists");
  }
  storage.emplace(name, Dictionary());
}

void showDict(std::istream & in, std::ostream & out, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (!storage.count(name)) {
    throw std::logic_error("dictionary not found");
  }
  out << "<DICT: " << name << ", WORDS: " << storage.at(name).count() << ">\n";
}

void dropDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (!storage.count(name)) {
    throw std::logic_error("dictionary not found");
  }
  storage.erase(name);
}

void addWord(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict, word, translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).insert(word, translation);
}

void removeWord(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict, word;
  in >> dict >> word;
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).remove(word);
}

void addTranslation(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict, word, translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).addTranslation(word, translation);
}

void removeTranslation(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict, word, translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).removeTranslation(word, translation);
}

void translate(std::istream & in, std::ostream & out, DictionaryStorage & storage)
{
  std::string dict, word;
  in >> dict >> word;
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  std::list< std::string > translations = storage.at(dict).get(word);
  out << "<";
  bool first = true;
  for (const auto & t : translations) {
    if (!first) {
      out << ", ";
    }
    out << t;
    first = false;
  }
  out << ">\n";
}
