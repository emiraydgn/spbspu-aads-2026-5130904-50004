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
    Iterator();

    T& operator*() const;
    T* operator->() const;

    Iterator& operator++();
    Iterator operator++(int);

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

  private:
    explicit Iterator(detail::Node< T >* node);

    detail::Node< T >* node_;
  };

  template< class T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< class T >
  T& Iterator< T >::operator*() const
  {
    return node_->data;
  }

  template< class T >
  T* Iterator< T >::operator->() const
  {
    return &(node_->data);
  }

  template< class T >
  Iterator< T >& Iterator< T >::operator++()
  {
    if (node_ != nullptr)
    {
      node_ = node_->next;
    }
    return *this;
  }

  template< class T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator tmp(*this);
    ++(*this);
    return tmp;
  }

  template< class T >
  bool Iterator< T >::operator==(const Iterator& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool Iterator< T >::operator!=(const Iterator& other) const
  {
    return node_ != other.node_;
  }

  template< class T >
  Iterator< T >::Iterator(detail::Node< T >* node):
    node_(node)
  {}
}

#endif
