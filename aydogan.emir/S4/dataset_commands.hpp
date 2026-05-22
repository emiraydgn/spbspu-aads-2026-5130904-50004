#ifndef DATASET_COMMANDS_HPP
#define DATASET_COMMANDS_HPP

#include "bst.hpp"

#include <iosfwd>
#include <string>
#include <vector>

namespace aydogan
{
  class DatasetCommands
  {
  public:
    DatasetCommands();

    void loadFromFile(const std::string& filename);
    void run(std::istream& input, std::ostream& output);

  private:
    using Dictionary = BSTree< unsigned int, std::string >;
    using Storage = BSTree< std::string, Dictionary >;
    using Tokens = std::vector< std::string >;
    using Handler = void (*)(DatasetCommands&, const Tokens&, std::ostream&);

    void registerCommands();
    void processLine(const std::string& line, std::ostream& output);

    static Tokens tokenize(const std::string& line);
    static bool parseUnsigned(const std::string& text, unsigned int& value);

    static void printInvalid(std::ostream& output);
    static void printDictionary(
      const std::string& name,
      const Dictionary& dictionary,
      std::ostream& output
    );

    static Dictionary makeComplement(const Dictionary& left, const Dictionary& right);
    static Dictionary makeIntersection(const Dictionary& left, const Dictionary& right);
    static Dictionary makeUnion(const Dictionary& left, const Dictionary& right);

    static void handlePrint(DatasetCommands& processor, const Tokens& tokens, std::ostream& output);
    static void handleComplement(DatasetCommands& processor, const Tokens& tokens, std::ostream& output);
    static void handleIntersect(DatasetCommands& processor, const Tokens& tokens, std::ostream& output);
    static void handleUnion(DatasetCommands& processor, const Tokens& tokens, std::ostream& output);

    Storage dictionaries_;
    BSTree< std::string, Handler > commandTable_;
  };
}

#endif
