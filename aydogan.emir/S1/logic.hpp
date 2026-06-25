#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "list.hpp"
#include <iosfwd>
#include <string>
#include <utility>

namespace aydogan
{
  using NumberList = List< unsigned long long >;
  using NamedSequence = std::pair< std::string, NumberList >;
  using SequenceList = List< NamedSequence >;

  void readInput(std::istream& in, SequenceList& data);

  int runProgram(const SequenceList& data, std::ostream& out, std::ostream& err);
}

#endif
