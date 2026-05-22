#ifndef BST_HPP
#define BST_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace aydogan
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  private:
    struct Node
    {
      std::pair< Key, Value > data;
      Node* left;
      Node* right;
      Node* parent;
      bool fake;

      Node():
        data(),
        left(this),
        right(this),
        parent(nullptr),
        fake(true)
      {}

      Node(const Key& key, const Value& value, Node* parentNode, Node* fakeLeaf):
        data(key, value),
        left(fakeLeaf),
        right(fakeLeaf),
        parent(parentNode),
        fake(false)
      {}
    };

  public:
    class ConstIterator;

    class Iterator
    {
    public:
      Iterator():
        node_(nullptr),
        tree_(nullptr)
      {}

      std::pair< Key, Value >& operator*() const
      {
        return node_->data;
      }

      std::pair< Key, Value >* operator->() const
      {
        return &(node_->data);
      }

      bool operator==(const Iterator& other) const
      {
        return node_ == other.node_ && tree_ == other.tree_;
      }

      bool operator!=(const Iterator& other) const
      {
        return !(*this == other);
      }

    private:
      friend class BSTree;
      friend class ConstIterator;

      Iterator(Node* node, BSTree* tree):
        node_(node),
        tree_(tree)
      {}

      Node* node_;
      BSTree* tree_;
    };

    class ConstIterator
    {
    public:
      ConstIterator():
        node_(nullptr),
        tree_(nullptr)
      {}

      ConstIterator(const Iterator& other):
        node_(other.node_),
        tree_(other.tree_)
      {}

      const std::pair< Key, Value >& operator*() const
      {
        return node_->data;
      }

      const std::pair< Key, Value >* operator->() const
      {
        return &(node_->data);
      }

      bool operator==(const ConstIterator& other) const
      {
        return node_ == other.node_ && tree_ == other.tree_;
      }

      bool operator!=(const ConstIterator& other) const
      {
        return !(*this == other);
      }

    private:
      friend class BSTree;

      ConstIterator(Node* node, const BSTree* tree):
        node_(node),
        tree_(tree)
      {}

      Node* node_;
      const BSTree* tree_;
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    BSTree():
      root_(fakeLeaf()),
      size_(0),
      compare_()
    {}

    ~BSTree()
    {
      clear();
    }

    bool isEmpty() const noexcept
    {
      return size_ == 0;
    }

    bool empty() const noexcept
    {
      return isEmpty();
    }

    std::size_t getSize() const noexcept
    {
      return size_;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    void clear()
    {
      clearNode(root_);
      root_ = fakeLeaf();
      size_ = 0;
    }

    iterator begin()
    {
      return iterator(fakeLeaf(), this);
    }

    iterator end()
    {
      return iterator(fakeLeaf(), this);
    }

    const_iterator begin() const
    {
      return cbegin();
    }

    const_iterator end() const
    {
      return cend();
    }

    const_iterator cbegin() const
    {
      return const_iterator(fakeLeaf(), this);
    }

    const_iterator cend() const
    {
      return const_iterator(fakeLeaf(), this);
    }

  private:
    static Node* fakeLeaf()
    {
      return &fakeLeaf_;
    }

    void clearNode(Node* node)
    {
      if (node == nullptr || node->fake)
      {
        return;
      }

      clearNode(node->left);
      clearNode(node->right);
      delete node;
    }

    Node* root_;
    std::size_t size_;
    Compare compare_;

    static Node fakeLeaf_;
  };

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node BSTree< Key, Value, Compare >::fakeLeaf_;
}

#endif
