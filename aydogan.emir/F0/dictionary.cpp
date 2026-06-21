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
