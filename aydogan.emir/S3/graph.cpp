#include "graph.hpp"
#include <algorithm>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

std::size_t aydogan::EdgeKeyHash::operator()(const EdgeKey& edge) const
{
  boost::hash2::siphash_64 hasher;
  boost::hash2::hash_append(hasher, {}, edge.from);
  boost::hash2::hash_append(hasher, {}, edge.to);
  return static_cast< std::size_t >(hasher.result());
}

bool aydogan::EdgeKeyEqual::operator()(const EdgeKey& lhs, const EdgeKey& rhs) const
{
  return lhs.from == rhs.from && lhs.to == rhs.to;
}

aydogan::Graph::Graph():
  vertices_(),
  edges_(16)
{}

void aydogan::Graph::swap(Graph& other) noexcept
{
  vertices_.swap(other.vertices_);
  edges_.swap(other.edges_);
}

bool aydogan::Graph::hasVertex(const std::string& vertex) const
{
  return std::find(vertices_.begin(), vertices_.end(), vertex) != vertices_.end();
}

void aydogan::Graph::addVertexDirect(const std::string& vertex)
{
  if (!hasVertex(vertex))
  {
    vertices_.push_back(vertex);
    sortVertices();
  }
}

void aydogan::Graph::addVertex(const std::string& vertex)
{
  Graph copy(*this);
  copy.addVertexDirect(vertex);
  swap(copy);
}
