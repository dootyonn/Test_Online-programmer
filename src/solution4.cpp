/*********************************************************************************************************************
 * 
 *  4. Implement the three functions in BinarySearchTree
 * 
 *  Notes:
 *      * My implementation is not auto balancing binary tree when adding/removing elements, the question only 
 *        require a binary tree which may not balanced when searching an element
 *      * I've used templated functions so that I don't need to declare them as friend in the header file
 * 
 * 
 * 
 * *******************************************************************************************************************/

#include <solution4.hpp>

#include <functional>
#include <print>
#include <set>
#include <list>
#include <vector>
#include <concepts>

#include <cstdlib>


namespace Quiz {

    template <typename TNode>
    using TypePtr = TNode*;

    template<typename TNode>
    concept IsNode = requires (TNode n) {
        n.value;
        n.left;
        n.right;
        requires std::same_as<int, decltype(n.value)>;
        requires std::same_as<TNode*, decltype(n.left)>;
        requires std::same_as<TNode*, decltype(n.right)>;
    };

    template <typename T>
    using VisitAction = std::function<bool(T* current)>;

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

    template<typename TNode>
    requires IsNode<TNode>
    bool containsValue(TNode* node, int value) {
        if (!node) {
            return false;
        }

        if (value < node->value) {
            return containsValue(node->left, value);
        }
        else if (value > node->value) {
            return containsValue(node->right, value);
        }
        else {
            return true;
        }
    }

    template<typename TNode>
    requires IsNode<TNode>
    void destroy(TypePtr<TNode>& node) {
        if (node) {
            delete node;
            node = nullptr;
        }
    }

    template<typename TNode>
    requires IsNode<TNode>
    TNode* createLeafNode(int value) {
        return new TNode{ .value = value };
    }

    template<typename TNode>
    requires IsNode<TNode>
    bool addNode(TNode*& node, int value) {
        if (!node) {
            node = createLeafNode<TNode>(value);
            return true;
        }

        if (value < node->value) {
            if (!node->left) {
                node->left = createLeafNode<TNode>(value);
                return true;
            }
            else {
                return addNode(node->left, value);
            }
        }
        else if (value > node->value) {
            if (!node->right) {
                node->right = createLeafNode<TNode>(value);
                return true;
            }
            else {
                return addNode(node->right, value);
            }
        }
        else {
            return false;
        }
    }

    template<typename TNode>
    requires IsNode<TNode>
    bool removeNode(TypePtr<TNode>& node, int value) {
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
                // leftmost element from the rigth tree
                while (lowest && lowest->left) {
                    lowest = lowest->left;
                }

                if (lowest) {
                    node->value = lowest->value;
                    //remove lowest node from the right tree
                    return removeNode(node->right, lowest->value); 
                }
                return false;
            }
        }
    }


    BinarySearchTree::~BinarySearchTree() {
        postOrderVisit<Node>(this->root, [](Node* node) { 
            destroy<Node>(node);
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
        return containsValue<Node>(this->root, value);
    }

}