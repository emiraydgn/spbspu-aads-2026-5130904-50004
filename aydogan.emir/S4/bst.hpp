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

      Iterator& operator++()
      {
        node_ = tree_->nextNode(node_);
        return *this;
      }

      Iterator operator++(int)
      {
        Iterator old(*this);
        ++(*this);
        return old;
      }

      Iterator& operator--()
      {
        node_ = tree_->previousNode(node_);
        return *this;
      }

      Iterator operator--(int)
      {
        Iterator old(*this);
        --(*this);
        return old;
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

      ConstIterator& operator++()
      {
        node_ = tree_->nextNode(node_);
        return *this;
      }

      ConstIterator operator++(int)
      {
        ConstIterator old(*this);
        ++(*this);
        return old;
      }

      ConstIterator& operator--()
      {
        node_ = tree_->previousNode(node_);
        return *this;
      }

      ConstIterator operator--(int)
      {
        ConstIterator old(*this);
        --(*this);
        return old;
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

    BSTree(const BSTree& other):
      root_(fakeLeaf()),
      size_(0),
      compare_(other.compare_)
    {
      for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
      {
        push(it->first, it->second);
      }
    }

    BSTree(BSTree&& other) noexcept:
      root_(fakeLeaf()),
      size_(0),
      compare_(other.compare_)
    {
      swap(other);
    }

    ~BSTree()
    {
      clear();
    }

    BSTree& operator=(const BSTree& other)
    {
      if (this != &other)
      {
        BSTree copy(other);
        swap(copy);
      }
      return *this;
    }

    BSTree& operator=(BSTree&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        swap(other);
      }
      return *this;
    }

    void swap(BSTree& other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
      std::swap(compare_, other.compare_);
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

    void push(const Key& key, const Value& value)
    {
      if (root_->fake)
      {
        root_ = new Node(key, value, nullptr, fakeLeaf());
        ++size_;
        return;
      }

      Node* current = root_;
      Node* parent = nullptr;

      while (!current->fake)
      {
        parent = current;

        if (compare_(key, current->data.first))
        {
          current = current->left;
        }
        else if (compare_(current->data.first, key))
        {
          current = current->right;
        }
        else
        {
          current->data.second = value;
          return;
        }
      }

      Node* created = new Node(key, value, parent, fakeLeaf());

      if (compare_(key, parent->data.first))
      {
        parent->left = created;
      }
      else
      {
        parent->right = created;
      }

      ++size_;
    }

    Value& get(const Key& key)
    {
      Node* node = findNode(key);

      if (node->fake)
      {
        throw std::out_of_range("Key not found");
      }

      return node->data.second;
    }

    const Value& get(const Key& key) const
    {
      Node* node = findNode(key);

      if (node->fake)
      {
        throw std::out_of_range("Key not found");
      }

      return node->data.second;
    }

    Value drop(const Key& key)
    {
      Node* node = findNode(key);

      if (node->fake)
      {
        throw std::out_of_range("Key not found");
      }

      Value value = node->data.second;
      eraseNode(node);
      --size_;
      return value;
    }

    iterator find(const Key& key)
    {
      Node* node = findNode(key);
      return iterator(node, this);
    }

    const_iterator find(const Key& key) const
    {
      Node* node = findNode(key);
      return const_iterator(node, this);
    }

    iterator begin()
    {
      return iterator(leftmost(root_), this);
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
      return const_iterator(leftmost(root_), this);
    }

    const_iterator cend() const
    {
      return const_iterator(fakeLeaf(), this);
    }

    const_iterator rotateLeft(const_iterator it)
    {
      Node* child = it.node_;

      if (child == nullptr || child->fake || child->parent == nullptr)
      {
        throw std::logic_error("Invalid rotation");
      }

      Node* parent = child->parent;

      if (parent->right != child)
      {
        throw std::logic_error("Invalid left rotation");
      }

      Node* grandParent = parent->parent;
      parent->right = child->left;

      if (!child->left->fake)
      {
        child->left->parent = parent;
      }

      child->left = parent;
      parent->parent = child;
      child->parent = grandParent;

      replaceParentChild(grandParent, parent, child);

      return const_iterator(child, this);
    }

    const_iterator rotateRight(const_iterator it)
    {
      Node* child = it.node_;

      if (child == nullptr || child->fake || child->parent == nullptr)
      {
        throw std::logic_error("Invalid rotation");
      }

      Node* parent = child->parent;

      if (parent->left != child)
      {
        throw std::logic_error("Invalid right rotation");
      }

      Node* grandParent = parent->parent;
      parent->left = child->right;

      if (!child->right->fake)
      {
        child->right->parent = parent;
      }

      child->right = parent;
      parent->parent = child;
      child->parent = grandParent;

      replaceParentChild(grandParent, parent, child);

      return const_iterator(child, this);
    }

    const_iterator rotateLargeLeft(const_iterator it)
    {
      const_iterator middle = rotateRight(it);
      return rotateLeft(middle);
    }

    const_iterator rotateLargeRight(const_iterator it)
    {
      const_iterator middle = rotateLeft(it);
      return rotateRight(middle);
    }

    std::size_t height() const
    {
      return height(root_);
    }

    std::size_t height(const_iterator it) const
    {
      if (it.node_ == nullptr || it.node_->fake)
      {
        return 0;
      }

      return height(it.node_);
    }

  private:
    static Node* fakeLeaf()
    {
      return &fakeLeaf_;
    }

    Node* findNode(const Key& key) const
    {
      Node* current = root_;

      while (!current->fake)
      {
        if (compare_(key, current->data.first))
        {
          current = current->left;
        }
        else if (compare_(current->data.first, key))
        {
          current = current->right;
        }
        else
        {
          return current;
        }
      }

      return fakeLeaf();
    }

    Node* leftmost(Node* node) const
    {
      if (node == nullptr || node->fake)
      {
        return fakeLeaf();
      }

      while (!node->left->fake)
      {
        node = node->left;
      }

      return node;
    }

    Node* rightmost(Node* node) const
    {
      if (node == nullptr || node->fake)
      {
        return fakeLeaf();
      }

      while (!node->right->fake)
      {
        node = node->right;
      }

      return node;
    }

    Node* nextNode(Node* node) const
    {
      if (node == nullptr || node->fake)
      {
        return fakeLeaf();
      }

      if (!node->right->fake)
      {
        return leftmost(node->right);
      }

      Node* parent = node->parent;

      while (parent != nullptr && parent->right == node)
      {
        node = parent;
        parent = parent->parent;
      }

      return parent == nullptr ? fakeLeaf() : parent;
    }

    Node* previousNode(Node* node) const
    {
      if (node == nullptr)
      {
        return fakeLeaf();
      }

      if (node->fake)
      {
        return rightmost(root_);
      }

      if (!node->left->fake)
      {
        return rightmost(node->left);
      }

      Node* parent = node->parent;

      while (parent != nullptr && parent->left == node)
      {
        node = parent;
        parent = parent->parent;
      }

      return parent == nullptr ? fakeLeaf() : parent;
    }

    void replaceParentChild(Node* parent, Node* oldChild, Node* newChild)
    {
      if (parent == nullptr)
      {
        root_ = newChild;
        newChild->parent = nullptr;
      }
      else if (parent->left == oldChild)
      {
        parent->left = newChild;
      }
      else
      {
        parent->right = newChild;
      }
    }

    void replaceNode(Node* oldNode, Node* newNode)
    {
      if (oldNode->parent == nullptr)
      {
        root_ = newNode;

        if (!newNode->fake)
        {
          newNode->parent = nullptr;
        }
      }
      else if (oldNode->parent->left == oldNode)
      {
        oldNode->parent->left = newNode;

        if (!newNode->fake)
        {
          newNode->parent = oldNode->parent;
        }
      }
      else
      {
        oldNode->parent->right = newNode;

        if (!newNode->fake)
        {
          newNode->parent = oldNode->parent;
        }
      }
    }

    void eraseNode(Node* node)
    {
      if (node->left->fake && node->right->fake)
      {
        replaceNode(node, fakeLeaf());
        delete node;
      }
      else if (node->left->fake)
      {
        replaceNode(node, node->right);
        delete node;
      }
      else if (node->right->fake)
      {
        replaceNode(node, node->left);
        delete node;
      }
      else
      {
        Node* successor = leftmost(node->right);
        node->data = successor->data;
        eraseNode(successor);
      }
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

    std::size_t height(Node* node) const
    {
      if (node == nullptr || node->fake)
      {
        return 0;
      }

      std::size_t leftHeight = height(node->left);
      std::size_t rightHeight = height(node->right);

      return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
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
