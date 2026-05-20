#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace aydogan
{
  struct EdgeKey
  {
    std::string from;
    std::string to;

    EdgeKey() = default;

    EdgeKey(const std::string& first, const std::string& second):
      from(first),
      to(second)
    {}
  };

  struct EdgeKeyHash
  {
    std::size_t operator()(const EdgeKey& edge) const;
  };

  struct EdgeKeyEqual
  {
    bool operator()(const EdgeKey& lhs, const EdgeKey& rhs) const;
  };

  struct EdgeInfo
  {
    std::string vertex;
    unsigned int weight;

    EdgeInfo(const std::string& name, unsigned int value):
      vertex(name),
      weight(value)
    {}
  };
}

#endif
