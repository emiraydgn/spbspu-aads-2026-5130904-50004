#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
namespace aydogan
{
  template< class T >
  class List
  {
  public:
    List();
  head_(nullptr)
{}

  private:
  struct Node
{
  T data;
  Node* next;

  Node(const T& value, Node* nextNode):
    data(value),
    next(nextNode)
  {}
};
    struct Node;
    Node* head_;
  };
}

#endif
