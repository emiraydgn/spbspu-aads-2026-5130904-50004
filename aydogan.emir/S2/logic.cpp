#include "logic.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
  bool isOperator(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "%" || token == "**";
  }

  int precedence(const std::string& token)
  {
    if (token == "**")
    {
      return 3;
    }
    if (token == "*" || token == "/" || token == "%")
    {
      return 2;
    }
    if (token == "+" || token == "-")
    {
      return 1;
    }
    return 0;
  }

  bool isRightAssociative(const std::string& token)
  {
    return token == "**";
  }

  bool isNumber(const std::string& token)
  {
    if (token.empty())
    {
      return false;
    }

    std::size_t start = 0;
    if (token[0] == '-')
    {
      if (token.size() == 1)
      {
        return false;
      }
      start = 1;
    }

    for (std::size_t i = start; i < token.size(); ++i)
    {
      if (!std::isdigit(static_cast< unsigned char >(token[i])))
      {
        return false;
      }
    }

    return true;
  }

  long long parseNumber(const std::string& token)
  {
    std::size_t pos = 0;
    long long value = 0;

    try
    {
      value = std::stoll(token, &pos);
    }
    catch (const std::exception&)
    {
      throw std::runtime_error("Invalid expression");
    }

    if (pos != token.size())
    {
      throw std::runtime_error("Invalid expression");
    }

    return value;
  }

  long long checkedAdd(long long lhs, long long rhs)
  {
    __int128 value = static_cast< __int128 >(lhs) + static_cast< __int128 >(rhs);
    if (value < std::numeric_limits< long long >::min() ||
        value > std::numeric_limits< long long >::max())
    {
      throw std::overflow_error("Overflow");
    }
    return static_cast< long long >(value);
  }

  long long checkedSub(long long lhs, long long rhs)
  {
    __int128 value = static_cast< __int128 >(lhs) - static_cast< __int128 >(rhs);
    if (value < std::numeric_limits< long long >::min() ||
        value > std::numeric_limits< long long >::max())
    {
      throw std::overflow_error("Overflow");
    }
    return static_cast< long long >(value);
  }

  long long checkedMul(long long lhs, long long rhs)
  {
    __int128 value = static_cast< __int128 >(lhs) * static_cast< __int128 >(rhs);
    if (value < std::numeric_limits< long long >::min() ||
        value > std::numeric_limits< long long >::max())
    {
      throw std::overflow_error("Overflow");
    }
    return static_cast< long long >(value);
  }

  long long checkedDiv(long long lhs, long long rhs)
  {
    if (rhs == 0)
    {
      throw std::runtime_error("Division by zero");
    }
    if (lhs == std::numeric_limits< long long >::min() && rhs == -1)
    {
      throw std::overflow_error("Overflow");
    }
    return lhs / rhs;
  }

  long long checkedMod(long long lhs, long long rhs)
  {
    if (rhs == 0)
    {
      throw std::runtime_error("Division by zero");
    }
    if (lhs == std::numeric_limits< long long >::min() && rhs == -1)
    {
      throw std::overflow_error("Overflow");
    }
    return lhs % rhs;
  }

  long long checkedPow(long long lhs, long long rhs)
  {
    if (rhs < 0)
    {
      throw std::runtime_error("Invalid expression");
    }

    long long result = 1;
    long long base = lhs;
    long long power = rhs;

    while (power > 0)
    {
      if (power % 2 == 1)
      {
        result = checkedMul(result, base);
      }

      power /= 2;

      if (power > 0)
      {
        base = checkedMul(base, base);
      }
    }

    return result;
  }

  long long applyOperator(long long lhs, long long rhs, const std::string& op)
  {
    if (op == "+")
    {
      return checkedAdd(lhs, rhs);
    }
    if (op == "-")
    {
      return checkedSub(lhs, rhs);
    }
    if (op == "*")
    {
      return checkedMul(lhs, rhs);
    }
    if (op == "/")
    {
      return checkedDiv(lhs, rhs);
    }
    if (op == "%")
    {
      return checkedMod(lhs, rhs);
    }
    if (op == "**")
    {
      return checkedPow(lhs, rhs);
    }

    throw std::runtime_error("Invalid expression");
  }

  aydogan::Queue< std::string > toPostfix(const std::string& expression)
  {
    aydogan::Queue< std::string > output;
    aydogan::Stack< std::string > operators;
    std::istringstream input(expression);
    std::string token;

    while (input >> token)
    {
      if (isNumber(token))
      {
        output.push(token);
      }
      else if (token == "(")
      {
        operators.push(token);
      }
      else if (token == ")")
      {
        while (!operators.empty() && operators.top() != "(")
        {
          output.push(operators.drop());
        }

        if (operators.empty())
        {
          throw std::runtime_error("Invalid expression");
        }

        operators.drop();
      }
      else if (isOperator(token))
      {
        while (!operators.empty() && isOperator(operators.top()))
        {
          const std::string& topOp = operators.top();
          bool shouldPop = false;

          if (isRightAssociative(token))
          {
            shouldPop = precedence(token) < precedence(topOp);
          }
          else
          {
            shouldPop = precedence(token) <= precedence(topOp);
          }

          if (!shouldPop)
          {
            break;
          }

          output.push(operators.drop());
        }

        operators.push(token);
      }
      else
      {
        throw std::runtime_error("Invalid expression");
      }
    }

    while (!operators.empty())
    {
      if (operators.top() == "(" || operators.top() == ")")
      {
        throw std::runtime_error("Invalid expression");
      }

      output.push(operators.drop());
    }

    return output;
  }

  long long evaluatePostfix(aydogan::Queue< std::string > postfix)
  {
    aydogan::Stack< long long > values;

    while (!postfix.empty())
    {
      std::string token = postfix.drop();

      if (isNumber(token))
      {
        values.push(parseNumber(token));
      }
      else if (isOperator(token))
      {
        if (values.empty())
        {
          throw std::runtime_error("Invalid expression");
        }
        long long rhs = values.drop();

        if (values.empty())
        {
          throw std::runtime_error("Invalid expression");
        }
        long long lhs = values.drop();

        values.push(applyOperator(lhs, rhs, token));
      }
      else
      {
        throw std::runtime_error("Invalid expression");
      }
    }

    if (values.empty())
    {
      throw std::runtime_error("Invalid expression");
    }

    long long result = values.drop();

    if (!values.empty())
    {
      throw std::runtime_error("Invalid expression");
    }

    return result;
  }
}

long long aydogan::calculateExpression(const std::string& expression)
{
  Queue< std::string > postfix = toPostfix(expression);
  return evaluatePostfix(postfix);
}

int aydogan::run(std::istream& in, std::ostream& out, std::ostream& err)
{
  Stack< long long > results;
  std::string line;

  while (std::getline(in, line))
  {
    bool onlySpaces = true;
    for (char c: line)
    {
      if (!std::isspace(static_cast< unsigned char >(c)))
      {
        onlySpaces = false;
        break;
      }
    }

    if (onlySpaces)
    {
      continue;
    }

    try
    {
      results.push(calculateExpression(line));
    }
    catch (const std::exception& e)
    {
      err << e.what() << "\n";
      return 1;
    }
  }

  bool first = true;
  while (!results.empty())
  {
    if (!first)
    {
      out << " ";
    }
    out << results.drop();
    first = false;
  }

  if (!first)
  {
    out << "\n";
  }

  return 0;
}
