#include "dictionary.hpp"
#include <cctype>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

namespace aydogan
{
  namespace detail
  {
    void skipSpaces(const std::string & line, std::size_t & pos)
    {
      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos]))) {
        ++pos;
      }
    }

    std::string readToken(const std::string & line, std::size_t & pos)
    {
      skipSpaces(line, pos);

      std::size_t begin = pos;
      while (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos]))) {
        ++pos;
      }

      return line.substr(begin, pos - begin);
    }

    bool readQuoted(const std::string & line, std::size_t & pos, std::string & value)
    {
      skipSpaces(line, pos);

      if (pos >= line.size()) {
        return false;
      }
      if (line[pos] != '"') {
        throw std::logic_error("bad file format");
      }

      ++pos;
      std::size_t begin = pos;

      while (pos < line.size() && line[pos] != '"') {
        ++pos;
      }

      if (pos >= line.size()) {
        throw std::logic_error("bad file format");
      }

      value = line.substr(begin, pos - begin);
      ++pos;
      return true;
    }

    std::vector< std::string > readDictNames(std::istream & in)
    {
      std::string line;
      std::getline(in, line);

      std::vector< std::string > dicts;
      std::size_t pos = 0;

      while (pos < line.size()) {
        std::string name = readToken(line, pos);
        if (!name.empty()) {
          dicts.push_back(name);
        }
      }

      return dicts;
    }

    void checkDictsExist(const std::vector< std::string > & dicts, const DictionaryStorage & storage)
    {
      for (const std::string & d : dicts) {
        if (!storage.count(d)) {
          throw std::logic_error("dictionary not found: " + d);
        }
      }
    }

    void addWordToDict(Dictionary & dict, const std::string & key, const List< std::string > & translations)
    {
      bool first = true;
      for (ConstIterator< std::string > it = translations.cbegin(); it != translations.cend(); ++it) {
        if (first) {
          dict.insert(key, *it);
          first = false;
        } else {
          dict.addTranslation(key, *it);
        }
      }
    }

    bool containsTranslation(const List< std::string > & translations, const std::string & value)
    {
      for (ConstIterator< std::string > it = translations.cbegin(); it != translations.cend(); ++it) {
        if (*it == value) {
          return true;
        }
      }
      return false;
    }
  }
}

void aydogan::createDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (storage.count(name)) {
    throw std::logic_error("dictionary already exists");
  }
  storage.emplace(name, Dictionary());
}

void aydogan::showDict(std::istream & in, std::ostream & out, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (!storage.count(name)) {
    throw std::logic_error("dictionary not found");
  }
  out << "<DICT: " << name << ", WORDS: " << storage.at(name).count() << ">\n";
}

void aydogan::dropDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (!storage.count(name)) {
    throw std::logic_error("dictionary not found");
  }
  storage.erase(name);
}

void aydogan::addWord(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string word;
  std::string translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).insert(word, translation);
}

void aydogan::removeWord(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string word;
  in >> dict >> word;
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).remove(word);
}

void aydogan::addTranslation(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string word;
  std::string translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).addTranslation(word, translation);
}

void aydogan::removeTranslation(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string word;
  std::string translation;
  in >> dict >> word >> std::quoted(translation);
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }
  storage.at(dict).removeTranslation(word, translation);
}

void aydogan::translate(std::istream & in, std::ostream & out, DictionaryStorage & storage)
{
  std::string dict;
  std::string word;
  in >> dict >> word;
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }

  List< std::string > translations = storage.at(dict).get(word);
  out << "<";
  bool first = true;
  for (ConstIterator< std::string > it = translations.cbegin(); it != translations.cend(); ++it) {
    if (!first) {
      out << ", ";
    }
    out << *it;
    first = false;
  }
  out << ">\n";
}

void aydogan::mergeDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }

  std::vector< std::string > dicts = detail::readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }
  detail::checkDictsExist(dicts, storage);

  Dictionary merged;
  for (const std::string & d : dicts) {
    for (const std::string & key : storage.at(d).keys()) {
      List< std::string > translations = storage.at(d).get(key);
      if (!merged.contains(key)) {
        detail::addWordToDict(merged, key, translations);
      } else {
        List< std::string > existing = merged.get(key);
        for (ConstIterator< std::string > it = translations.cbegin(); it != translations.cend(); ++it) {
          if (!detail::containsTranslation(existing, *it)) {
            merged.addTranslation(key, *it);
            existing = merged.get(key);
          }
        }
      }
    }
  }
  storage.emplace(result, merged);
}

void aydogan::unionDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }

  std::vector< std::string > dicts = detail::readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }
  detail::checkDictsExist(dicts, storage);

  Dictionary united;
  for (const std::string & d : dicts) {
    for (const std::string & key : storage.at(d).keys()) {
      if (!united.contains(key)) {
        detail::addWordToDict(united, key, storage.at(d).get(key));
      }
    }
  }
  storage.emplace(result, united);
}

void aydogan::differenceDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string result;
  in >> result;
  if (storage.count(result)) {
    throw std::logic_error("dictionary already exists");
  }

  std::vector< std::string > dicts = detail::readDictNames(in);
  if (dicts.size() < 2) {
    throw std::logic_error("at least two dictionaries required");
  }
  detail::checkDictsExist(dicts, storage);

  Dictionary diff;
  const std::string & first = dicts[0];
  for (const std::string & key : storage.at(first).keys()) {
    bool inOther = false;
    for (std::size_t i = 1; i < dicts.size(); ++i) {
      if (storage.at(dicts[i]).contains(key)) {
        inOther = true;
        break;
      }
    }
    if (!inOther) {
      detail::addWordToDict(diff, key, storage.at(first).get(key));
    }
  }
  storage.emplace(result, diff);
}

void aydogan::countDict(std::istream & in, std::ostream & out, DictionaryStorage & storage)
{
  std::string name;
  in >> name;
  if (!storage.count(name)) {
    throw std::logic_error("dictionary not found");
  }
  out << storage.at(name).count() << "\n";
}

void aydogan::help(std::istream &, std::ostream & out, DictionaryStorage &)
{
  out << "Commands:\n";
  out << "  create-dict <dict>\n";
  out << "  show <dict>\n";
  out << "  drop-dict <dict>\n";
  out << "  add-word <dict> <word> <translation>\n";
  out << "  remove-word <dict> <word>\n";
  out << "  add-translation <dict> <word> <translation>\n";
  out << "  remove-translation <dict> <word> <translation>\n";
  out << "  translate <dict> <word>\n";
  out << "  merge-dict <result> <dict1> <dict2> ... <dictk>\n";
  out << "  union <result> <dict1> <dict2> ... <dictk>\n";
  out << "  difference <result> <dict1> <dict2> ... <dictk>\n";
  out << "  count <dict>\n";
  out << "  save <dict> <filename>\n";
  out << "  load <dict> <filename>\n";
  out << "  help\n";
  out << "  exit\n";
}

void aydogan::saveDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string filename;
  in >> dict >> filename;
  if (!storage.count(dict)) {
    throw std::logic_error("dictionary not found");
  }

  std::ofstream file(filename);
  if (!file) {
    throw std::logic_error("cannot open file");
  }

  for (const std::string & key : storage.at(dict).keys()) {
    file << key;
    List< std::string > translations = storage.at(dict).get(key);
    for (ConstIterator< std::string > it = translations.cbegin(); it != translations.cend(); ++it) {
      file << ' ' << std::quoted(*it);
    }
    file << '\n';
  }
}

void aydogan::loadDict(std::istream & in, std::ostream &, DictionaryStorage & storage)
{
  std::string dict;
  std::string filename;
  in >> dict >> filename;
  if (storage.count(dict)) {
    throw std::logic_error("dictionary already exists");
  }

  std::ifstream file(filename);
  if (!file) {
    throw std::logic_error("cannot open file");
  }

  Dictionary newDict;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::size_t pos = 0;
    std::string word = detail::readToken(line, pos);
    if (word.empty()) {
      continue;
    }

    std::string translation;
    bool first = true;
    while (detail::readQuoted(line, pos, translation)) {
      if (first) {
        newDict.insert(word, translation);
        first = false;
      } else {
        newDict.addTranslation(word, translation);
      }
    }

    if (first) {
      throw std::logic_error("bad file format");
    }
  }
  storage.emplace(dict, newDict);
}
