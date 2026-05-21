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

void aydogan::Graph::addEdgeDirect(
  const std::string& from,
  const std::string& to,
  unsigned int weight
)
{
  addVertexDirect(from);
  addVertexDirect(to);

  EdgeKey key(from, to);
  WeightList* weights = edges_.find(key);

  if (weights != nullptr)
  {
    weights->push_back(weight);
  }
  else
  {
    WeightList newWeights;
    newWeights.push_back(weight);
    edges_.insert(key, newWeights);
  }
}

void aydogan::Graph::addEdge(
  const std::string& from,
  const std::string& to,
  unsigned int weight
)
{
  Graph copy(*this);
  copy.addEdgeDirect(from, to, weight);
  swap(copy);
}

bool aydogan::Graph::removeEdge(
  const std::string& from,
  const std::string& to,
  unsigned int weight
)
{
  Graph copy(*this);

  EdgeKey key(from, to);
  WeightList* weights = copy.edges_.find(key);

  if (weights == nullptr)
  {
    return false;
  }

  for (auto it = weights->begin(); it != weights->end(); ++it)
  {
    if (*it == weight)
    {
      weights->erase(it);

      if (weights->empty())
      {
        copy.edges_.erase(key);
      }

      swap(copy);
      return true;
    }
  }

  return false;
}

std::vector< std::string > aydogan::Graph::getVertices() const
{
  return vertices_;
}

std::vector< aydogan::EdgeInfo > aydogan::Graph::getOutbound(const std::string& vertex) const
{
  std::vector< EdgeInfo > result;

  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->key.from == vertex)
    {
      for (unsigned int weight: it->value)
      {
        result.push_back(EdgeInfo(it->key.to, weight));
      }
    }
  }

  sortEdges(result);
  return result;
}

std::vector< aydogan::EdgeInfo > aydogan::Graph::getInbound(const std::string& vertex) const
{
  std::vector< EdgeInfo > result;

  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->key.to == vertex)
    {
      for (unsigned int weight: it->value)
      {
        result.push_back(EdgeInfo(it->key.from, weight));
      }
    }
  }

  sortEdges(result);
  return result;
}

aydogan::Graph aydogan::Graph::mergeWith(const Graph& other) const
{
  Graph result(*this);

  for (const std::string& vertex: other.vertices_)
  {
    result.addVertexDirect(vertex);
  }

  for (auto it = other.edges_.cbegin(); it != other.edges_.cend(); ++it)
  {
    for (unsigned int weight: it->value)
    {
      result.addEdgeDirect(it->key.from, it->key.to, weight);
    }
  }

  return result;
}

aydogan::Graph aydogan::Graph::extractSubgraph(const std::vector< std::string >& selected) const
{
  Graph result;

  for (const std::string& vertex: selected)
  {
    result.addVertexDirect(vertex);
  }

  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (result.hasVertex(it->key.from) && result.hasVertex(it->key.to))
    {
      for (unsigned int weight: it->value)
      {
        result.addEdgeDirect(it->key.from, it->key.to, weight);
      }
    }
  }

  return result;
}

void aydogan::Graph::sortVertices()
{
  std::sort(vertices_.begin(), vertices_.end());
}

void aydogan::Graph::sortEdges(std::vector< EdgeInfo >& edges)
{
  std::sort(edges.begin(), edges.end(), [](const EdgeInfo& lhs, const EdgeInfo& rhs)
  {
    if (lhs.vertex != rhs.vertex)
    {
      return lhs.vertex < rhs.vertex;
    }

    return lhs.weight < rhs.weight;
  });
}
