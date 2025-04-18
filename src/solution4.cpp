#include <solution4.hpp>

#include <functional>
#include <print>
#include <set>
#include <list>
#include <vector>

#include <cstdlib>


namespace Quiz {

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

}