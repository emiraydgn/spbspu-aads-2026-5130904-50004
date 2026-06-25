#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <utility>

#include "../common/list.hpp"

namespace aydogan
{
  template< class T >
  class Stack
  {
  public:
    bool empty() const noexcept;
    void push(const T& rhs);
    void push(T&& rhs);
    T& top();
    const T& top() const;
    T pop();

  private:
    List< T > data_;
  };

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    data_.push_front(rhs);
  }

  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    data_.push_front(std::move(rhs));
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (empty())
    {
      throw std::out_of_range("Stack is empty");
    }
    return data_.front();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::out_of_range("Stack is empty");
    }
    return data_.front();
  }

  template< class T >
  T Stack< T >::pop()
  {
    if (empty())
    {
      throw std::out_of_range("Stack is empty");
    }

    T value = std::move(data_.front());
    data_.pop_front();
    return value;
  }
}

#endif
