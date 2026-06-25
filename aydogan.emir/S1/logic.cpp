#include "logic.hpp"

#include <cctype>
#include <cstdio>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
  using IteratorList = aydogan::List< aydogan::ConstIterator< unsigned long long > >;

  bool printRowAndCalculateSum(IteratorList& iters,
    const aydogan::SequenceList& data,
    std::ostream& out,
    unsigned long long& currentSum,
    bool& overflow)
  {
    bool rowHasValues = false;
    const unsigned long long maxValue =
      std::numeric_limits< unsigned long long >::max();

    auto iterNode = iters.begin();
    auto seqNode = data.cbegin();

    while (iterNode != iters.end())
    {
      if (*iterNode != seqNode->second.cend())
      {
        unsigned long long value = **iterNode;

        if (rowHasValues)
        {
          out << " ";
        }
        out << value;

        if (maxValue - currentSum < value)
        {
          overflow = true;
        }
        else
        {
          currentSum += value;
        }

        ++(*iterNode);
        rowHasValues = true;
      }

      ++iterNode;
      ++seqNode;
    }

    return rowHasValues;
  }
}

namespace aydogan
{
  void readInput(std::istream& in, SequenceList& data)
  {
    std::string name;
    auto seqTail = data.beforeBegin();

    while (in >> name)
    {
      NumberList numbers;
      auto numTail = numbers.beforeBegin();

      while (true)
      {
        unsigned long long value = 0;

        if (in >> value)
        {
          numTail = numbers.insertAfter(numTail, value);
        }
        else
        {
          if (in.eof())
          {
            break;
          }

          in.clear();

          int c = in.peek();
          if (c == EOF)
          {
            break;
          }

          if (std::isalpha(static_cast< unsigned char >(c)))
          {
            break;
          }

          throw std::overflow_error("Overflow");
        }
      }

      seqTail = data.insertAfter(
        seqTail,
        std::make_pair(name, std::move(numbers))
      );
    }
  }

  void printNames(const SequenceList& data, std::ostream& out)
  {
    auto it = data.cbegin();

    out << it->first;
    ++it;

    for (; it != data.cend(); ++it)
    {
      out << " " << it->first;
    }
    out << "\n";
  }

  int printRowsAndSums(const SequenceList& data, std::ostream& out, std::ostream& err)
  {
    List< ConstIterator< unsigned long long > > iters;
    auto iterTail = iters.beforeBegin();

    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
      iterTail = iters.insertAfter(iterTail, it->second.cbegin());
    }

    List< unsigned long long > sums;
    auto sumTail = sums.beforeBegin();

    while (true)
    {
      unsigned long long currentSum = 0;
      bool overflow = false;

      bool rowHasValues = printRowAndCalculateSum(
        iters,
        data,
        out,
        currentSum,
        overflow
      );

      if (!rowHasValues)
      {
        break;
      }

      out << "\n";

      if (overflow)
      {
        err << "Overflow\n";
        return 1;
      }

      sumTail = sums.insertAfter(sumTail, currentSum);
    }

    if (sums.empty())
    {
      out << "0\n";
    }
    else
    {
      auto sumIt = sums.cbegin();

      out << *sumIt;
      ++sumIt;

      for (; sumIt != sums.cend(); ++sumIt)
      {
        out << " " << *sumIt;
      }
      out << "\n";
    }

    return 0;
  }
}
