#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"
#include <stdexcept>
#include <utility>

namespace aydogan
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(const Stack&) = default;
    Stack& operator=(Stack&&) = default;
    ~Stack() = default;

    bool empty() const noexcept
    {
      return data_.empty();
    }

    void push(const T& rhs)
    {
      data_.push_front(rhs);
    }

    void push(T&& rhs)
    {
      data_.push_front(std::move(rhs));
    }

    T& top()
    {
      if (empty())
      {
        throw std::out_of_range("Stack is empty");
      }
      return data_.front();
    }

    const T& top() const
    {
      if (empty())
      {
        throw std::out_of_range("Stack is empty");
      }
      return data_.front();
    }

    T drop()
    {
      if (empty())
      {
        throw std::out_of_range("Stack is empty");
      }

      T value = std::move(data_.front());
      data_.pop_front();
      return value;
    }

  private:
    List< T > data_;
  };
}

#endif
