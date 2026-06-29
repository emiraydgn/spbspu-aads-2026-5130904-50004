#include "dictionary.hpp"
#include <cctype>
#include <fstream>
#include <iomanip>
#include <stdexcept>

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

    List< std::string > readDictNames(std::istream & in)
    {
      std::string line;
      std::getline(in, line);

      List< std::string > dicts;
      Iterator< std::string > tail = dicts.beforeBegin();
      std::size_t pos = 0;

      while (pos < line.size()) {
        std::string name = readToken(line, pos);
        if (!name.empty()) {
          tail = dicts.insertAfter(tail, name);
        }
      }

      return dicts;
    }

    void checkDictsExist(const List< std::string > & dicts, const DictionaryStorage & storage)
    {
      for (ConstIterator< std::string > it = dicts.cbegin(); it != dicts.cend(); ++it) {
        if (!storage.count(*it)) {
          throw std::logic_error("dictionary not found: " + *it);
        }
      }
    }

    std::size_t countNames(const List< std::string > & dicts)
    {
      std::size_t count = 0;

      for (ConstIterator< std::string > it = dicts.cbegin(); it != dicts.cend(); ++it) {
        ++count;
      }

      return count;
    }

    void addWordToDict(
        Dictionary & dict,
        const std::string & key,
        const List< std::string > & translations
    )
    {
      bool first = true;

      for (ConstIterator< std::string > it = translations.cbegin();
          it != translations.cend();
          ++it) {
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

  for (ConstIterator< std::string > it = translations.cbegin();
      it != translations.cend();
      ++it) {
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

  List< std::string > dicts = detail::readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }

  detail::checkDictsExist(dicts, storage);

  Dictionary merged;

  for (ConstIterator< std::string > dictIt = dicts.cbegin();
      dictIt != dicts.cend();
      ++dictIt) {
    List< std::string > keys = storage.at(*dictIt).keys();

    for (ConstIterator< std::string > keyIt = keys.cbegin();
        keyIt != keys.cend();
        ++keyIt) {
      List< std::string > translations = storage.at(*dictIt).get(*keyIt);

      if (!merged.contains(*keyIt)) {
        detail::addWordToDict(merged, *keyIt, translations);
      } else {
        List< std::string > existing = merged.get(*keyIt);

        for (ConstIterator< std::string > transIt = translations.cbegin();
            transIt != translations.cend();
            ++transIt) {
          if (!detail::containsTranslation(existing, *transIt)) {
            merged.addTranslation(*keyIt, *transIt);
            existing = merged.get(*keyIt);
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

  List< std::string > dicts = detail::readDictNames(in);
  if (dicts.empty()) {
    throw std::logic_error("no dictionaries specified");
  }

  detail::checkDictsExist(dicts, storage);

  Dictionary united;

  for (ConstIterator< std::string > dictIt = dicts.cbegin();
      dictIt != dicts.cend();
      ++dictIt) {
    List< std::string > keys = storage.at(*dictIt).keys();

    for (ConstIterator< std::string > keyIt = keys.cbegin();
        keyIt != keys.cend();
        ++keyIt) {
      if (!united.contains(*keyIt)) {
        detail::addWordToDict(united, *keyIt, storage.at(*dictIt).get(*keyIt));
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

  List< std::string > dicts = detail::readDictNames(in);
  if (detail::countNames(dicts) < 2) {
    throw std::logic_error("at least two dictionaries required");
  }

  detail::checkDictsExist(dicts, storage);

  Dictionary diff;
  ConstIterator< std::string > firstIt = dicts.cbegin();
  List< std::string > firstKeys = storage.at(*firstIt).keys();

  for (ConstIterator< std::string > keyIt = firstKeys.cbegin();
      keyIt != firstKeys.cend();
      ++keyIt) {
    bool inOther = false;
    ConstIterator< std::string > dictIt = firstIt;
    ++dictIt;

    while (dictIt != dicts.cend()) {
      if (storage.at(*dictIt).contains(*keyIt)) {
        inOther = true;
        break;
      }

      ++dictIt;
    }

    if (!inOther) {
      detail::addWordToDict(diff, *keyIt, storage.at(*firstIt).get(*keyIt));
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

  List< std::string > keys = storage.at(dict).keys();

  for (ConstIterator< std::string > keyIt = keys.cbegin();
      keyIt != keys.cend();
      ++keyIt) {
    file << *keyIt;
    List< std::string > translations = storage.at(dict).get(*keyIt);

    for (ConstIterator< std::string > transIt = translations.cbegin();
        transIt != translations.cend();
        ++transIt) {
      file << ' ' << std::quoted(*transIt);
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
