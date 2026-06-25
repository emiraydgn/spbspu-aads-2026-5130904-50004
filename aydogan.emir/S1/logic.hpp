#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iosfwd>
#include <string>
#include <utility>

#include "list.hpp"

namespace aydogan
{
  using NumberList = List< unsigned long long >;
  using NamedSequence = std::pair< std::string, NumberList >;
  using SequenceList = List< NamedSequence >;

  void readInput(std::istream& in, SequenceList& data);
  void printNames(const SequenceList& data, std::ostream& out);
  int printRowsAndSums(const SequenceList& data, std::ostream& out, std::ostream& err);
}

#endif
