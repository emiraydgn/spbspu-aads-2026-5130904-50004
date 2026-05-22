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
