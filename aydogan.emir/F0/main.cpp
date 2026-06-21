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

  return 0;
}
