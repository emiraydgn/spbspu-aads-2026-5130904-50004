#include "command_processor.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>

aydogan::CommandProcessor::CommandProcessor():
  commandTable_(16),
  graphStorage_(16)
{
  registerCommands();
}

void aydogan::CommandProcessor::registerCommands()
{
  commandTable_.insert("graphs", &CommandProcessor::handleGraphs);
  commandTable_.insert("vertexes", &CommandProcessor::handleVertexes);
  commandTable_.insert("outbound", &CommandProcessor::handleOutbound);
  commandTable_.insert("inbound", &CommandProcessor::handleInbound);
  commandTable_.insert("bind", &CommandProcessor::handleBind);
  commandTable_.insert("cut", &CommandProcessor::handleCut);
  commandTable_.insert("create", &CommandProcessor::handleCreate);
  commandTable_.insert("merge", &CommandProcessor::handleMerge);
  commandTable_.insert("extract", &CommandProcessor::handleExtract);
}

void aydogan::CommandProcessor::loadGraphs(const std::string& filename)
{
  std::ifstream input(filename);

  if (!input)
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;

  while (std::getline(input, line))
  {
    Tokens header = tokenize(line);

    if (header.empty())
    {
      continue;
    }

    if (header.size() != 2)
    {
      continue;
    }

    unsigned int edgeCount = 0;

    if (!parseUnsigned(header[1], edgeCount))
    {
      continue;
    }

    Graph graph;

    for (unsigned int i = 0; i < edgeCount; ++i)
    {
      std::string edgeLine;

      if (!std::getline(input, edgeLine))
      {
        break;
      }

      Tokens edgeTokens = tokenize(edgeLine);

      if (edgeTokens.size() != 3)
      {
        continue;
      }

      unsigned int weight = 0;

      if (parseUnsigned(edgeTokens[2], weight))
      {
        graph.addEdge(edgeTokens[0], edgeTokens[1], weight);
      }
    }

    graphStorage_.insert(header[0], graph);
  }
}

void aydogan::CommandProcessor::run(std::istream& input, std::ostream& output)
{
  std::string line;

  while (std::getline(input, line))
  {
    processLine(line, output);
  }
}

void aydogan::CommandProcessor::processLine(const std::string& line, std::ostream& output)
{
  Tokens tokens = tokenize(line);

  if (tokens.empty())
  {
    return;
  }

  Handler* handler = commandTable_.find(tokens[0]);

  if (handler == nullptr)
  {
    printInvalid(output);
    return;
  }

  (*handler)(*this, tokens, output);
}

aydogan::CommandProcessor::Tokens aydogan::CommandProcessor::tokenize(const std::string& line)
{
  Tokens tokens;
  std::string current;

  for (char symbol: line)
  {
    if (std::isspace(static_cast< unsigned char >(symbol)))
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
    else
    {
      current += symbol;
    }
  }

  if (!current.empty())
  {
    tokens.push_back(current);
  }

  return tokens;
}

bool aydogan::CommandProcessor::parseUnsigned(const std::string& text, unsigned int& value)
{
  if (text.empty())
  {
    return false;
  }

  unsigned long long result = 0;

  for (char symbol: text)
  {
    if (!std::isdigit(static_cast< unsigned char >(symbol)))
    {
      return false;
    }

    result = result * 10 + static_cast< unsigned int >(symbol - '0');

    if (result > std::numeric_limits< unsigned int >::max())
    {
      return false;
    }
  }

  value = static_cast< unsigned int >(result);
  return true;
}

bool aydogan::CommandProcessor::hasDuplicates(const std::vector< std::string >& values)
{
  std::vector< std::string > copy(values);
  std::sort(copy.begin(), copy.end());

  for (std::size_t i = 1; i < copy.size(); ++i)
  {
    if (copy[i] == copy[i - 1])
    {
      return true;
    }
  }

  return false;
}

void aydogan::CommandProcessor::printInvalid(std::ostream& output)
{
  output << "<INVALID COMMAND>\n";
}

void aydogan::CommandProcessor::printEdges(
  const std::vector< EdgeInfo >& edges,
  std::ostream& output
)
{
  if (edges.empty())
  {
    output << "\n";
    return;
  }

  std::string currentVertex = edges[0].vertex;
  output << currentVertex << " " << edges[0].weight;

  for (std::size_t i = 1; i < edges.size(); ++i)
  {
    if (edges[i].vertex == currentVertex)
    {
      output << " " << edges[i].weight;
    }
    else
    {
      currentVertex = edges[i].vertex;
      output << "\n" << currentVertex << " " << edges[i].weight;
    }
  }

  output << "\n";
}
