#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "dictionary.hpp"

int main()
{
  aydogan::DictionaryStorage storage;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmd == "exit") {
        break;
      } else if (cmd == "create-dict") {
        aydogan::createDict(std::cin, std::cout, storage);
      } else if (cmd == "show") {
        aydogan::showDict(std::cin, std::cout, storage);
      } else if (cmd == "drop-dict") {
        aydogan::dropDict(std::cin, std::cout, storage);
      } else if (cmd == "add-word") {
        aydogan::addWord(std::cin, std::cout, storage);
      } else if (cmd == "remove-word") {
        aydogan::removeWord(std::cin, std::cout, storage);
      } else if (cmd == "add-translation") {
        aydogan::addTranslation(std::cin, std::cout, storage);
      } else if (cmd == "remove-translation") {
        aydogan::removeTranslation(std::cin, std::cout, storage);
      } else if (cmd == "translate") {
        aydogan::translate(std::cin, std::cout, storage);
      } else if (cmd == "merge-dict") {
        aydogan::mergeDict(std::cin, std::cout, storage);
      } else if (cmd == "union") {
        aydogan::unionDict(std::cin, std::cout, storage);
      } else if (cmd == "difference") {
        aydogan::differenceDict(std::cin, std::cout, storage);
      } else if (cmd == "count") {
        aydogan::countDict(std::cin, std::cout, storage);
      } else if (cmd == "save") {
        aydogan::saveDict(std::cin, std::cout, storage);
      } else if (cmd == "load") {
        aydogan::loadDict(std::cin, std::cout, storage);
      } else if (cmd == "help") {
        aydogan::help(std::cin, std::cout, storage);
      } else {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    } catch (const std::logic_error & e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof() && cmd != "exit") {
    std::cerr << "Bad input\n";
    return 1;
  }

  return 0;
}
