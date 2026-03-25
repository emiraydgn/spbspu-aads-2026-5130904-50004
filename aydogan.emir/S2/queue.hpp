#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "list.hpp"
#include <utility>
#include <stdexcept>

namespace aydogan
{
  template< class T >
  class Queue
  {
  public:
    Queue()
  data_(),
  tail_(data_.beforeBegin())
{}

bool empty() const noexcept
{
  return data_.empty();
}

void push(const T& rhs)
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

void push(T&& rhs)
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
T& front()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return data_.front();
}

const T& front() const
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return data_.front();
}
T drop()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }

  bool wasLast = (data_.begin() == tail_);
  T value = std::move(data_.front());
  data_.pop_front();

  if (wasLast)
  {
    tail_ = data_.beforeBegin();
  }

  return value;
}
  private:
    List< T > data_;
    Iterator< T > tail_;
  };
}

#endif