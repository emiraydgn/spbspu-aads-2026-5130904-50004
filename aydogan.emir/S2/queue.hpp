#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "list.hpp"
#include <stdexcept>
#include <utility>

namespace aydogan
{
  template< class T >
  class Queue
  {
  public:
    Queue():
      data_(),
      tail_(data_.beforeBegin())
    {}

    Queue(const Queue& other):
      data_(other.data_),
      tail_(data_.beforeBegin())
    {
      rebuildTail();
    }

    Queue(Queue&& other):
      data_(std::move(other.data_)),
      tail_(data_.beforeBegin())
    {
      rebuildTail();
      other.tail_ = other.data_.beforeBegin();
    }

    Queue& operator=(const Queue& other)
    {
      if (this != &other)
      {
        data_ = other.data_;
        rebuildTail();
      }
      return *this;
    }

    Queue& operator=(Queue&& other)
    {
      if (this != &other)
      {
        data_ = std::move(other.data_);
        rebuildTail();
        other.tail_ = other.data_.beforeBegin();
      }
      return *this;
    }

    ~Queue() = default;

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
    void rebuildTail()
    {
      tail_ = data_.beforeBegin();
      for (auto it = data_.begin(); it != data_.end(); ++it)
      {
        tail_ = it;
      }
    }

    List< T > data_;
    Iterator< T > tail_;
  };
}

#endif
