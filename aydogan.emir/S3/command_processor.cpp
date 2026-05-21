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
