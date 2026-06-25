#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <utility>

#include "../common/list.hpp"

namespace aydogan
{
  template< class T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue& other);
    Queue(Queue&& other);
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other);

    bool empty() const noexcept;
    void push(const T& rhs);
    void push(T&& rhs);
    T& front();
    const T& front() const;
    T pop();

  private:
    void rebuildTail();

    List< T > data_;
    Iterator< T > tail_;
  };

  template< class T >
  Queue< T >::Queue():
    data_(),
    tail_(data_.beforeBegin())
  {}

  template< class T >
  Queue< T >::Queue(const Queue& other):
    data_(other.data_),
    tail_(data_.beforeBegin())
  {
    rebuildTail();
  }

  template< class T >
  Queue< T >::Queue(Queue&& other):
    data_(std::move(other.data_)),
    tail_(data_.beforeBegin())
  {
    rebuildTail();
    other.tail_ = other.data_.beforeBegin();
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(const Queue& other)
  {
    if (this != &other)
    {
      data_ = other.data_;
      rebuildTail();
    }
    return *this;
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(Queue&& other)
  {
    if (this != &other)
    {
      data_ = std::move(other.data_);
      rebuildTail();
      other.tail_ = other.data_.beforeBegin();
    }
    return *this;
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    if (empty())
    {
      tail_ = data_.insertAfter(data_.beforeBegin(), rhs);
    }
    else
    {
      tail_ = data_.insertAfter(tail_, rhs);
    }
  }

  template< class T >
  void Queue< T >::push(T&& rhs)
  {
    if (empty())
    {
      tail_ = data_.insertAfter(data_.beforeBegin(), std::move(rhs));
    }
    else
    {
      tail_ = data_.insertAfter(tail_, std::move(rhs));
    }
  }

  template< class T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::out_of_range("Queue is empty");
    }
    return data_.front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    if (empty())
    {
      throw std::out_of_range("Queue is empty");
    }
    return data_.front();
  }

  template< class T >
  T Queue< T >::pop()
  {
    if (empty())
    {
      throw std::out_of_range("Queue is empty");
    }

    bool wasLast = data_.begin() == tail_;
    T value = std::move(data_.front());
    data_.pop_front();

    if (wasLast)
    {
      tail_ = data_.beforeBegin();
    }

    return value;
  }

  template< class T >
  void Queue< T >::rebuildTail()
  {
    tail_ = data_.beforeBegin();
    for (auto it = data_.begin(); it != data_.end(); ++it)
    {
      tail_ = it;
    }
  }
}

#endif
