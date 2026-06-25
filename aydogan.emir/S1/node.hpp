#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace aydogan
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T data;
      Node* next;

      Node():
        data(),
        next(nullptr)
      {}

      Node(const T& value, Node* nextNode):
        data(value),
        next(nextNode)
      {}

      Node(T&& value, Node* nextNode):
        data(std::move(value)),
        next(nextNode)
      {}
    };
  }
}

#endif
