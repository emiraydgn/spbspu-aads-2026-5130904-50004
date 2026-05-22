#include "dataset_commands.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>

aydogan::DatasetCommands::DatasetCommands():
  dictionaries_(),
  commandTable_()
{
  registerCommands();
}

void aydogan::DatasetCommands::registerCommands()
{
  commandTable_.push("print", &DatasetCommands::handlePrint);
  commandTable_.push("complement", &DatasetCommands::handleComplement);
  commandTable_.push("intersect", &DatasetCommands::handleIntersect);
  commandTable_.push("union", &DatasetCommands::handleUnion);
}

aydogan::DatasetCommands::Tokens aydogan::DatasetCommands::tokenize(const std::string& line)
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

bool aydogan::DatasetCommands::parseUnsigned(const std::string& text, unsigned int& value)
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

void aydogan::DatasetCommands::printInvalid(std::ostream& output)
{
  output << "<INVALID COMMAND>\n";
}
