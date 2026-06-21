#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>

#include "dictionary.hpp"

int main()
{
  DictionaryStorage storage;

  using cmd_t = void (*)(std::istream &, std::ostream &, DictionaryStorage &);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["create-dict"] = createDict;
  cmds["show"] = showDict;
  cmds["drop-dict"] = dropDict;
  cmds["add-word"] = addWord;
  cmds["remove-word"] = removeWord;
  cmds["add-translation"] = addTranslation;
  cmds["remove-translation"] = removeTranslation;
  cmds["translate"] = translate;
  cmds["merge-dict"] = mergeDict;
  cmds["union"] = unionDict;
  cmds["difference"] = differenceDict;
  cmds["count"] = countDict;
  cmds["help"] = help;

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmd == "exit") {
      break;
    }

    try {
      cmds.at(cmd)(std::cin, std::cout, storage);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
