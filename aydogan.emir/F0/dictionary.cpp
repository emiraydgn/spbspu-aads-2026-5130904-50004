#include "dictionary.hpp"
#include <iomanip>
#include <sstream>
#include <vector>
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

namespace {

  std::vector< std::string > readDictNames(std::istream & in)
  {
    std::string line;
    std::getline(in, line);
    std::istringstream iss(line);
    std::vector< std::string > dicts;
    std::string name;
    while (iss >> name) {
      dicts.push_back(name);
    }
    return dicts;
  }

  void checkDictsExist(const std::vector< std::string > & dicts, const DictionaryStorage & storage)
  {
    for (const auto & d : dicts) {
      if (!storage.count(d)) {
        throw std::logic_error("dictionary not found: " + d);
      }
    }
  }

  void addWordToDict(Dictionary & dict, const std::string & key, const std::list< std::string > & translations)
  {
    bool first = true;
    for (const auto & t : translations) {
      if (first) {
        dict.insert(key, t);
        first = false;
      } else {
        dict.addTranslation(key, t);
      }
    }
  }

}

void mergeDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }
  std::vector< std::string > dicts = readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }
  checkDictsExist(dicts, storage);

  Dictionary merged;
  for (const auto & d : dicts) {
    for (const auto & key : storage.at(d).keys()) {
      std::list< std::string > translations = storage.at(d).get(key);
      if (!merged.contains(key)) {
        addWordToDict(merged, key, translations);
      } else {
        std::list< std::string > existing = merged.get(key);
        for (const auto & t : translations) {
          bool found = false;
          for (const auto & et : existing) {
            if (et == t) {
              found = true;
              break;
            }
          }
          if (!found) {
            merged.addTranslation(key, t);
            existing.push_back(t);
          }
        }
      }
    }
  }
  storage.emplace(result, merged);
}

void unionDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }
  std::vector< std::string > dicts = readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }
  checkDictsExist(dicts, storage);

  Dictionary united;
  for (const auto & d : dicts) {
    for (const auto & key : storage.at(d).keys()) {
      if (!united.contains(key)) {
        addWordToDict(united, key, storage.at(d).get(key));
      }
    }
  }
  storage.emplace(result, united);
}

void differenceDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }
  std::vector< std::string > dicts = readDictNames(in);
  if (dicts.size() < 2) {
    throw std::logic_error("at least two dictionaries required");
  }
  checkDictsExist(dicts, storage);

  Dictionary diff;
  const std::string & first = dicts[0];
  for (const auto & key : storage.at(first).keys()) {
    bool inOther = false;
    for (std::size_t i = 1; i < dicts.size(); ++i) {
      if (storage.at(dicts[i]).contains(key)) {
        inOther = true;
        break;
      }
    }
    if (!inOther) {
      addWordToDict(diff, key, storage.at(first).get(key));
    }
  }
  storage.emplace(result, diff);
}
