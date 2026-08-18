#include <concepts>
#include <iterator>
#include <memory>
#include <span>
#include <vector>

#pragma once

/// @brief this concept declares the requirements for a node type. The tree
/// class requires these methods to work with any node type.
///
/// Move and copy constructors and assignments should update the parent member
/// pointer of every children, andadding a child in a node should update the
/// parent member pointer of every grandchildren to its corresponding father.
/// This way, dangling pointers are avoided. The template also requires NodeType
/// to have a property parent of type NodeType *.
template<class NodeType>
concept TreeNode = requires(NodeType *n, NodeType &&i) {
    { n->getParent() } -> std::same_as<NodeType *>;

    { n->getChildren() } -> std::same_as<std::span<NodeType>>;
    { n->addChild(std::move(i)) };
};

/// @brief Tree data structure that provides const and non const bidirectional
/// iterators.
///
/// Works like a forest, storing from zero to many nodes in the top
/// level. Every node can have from one to many chidren, and just one parent.
/// It's movable only.
template<TreeNode NodeType>
class Tree
{
    template<bool IsConst>
    class basic_bidirectional_iterator
    {
      public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type =
            std::conditional_t<IsConst, const NodeType, NodeType>;
        using pointer = value_type *;
        using reference = value_type &;

        basic_bidirectional_iterator(pointer element);
        basic_bidirectional_iterator();
        reference operator*() const;
        pointer operator->() const;
        basic_bidirectional_iterator &operator++();
        basic_bidirectional_iterator &operator--();
        basic_bidirectional_iterator operator++(int);
        basic_bidirectional_iterator operator--(int);
        bool operator==(const basic_bidirectional_iterator &) const = default;

      private:
        pointer current;
    };

  public:
    Tree(std::vector<NodeType> &&topLevelNodes);

    Tree(const Tree &other) = default;
    Tree(Tree &&other) noexcept = default;

    Tree &operator=(const Tree &other) = delete;
    Tree &operator=(Tree &&other) noexcept = delete;

    ~Tree() = default;

    /// A bidirectional iterator to the accounts in the chart. The order
    /// in each level of the tree is like this: element -> children (if present)
    /// -> next element. This order repeats every time the iterator steps on the
    /// children of a node.
    using bidirectional_iterator = basic_bidirectional_iterator<false>;

    /// A const version of the iterator.
    using const_bidirectional_iterator = basic_bidirectional_iterator<true>;

    static_assert(std::bidirectional_iterator<bidirectional_iterator>);

    /// @return a view of the orphan nodes of the underlying container.
    std::span<NodeType> getTopLevelNodes();

    /// @return a const view of the orphan nodes of the underlying container.
    std::span<const NodeType> getTopLevelNodes() const;

    /// @return a iterator to first node.
    bidirectional_iterator begin();
    /// \overload
    const_bidirectional_iterator begin() const;
    /// @return a constant iterator to the first node.
    const_bidirectional_iterator cbegin() const;
    /// @return a iterator to the end of the node container.
    bidirectional_iterator end();
    /// \overload
    const_bidirectional_iterator end() const;
    /// @return a constant iterator to the end of the node container.
    const_bidirectional_iterator cend() const;

  private:
    std::vector<NodeType> topLevelNodes;
};

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<
    IsConst>::basic_bidirectional_iterator(pointer element)
  : current(element)
{}

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<
    IsConst>::basic_bidirectional_iterator()
  : current(nullptr)
{}

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::reference
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator*() const
{
    return *current;
};

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::pointer
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator->() const
{
    return current;
};

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst> &
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator++()
{
    std::span childrenView = current->getChildren();
    if (!childrenView.empty())
    {
        current = childrenView.data();
        return *this;
    }
    auto parent = current->getParent();
    while (true)
    {
        if (parent && (current == &(parent->getChildren().back())))
        {
            current = parent;
            parent = parent->getParent();
        } else
        {
            current = std::next(current);
            return *this;
        }
    }
};

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst> &
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator--()
{
    std::span childrenView = current->getChildren();
    if (!childrenView.empty())
    {
        current = std::to_address(childrenView.end());
        return *this;
    }
    auto parent = current->getParent();
    while (true)
    {
        if (parent && (current == &(parent->getChildren().front())))
        {
            current = parent;
            parent = parent->getParent();
        } else
        {
            current = std::prev(current);
            return *this;
        }
    }
};

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<TreeNode NodeType>
template<bool IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>
Tree<NodeType>::basic_bidirectional_iterator<IsConst>::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

template<TreeNode NodeType>
Tree<NodeType>::Tree(std::vector<NodeType> &&topLevelNodes)
  : topLevelNodes(std::move(topLevelNodes))
{}

template<TreeNode NodeType>
std::span<NodeType> Tree<NodeType>::getTopLevelNodes()
{
    return topLevelNodes;
}

template<TreeNode NodeType>
std::span<const NodeType> Tree<NodeType>::getTopLevelNodes() const
{
    return topLevelNodes;
}

template<TreeNode NodeType>
Tree<NodeType>::bidirectional_iterator Tree<NodeType>::begin()
{
    return std::to_address(topLevelNodes.begin());
}

template<TreeNode NodeType>
Tree<NodeType>::const_bidirectional_iterator Tree<NodeType>::begin() const
{
    return std::to_address(topLevelNodes.begin());
}

template<TreeNode NodeType>
Tree<NodeType>::const_bidirectional_iterator Tree<NodeType>::cbegin() const
{
    return begin();
}

template<TreeNode NodeType>
Tree<NodeType>::bidirectional_iterator Tree<NodeType>::end()
{
    return std::to_address(topLevelNodes.end());
}

template<TreeNode NodeType>
Tree<NodeType>::const_bidirectional_iterator Tree<NodeType>::end() const
{
    return std::to_address(topLevelNodes.end());
}

template<TreeNode NodeType>
Tree<NodeType>::const_bidirectional_iterator Tree<NodeType>::cend() const
{
    return end();
}
