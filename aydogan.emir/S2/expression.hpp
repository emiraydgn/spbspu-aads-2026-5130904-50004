#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iosfwd>
#include <string>

namespace aydogan
{
  long long calculateExpression(const std::string& expression);
  int run(std::istream& in, std::ostream& out, std::ostream& err);
}

#endif
