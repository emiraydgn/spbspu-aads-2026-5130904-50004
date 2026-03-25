#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iosfwd>
#include <string>

namespace aydogan
{
  long long calculateExpression(const std::string& expression);
  int run(std::istream& in, std::ostream& out, std::ostream& err);
}

#endif
