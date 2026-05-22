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

void aydogan::DatasetCommands::loadFromFile(const std::string& filename)
{
  std::ifstream input(filename);

  if (!input)
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;

  while (std::getline(input, line))
  {
    Tokens tokens = tokenize(line);

    if (tokens.empty())
    {
      continue;
    }

    if ((tokens.size() - 1) % 2 != 0)
    {
      continue;
    }

    Dictionary dictionary;

    for (std::size_t i = 1; i < tokens.size(); i += 2)
    {
      unsigned int key = 0;

      if (parseUnsigned(tokens[i], key))
      {
        dictionary.push(key, tokens[i + 1]);
      }
    }

    dictionaries_.push(tokens[0], dictionary);
  }
}

void aydogan::DatasetCommands::run(std::istream& input, std::ostream& output)
{
  std::string line;

  while (std::getline(input, line))
  {
    processLine(line, output);
  }
}

void aydogan::DatasetCommands::processLine(const std::string& line, std::ostream& output)
{
  Tokens tokens = tokenize(line);

  if (tokens.empty())
  {
    return;
  }

  auto handler = commandTable_.find(tokens[0]);

  if (handler == commandTable_.end())
  {
    printInvalid(output);
    return;
  }

  handler->second(*this, tokens, output);
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

void aydogan::DatasetCommands::printDictionary(
  const std::string& name,
  const Dictionary& dictionary,
  std::ostream& output
)
{
  if (dictionary.empty())
  {
    output << "<EMPTY>\n";
    return;
  }

  output << name;

  for (auto it = dictionary.cbegin(); it != dictionary.cend(); ++it)
  {
    output << " " << it->first << " " << it->second;
  }

  output << "\n";
}

aydogan::DatasetCommands::Dictionary aydogan::DatasetCommands::makeComplement(
  const Dictionary& left,
  const Dictionary& right
)
{
  Dictionary result;

  for (auto it = left.cbegin(); it != left.cend(); ++it)
  {
    if (right.find(it->first) == right.cend())
    {
      result.push(it->first, it->second);
    }
  }

  return result;
}

aydogan::DatasetCommands::Dictionary aydogan::DatasetCommands::makeIntersection(
  const Dictionary& left,
  const Dictionary& right
)
{
  Dictionary result;

  for (auto it = left.cbegin(); it != left.cend(); ++it)
  {
    if (right.find(it->first) != right.cend())
    {
      result.push(it->first, it->second);
    }
  }

  return result;
}

aydogan::DatasetCommands::Dictionary aydogan::DatasetCommands::makeUnion(
  const Dictionary& left,
  const Dictionary& right
)
{
  Dictionary result;

  for (auto it = left.cbegin(); it != left.cend(); ++it)
  {
    result.push(it->first, it->second);
  }

  for (auto it = right.cbegin(); it != right.cend(); ++it)
  {
    if (left.find(it->first) == left.cend())
    {
      result.push(it->first, it->second);
    }
  }

  return result;
}

void aydogan::DatasetCommands::handlePrint(
  DatasetCommands& processor,
  const Tokens& tokens,
  std::ostream& output
)
{
  if (tokens.size() != 2)
  {
    printInvalid(output);
    return;
  }

  auto dictionary = processor.dictionaries_.find(tokens[1]);

  if (dictionary == processor.dictionaries_.end())
  {
    printInvalid(output);
    return;
  }

  printDictionary(tokens[1], dictionary->second, output);
}

void aydogan::DatasetCommands::handleComplement(
  DatasetCommands& processor,
  const Tokens& tokens,
  std::ostream& output
)
{
  if (tokens.size() != 4)
  {
    printInvalid(output);
    return;
  }

  auto left = processor.dictionaries_.find(tokens[2]);
  auto right = processor.dictionaries_.find(tokens[3]);

  if (left == processor.dictionaries_.end() || right == processor.dictionaries_.end())
  {
    printInvalid(output);
    return;
  }

  Dictionary result = makeComplement(left->second, right->second);
  processor.dictionaries_.push(tokens[1], result);
}

void aydogan::DatasetCommands::handleIntersect(
  DatasetCommands& processor,
  const Tokens& tokens,
  std::ostream& output
)
{
  if (tokens.size() != 4)
  {
    printInvalid(output);
    return;
  }

  auto left = processor.dictionaries_.find(tokens[2]);
  auto right = processor.dictionaries_.find(tokens[3]);

  if (left == processor.dictionaries_.end() || right == processor.dictionaries_.end())
  {
    printInvalid(output);
    return;
  }

  Dictionary result = makeIntersection(left->second, right->second);
  processor.dictionaries_.push(tokens[1], result);
}

void aydogan::DatasetCommands::handleUnion(
  DatasetCommands& processor,
  const Tokens& tokens,
  std::ostream& output
)
{
  if (tokens.size() != 4)
  {
    printInvalid(output);
    return;
  }

  auto left = processor.dictionaries_.find(tokens[2]);
  auto right = processor.dictionaries_.find(tokens[3]);

  if (left == processor.dictionaries_.end() || right == processor.dictionaries_.end())
  {
    printInvalid(output);
    return;
  }

  Dictionary result = makeUnion(left->second, right->second);
  processor.dictionaries_.push(tokens[1], result);
}
