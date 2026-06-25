#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "node.hpp"

namespace aydogan
{
  template< class T >
  class List;

  template< class T >
  class ConstIterator;

  template< class T >
  class Iterator
  {
    friend class List< T >;
    friend class ConstIterator< T >;

  public:
    Iterator():
      node_(nullptr)
    {}

    T& operator*() const
    {
      return node_->data;
    }

    T* operator->() const
    {
      return &(node_->data);
    }

    Iterator& operator++()
    {
      if (node_ != nullptr)
      {
        node_ = node_->next;
      }
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const Iterator& other) const
    {
      return node_ != other.node_;
    }

  private:
    explicit Iterator(detail::Node< T >* node):
      node_(node)
    {}

    detail::Node< T >* node_;
  };
}

#endif
