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
