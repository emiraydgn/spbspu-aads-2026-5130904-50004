#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"

namespace aydogan
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;

  private:
    List< T > data_;
  };
}

#endif
