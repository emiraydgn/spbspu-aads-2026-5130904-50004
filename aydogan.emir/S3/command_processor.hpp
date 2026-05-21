#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include "graph.hpp"
#include "hash_table.hpp"

#include <iosfwd>
#include <string>
#include <vector>

namespace aydogan
{
  class CommandProcessor
  {
  public:
    CommandProcessor();

    void loadGraphs(const std::string& filename);
    void run(std::istream& input, std::ostream& output);

  private:
    using Tokens = std::vector< std::string >;
    using Handler = void (*)(CommandProcessor&, const Tokens&, std::ostream&);

    void registerCommands();
    void processLine(const std::string& line, std::ostream& output);

    static Tokens tokenize(const std::string& line);
    static bool parseUnsigned(const std::string& text, unsigned int& value);
    static bool hasDuplicates(const std::vector< std::string >& values);

    static void printInvalid(std::ostream& output);
    static void printEdges(const std::vector< EdgeInfo >& edges, std::ostream& output);

    HashTable< std::string, Handler > commandTable_;
    HashTable< std::string, Graph > graphStorage_;
  };
}

#endif
