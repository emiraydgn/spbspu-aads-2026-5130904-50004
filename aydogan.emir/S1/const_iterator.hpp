#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include "iterator.hpp"

namespace aydogan
{
  template< class T >
  class List;

  template< class T >
  class ConstIterator
  {
    friend class List< T >;

  public:
    ConstIterator():
      node_(nullptr)
    {}

    ConstIterator(const Iterator< T >& other):
      node_(other.node_)
    {}

    const T& operator*() const
    {
      return node_->data;
    }

    const T* operator->() const
    {
      return &(node_->data);
    }

    ConstIterator& operator++()
    {
      if (node_ != nullptr)
      {
        node_ = node_->next;
      }
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const ConstIterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return node_ != other.node_;
    }

  private:
    explicit ConstIterator(const detail::Node< T >* node):
      node_(node)
    {}

    const detail::Node< T >* node_;
  };
}

#endif
