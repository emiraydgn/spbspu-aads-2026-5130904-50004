#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <utility>

namespace aydogan
{
  template< class T >
  class List;

  template< class T >
  class Iterator;

  template< class T >
  class ConstIterator;

  namespace detail
  {
    template< class T >
    struct Node
    {
      T data;
      Node* next;

      Node():
        data(),
        next(nullptr)
      {}

      Node(const T& value, Node* nextNode):
        data(value),
        next(nextNode)
      {}

      Node(T&& value, Node* nextNode):
        data(std::move(value)),
        next(nextNode)
      {}
    };
  }

  template< class T >
  class Iterator
  {
    friend class List< T >;
    friend class ConstIterator< T >;

  public:
    Iterator():
      node_(nullptr)
    {}

    T& operator*() const
    {
      return node_->data;
    }

    T* operator->() const
    {
      return &(node_->data);
    }

    Iterator& operator++()
    {
      if (node_ != nullptr)
      {
        node_ = node_->next;
      }
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const Iterator& other) const
    {
      return node_ != other.node_;
    }

  private:
    explicit Iterator(detail::Node< T >* node):
      node_(node)
    {}

    detail::Node< T >* node_;
  };

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
