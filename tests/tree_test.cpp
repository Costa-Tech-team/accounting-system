#include <unistd.h>
#include <vector>
#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

struct Node
{
    Node(int n)
      : element{n}
    {}

    Node(Node &&other) noexcept
      : element{other.element},
        parent{other.parent},
        children{std::move(other.children)}
    {
        updateChildren();
    }

    Node &operator=(Node &&other) noexcept
    {
        element = other.element;
        parent = other.parent;
        children = std::move(other.children);
        updateChildren();
        return *this;
    }

    Node(const Node &other)
      : element{other.element},
        children{other.children}
    {
        updateChildren();
    }

    Node &operator=(const Node &other)
    {
        if (this == &other)
        {
            return *this;
        }
        element = other.element;
        parent = nullptr;
        children = other.children;
        updateChildren();
        return *this;
    }

    ~Node() = default;

    std::span<Node> getChildren() { return children; }

    Node *getParent() const { return parent; }

    void addChild(Node &&node)
    {
        children.push_back(std::move(node));
        updateChildren();
    }

    void updateChildren()
    {
        for (auto &child : children)
        {
            child.parent = this;
        }
    }

    int element;
    Node *parent{};
    std::vector<Node> children;
};

#include <tree.hpp>

static_assert(TreeNode<Node>);

TEST_CASE("Tree data structure")
{
    SECTION("Transversing a tree in order")
    {
        Node n1{0};
        Node n2{1};
        Node n3{2};
        Node n4{3};
        Node n5{4};
        Node n6{5};
        n4.addChild(std::move(n5)); // n1 --- n2 --- n3
        n2.addChild(std::move(n3)); //  \.    \.
        n2.addChild(std::move(n4)); //   \.    \--- n4 --- n5
        n1.addChild(std::move(n2)); //    \.
        n1.addChild(std::move(n6)); //     \--- n6
        Tree<Node> t{std::vector<Node>{n1}};

        std::vector<int> order{};
        for (auto &it : t)
        {
            order.emplace_back(it.element);
        }

        for (int i = 0; i < 6; i++)
        {
            REQUIRE(i == order[i]);
        }
    }
}