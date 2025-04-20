#ifndef SOLUTION4_HPP
#define SOLUTION4_HPP

namespace Quiz {

    class BinarySearchTree
    {
    public:
        ~BinarySearchTree();

        bool add(int value);
        bool remove(int value);
        bool contains(int value) const;

    private:
        struct Node
        {
            int value;
            Node* left{ nullptr };
            Node* right{ nullptr };
        };

        Node* root{ nullptr };
    };

}

#endif