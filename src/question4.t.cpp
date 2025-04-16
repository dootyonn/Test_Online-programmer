#include <gtest/gtest.h>

#include <set>
#include <list>
#include <vector>
#include <print>
#include <cstdlib>

class BinarySearchTree
{
public:
    ~BinarySearchTree();

    bool add(int value);
    bool remove(int value);
    bool contains(int value) const;

    void print() const;

private:
    struct Node
    {
        int value;
        Node* left{ nullptr };
        Node* right{ nullptr };
    };

    Node* root{ nullptr };
};

template <typename TNode>
using TypePtr = TNode*;

template <typename T>
using VisitAction = std::function<bool(T* current)>;

// template <typename TNode>
// bool inOrderVisit(
//     TNode* node,
//     VisitAction<TNode> action
// ) {
//     if (node) {
//         if (inOrderVisit(node->left, action)) {
//             return true;
//         }

//         if (action(node)) {
//             return true;
//         }
        
//         if (inOrderVisit(node->right, action)) {
//             return true;
//         }    
//     }

//     return false;
// }

// template <typename TNode>
// bool preOrderVisit(
//     TNode* node,
//     VisitAction<TNode> action
// ) {
//     if (node) {
//         if (action(node)) {
//             return true;
//         }

//         if (preOrderVisit(node->left, action)) {
//             return true;
//         }
    
//         if (preOrderVisit(node->right, action)) {
//             return true;
//         }    
//     }

//     return false;
// }

template<typename TNode>
bool postOrderVisit(
    TNode* node,
    VisitAction<TNode> action
) {
    if (node) {
        if (postOrderVisit(node->left, action)) {
            return true;
        }
    
        if (postOrderVisit(node->right, action)) {
            return true;
        }

        if (action(node)) {
            return true;
        }
    }

    return false;
}

// void BinarySearchTree::print() const {
//     std::print("content: ");
//     preOrderVisit<Node>(this->root, [] (const Node* node){
//         std::print("{} ", node->value);
//         return false;
//     });
//     std::println("");
// }


// template<typename TNode>
// std::size_t getSize(const TNode* node) {
//     size_t count = 0;
//     preOrderVisit(nullptr, node, [&count] {
//         ++count;
//         return false;
//     });

//     return count;
// }

template<typename TNode, typename TValue>
bool containsValue(TNode* node, TValue value) {
    if (!node) {
        return false;
    }

    if (node->value == value) {
        return true;
    }
    else if (value < node->value) {
        return containsValue(node->left, value);
    }
    else {
        return containsValue(node->right, value);
    }
}



template<typename TNode>
void destroy(TypePtr<TNode>& node) {
    if (node) {
        // std::println("destroy node: {}", node->value);
        delete node;
        node = nullptr;
    }
}

template<typename TNode, typename TValue>
TNode* createLeafNode(TValue value) {
    return new TNode{ .value = value };
}

template<typename TNode, typename TValue>
bool addNode(TypePtr<TNode>& node, TValue value) {
    if (!node) {
        node = createLeafNode<TNode>(value);
        return true;
    }

    if (value == node->value) {
        return false;
    }
    else if (value < node->value) {
        if (!node->left) {
            node->left = createLeafNode<TNode>(value);
            return true;
        }
        else {
            return addNode(node->left, value);
        }
    }
    else {
        if (!node->right) {
            node->right = createLeafNode<TNode>(value);
            return true;
        }
        else {
            return addNode(node->right, value);
        }
    }
}

template<typename TNode, typename TValue>
bool removeNode(TypePtr<TNode>& node, TValue value) {
    if (!node) {
        return false;
    }

    if (value < node->value) {
        return removeNode(node->left, value);
    }
    else if (value > node->value) {
        return removeNode(node->right, value);
    }
    else {
        if (!node->left) {
            auto* temp = node->right;
            destroy(node);
            node = temp;
            return true;
        }
        else if (!node->right) {
            auto* temp = node->left;
            destroy(node);
            node = temp;
            return true;
        }
        else {
            auto* lowest = node->right;
            while (lowest && lowest->left) {
                lowest = lowest->left;
            }

            node->value = lowest->value;
            return removeNode(node->right, lowest->value);
        }
    }
}


BinarySearchTree::~BinarySearchTree() {
    postOrderVisit<Node>(this->root, [](Node* node) { 
        destroy(node);
        return false;
    });
}

bool BinarySearchTree::add(int value)
{
    return addNode(this->root, value);
}

bool BinarySearchTree::remove(int value)
{
    return removeNode(this->root, value);
}

bool BinarySearchTree::contains(int value) const
{
    return containsValue<Node, int>(this->root, value);
}

TEST(question4, testContains) {
    {
        BinarySearchTree searchTree;

        EXPECT_FALSE(searchTree.contains(4));
    }

    {
        BinarySearchTree searchTree;
        searchTree.add(4);
        
        EXPECT_TRUE(searchTree.contains(4));

        searchTree.remove(4);

        EXPECT_FALSE(searchTree.contains(4));
    }

    {
        BinarySearchTree searchTree;
        searchTree.add(4);
        searchTree.add(1);
        searchTree.add(2);
        searchTree.add(8);
        searchTree.add(5);
        searchTree.add(12);
        
        EXPECT_TRUE(searchTree.contains(1));
        EXPECT_TRUE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_TRUE(searchTree.contains(5));
        EXPECT_TRUE(searchTree.contains(8));
        EXPECT_TRUE(searchTree.contains(12));

        searchTree.remove(2);
        EXPECT_TRUE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_TRUE(searchTree.contains(5));
        EXPECT_TRUE(searchTree.contains(8));
        EXPECT_TRUE(searchTree.contains(12));

        searchTree.remove(8);
        EXPECT_TRUE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_TRUE(searchTree.contains(5));
        EXPECT_FALSE(searchTree.contains(8));
        EXPECT_TRUE(searchTree.contains(12));

        searchTree.remove(1);
        EXPECT_FALSE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_TRUE(searchTree.contains(5));
        EXPECT_FALSE(searchTree.contains(8));
        EXPECT_TRUE(searchTree.contains(12));

        searchTree.remove(12);
        EXPECT_FALSE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_TRUE(searchTree.contains(5));
        EXPECT_FALSE(searchTree.contains(8));
        EXPECT_FALSE(searchTree.contains(12));

        searchTree.remove(5);
        EXPECT_FALSE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_TRUE(searchTree.contains(4));
        EXPECT_FALSE(searchTree.contains(5));
        EXPECT_FALSE(searchTree.contains(8));
        EXPECT_FALSE(searchTree.contains(12));

        searchTree.remove(4);
        EXPECT_FALSE(searchTree.contains(1));
        EXPECT_FALSE(searchTree.contains(2));
        EXPECT_FALSE(searchTree.contains(4));
        EXPECT_FALSE(searchTree.contains(5));
        EXPECT_FALSE(searchTree.contains(8));
        EXPECT_FALSE(searchTree.contains(12));
    }
}

TEST(question4, testDesctructor) {
    std::unique_ptr<BinarySearchTree> searchTree = std::make_unique<BinarySearchTree>();
    searchTree->add(4);
    searchTree->add(1);
    searchTree->add(2);
    searchTree->add(8);
    searchTree->add(5);
    searchTree->add(12);

    searchTree.reset();
}

TEST(question4, testAdd) {
    {
        BinarySearchTree searchTree;
        EXPECT_TRUE(searchTree.add(4));
        EXPECT_FALSE(searchTree.add(4));
    }
}

TEST(question4, testRemove) {
    {
        BinarySearchTree searchTree;
        EXPECT_FALSE(searchTree.remove(4));
    }

    {
        BinarySearchTree searchTree;
        searchTree.add(4);
        EXPECT_TRUE(searchTree.remove(4));
    }
}

TEST(question4, testConsistency) {
    constexpr std::size_t COUNT = 100;
    srand(time(0));

    std::list<int> values;
    for (std::size_t i = 0; i < (COUNT / 2); ++i) {
        values.push_back(rand() % COUNT); // Expect duplicates
    }

    BinarySearchTree searchTree;

    {
        std::set<int> added;

        for (int value : values) {
            bool result = searchTree.add(value);

            auto it = added.find(value);
            EXPECT_EQ(result, it == added.end());

            added.insert(value);

            for (int containsValue : values) {
                auto itAdded = added.find(containsValue);
                if (itAdded != added.end()) {
                    EXPECT_TRUE(searchTree.contains(containsValue));
                }
                else {
                    EXPECT_FALSE(searchTree.contains(containsValue));
                }
            }
        }
    }

    //searchTree.print();

    {
        std::set<int> removed;

        for (int value : values) { 
            //std::println("value: {}", value);

            bool result = searchTree.remove(value);

            auto it = removed.find(value);
            ASSERT_EQ(result, it == removed.end());

            //searchTree.print();
            removed.insert(value);

            for (int containsValue : values) {
                //std::print("containsValue: {}", containsValue);

                auto itRemoved = removed.find(containsValue);
                if (itRemoved != removed.end()) {
                    //std::println(" out");
                    bool result = searchTree.contains(containsValue);
                    ASSERT_FALSE(result);
                }
                else {
                    //std::println(" in");
                    bool result = searchTree.contains(containsValue);
                    // if (!result) {
                    //     std::println("here");
                    //     result = searchTree.contains(containsValue);
                    // }
                    ASSERT_TRUE(result);
                }
            }
        }
    }
}