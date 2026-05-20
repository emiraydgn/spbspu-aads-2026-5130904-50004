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

  class Graph
  {
  public:
    Graph();

    void swap(Graph& other) noexcept;

    bool hasVertex(const std::string& vertex) const;

    void addVertex(const std::string& vertex);
    void addEdge(const std::string& from, const std::string& to, unsigned int weight);
    bool removeEdge(const std::string& from, const std::string& to, unsigned int weight);

    std::vector< std::string > getVertices() const;
    std::vector< EdgeInfo > getOutbound(const std::string& vertex) const;
    std::vector< EdgeInfo > getInbound(const std::string& vertex) const;

  private:
    using WeightList = std::vector< unsigned int >;
    using EdgeTable = HashTable< EdgeKey, WeightList, EdgeKeyHash, EdgeKeyEqual >;

    std::vector< std::string > vertices_;
    EdgeTable edges_;
  };
}

#endif
