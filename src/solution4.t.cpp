#include <gtest/gtest.h>
#include <solution4.hpp>

#include <list>

namespace Quiz {

    TEST(solution4, testContains) {
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

    TEST(solution4, testDesctructor) {
        std::unique_ptr<BinarySearchTree> searchTree = std::make_unique<BinarySearchTree>();
        searchTree->add(4);
        searchTree->add(1);
        searchTree->add(2);
        searchTree->add(8);
        searchTree->add(5);
        searchTree->add(12);

        searchTree.reset();
    }

    TEST(solution4, testAdd) {
        {
            BinarySearchTree searchTree;
            EXPECT_TRUE(searchTree.add(4));
            EXPECT_FALSE(searchTree.add(4));
        }
    }

    TEST(solution4, testRemove) {
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

    std::vector<int> createRandomValues(
        std::size_t count,
        std::size_t min,
        std::size_t max
    ) {
        std::vector<int> result;
        result.reserve(count);

        srand(time(0));

        auto range = max - min;
        assert(range > 0);

        for (std::size_t i = 0; i < count; ++i) {
            result.push_back((rand() % range) + min);
        }

        return result;
    }

    TEST(solution4, testStress) {
        BinarySearchTree searchTree;
        
        const std::vector<int> values = createRandomValues(100, 0, 500);

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

        {
            std::set<int> removed;

            for (int value : values) { 

                bool result = searchTree.remove(value);

                auto it = removed.find(value);
                ASSERT_EQ(result, it == removed.end());

                removed.insert(value);

                for (int containsValue : values) {
                    auto itRemoved = removed.find(containsValue);
                    if (itRemoved != removed.end()) {
                        bool result = searchTree.contains(containsValue);
                        ASSERT_FALSE(result);
                    }
                    else {
                        ASSERT_TRUE( searchTree.contains(containsValue));
                    }
                }
            }
        }
    }

}