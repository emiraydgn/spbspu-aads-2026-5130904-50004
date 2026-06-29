#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

#include "dictionary.hpp"

int main()
{
  DictionaryStorage storage;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmd == "exit") {
        break;
      } else if (cmd == "create-dict") {
        createDict(std::cin, std::cout, storage);
      } else if (cmd == "show") {
        showDict(std::cin, std::cout, storage);
      } else if (cmd == "drop-dict") {
        dropDict(std::cin, std::cout, storage);
      } else if (cmd == "add-word") {
        addWord(std::cin, std::cout, storage);
      } else if (cmd == "remove-word") {
        removeWord(std::cin, std::cout, storage);
      } else if (cmd == "add-translation") {
        addTranslation(std::cin, std::cout, storage);
      } else if (cmd == "remove-translation") {
        removeTranslation(std::cin, std::cout, storage);
      } else if (cmd == "translate") {
        translate(std::cin, std::cout, storage);
      } else if (cmd == "merge-dict") {
        mergeDict(std::cin, std::cout, storage);
      } else if (cmd == "union") {
        unionDict(std::cin, std::cout, storage);
      } else if (cmd == "difference") {
        differenceDict(std::cin, std::cout, storage);
      } else if (cmd == "count") {
        countDict(std::cin, std::cout, storage);
      } else if (cmd == "save") {
        saveDict(std::cin, std::cout, storage);
      } else if (cmd == "load") {
        loadDict(std::cin, std::cout, storage);
      } else if (cmd == "help") {
        help(std::cin, std::cout, storage);
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
