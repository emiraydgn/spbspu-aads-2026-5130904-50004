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
    ConstIterator();
    ConstIterator(const Iterator< T >& other);

    const T& operator*() const;
    const T* operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

  private:
    explicit ConstIterator(const detail::Node< T >* node);

    const detail::Node< T >* node_;
  };

  template< class T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< class T >
  ConstIterator< T >::ConstIterator(const Iterator< T >& other):
    node_(other.node_)
  {}

  template< class T >
  const T& ConstIterator< T >::operator*() const
  {
    return node_->data;
  }

  template< class T >
  const T* ConstIterator< T >::operator->() const
  {
    return &(node_->data);
  }

  template< class T >
  ConstIterator< T >& ConstIterator< T >::operator++()
  {
    if (node_ != nullptr)
    {
      node_ = node_->next;
    }
    return *this;
  }

  template< class T >
  ConstIterator< T > ConstIterator< T >::operator++(int)
  {
    ConstIterator tmp(*this);
    ++(*this);
    return tmp;
  }

  template< class T >
  bool ConstIterator< T >::operator==(const ConstIterator& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool ConstIterator< T >::operator!=(const ConstIterator& other) const
  {
    return node_ != other.node_;
  }

  template< class T >
  ConstIterator< T >::ConstIterator(const detail::Node< T >* node):
    node_(node)
  {}
}

#endif
