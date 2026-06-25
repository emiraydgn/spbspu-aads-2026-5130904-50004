#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <utility>

#include "const_iterator.hpp"
#include "iterator.hpp"
#include "node.hpp"

namespace aydogan
{
  template< class T >
  class List
  {
    friend class Iterator< T >;
    friend class ConstIterator< T >;

  public:
    List():
      fake_(new detail::Node< T >())
    {}

    List(const List& other):
      List()
    {
      Iterator< T > tail = beforeBegin();
      for (ConstIterator< T > it = other.cbegin(); it != other.cend(); ++it)
      {
        tail = insertAfter(tail, *it);
      }
    }

    List(List&& other):
      fake_(other.fake_)
    {
      other.fake_ = new detail::Node< T >();
    }

    ~List()
    {
      clear();
      delete fake_;
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other)
    {
      if (this != &other)
      {
        clear();
        delete fake_;
        fake_ = other.fake_;
        other.fake_ = new detail::Node< T >();
      }
      return *this;
    }

    void swap(List& other) noexcept
    {
      std::swap(fake_, other.fake_);
    }

    bool empty() const noexcept
    {
      return fake_->next == nullptr;
    }

    Iterator< T > beforeBegin() noexcept
    {
      return Iterator< T >(fake_);
    }

    ConstIterator< T > beforeBegin() const noexcept
    {
      return ConstIterator< T >(fake_);
    }

    Iterator< T > begin() noexcept
    {
      return Iterator< T >(fake_->next);
    }

    ConstIterator< T > begin() const noexcept
    {
      return ConstIterator< T >(fake_->next);
    }

    ConstIterator< T > cbegin() const noexcept
    {
      return ConstIterator< T >(fake_->next);
    }

    Iterator< T > end() noexcept
    {
      return Iterator< T >(nullptr);
    }

    ConstIterator< T > end() const noexcept
    {
      return ConstIterator< T >(nullptr);
    }

    ConstIterator< T > cend() const noexcept
    {
      return ConstIterator< T >(nullptr);
    }

    T& front()
    {
      if (empty())
      {
        throw std::out_of_range("List is empty");
      }
      return fake_->next->data;
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::out_of_range("List is empty");
      }
      return fake_->next->data;
    }

    void push_front(const T& value)
    {
      insertAfter(beforeBegin(), value);
    }

    void push_front(T&& value)
    {
      insertAfter(beforeBegin(), std::move(value));
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::out_of_range("List is empty");
      }
      eraseAfter(beforeBegin());
    }

    Iterator< T > insertAfter(Iterator< T > pos, const T& value)
    {
      if (pos.node_ == nullptr)
      {
        throw std::out_of_range("Invalid iterator");
      }

      detail::Node< T >* newNode =
        new detail::Node< T >(value, pos.node_->next);

      pos.node_->next = newNode;
      return Iterator< T >(newNode);
    }

    Iterator< T > insertAfter(Iterator< T > pos, T&& value)
    {
      if (pos.node_ == nullptr)
      {
        throw std::out_of_range("Invalid iterator");
      }

      detail::Node< T >* newNode =
        new detail::Node< T >(std::move(value), pos.node_->next);

      pos.node_->next = newNode;
      return Iterator< T >(newNode);
    }

    Iterator< T > eraseAfter(Iterator< T > pos)
    {
      if (pos.node_ == nullptr || pos.node_->next == nullptr)
      {
        throw std::out_of_range("Nothing to erase");
      }

      detail::Node< T >* toDelete = pos.node_->next;
      pos.node_->next = toDelete->next;
      delete toDelete;

      return Iterator< T >(pos.node_->next);
    }

    void clear() noexcept
    {
      while (!empty())
      {
        detail::Node< T >* tmp = fake_->next;
        fake_->next = tmp->next;
        delete tmp;
      }
    }

  private:
    detail::Node< T >* fake_;
  };
}

#endif
