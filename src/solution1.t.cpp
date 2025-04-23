#include <gtest/gtest.h>
#include <solution1.hpp>

using namespace ::testing;

namespace Quiz {
namespace Tests {

    TEST(solution1, testGetSum) {
        std::list<float> numbers;
        {
            float result = GetSum(numbers);
            ASSERT_FLOAT_EQ(0.0f, result);
        }

        numbers.push_back(0.1f);
        numbers.push_back(0.2f);
        numbers.push_back(0.3f);

        {
            float result = GetSum(numbers);
            ASSERT_FLOAT_EQ(0.6f, result);
        }
    };

    TEST(solution1, testGetNames) {
        std::map< int, std::string > names;

        {
            EXPECT_EQ(GetName(names, 0), "");
            EXPECT_EQ(GetName(names, 10), "");
        }

        names[0] = "zero";
        names[1] = "one";

        {
            std::string result;
            EXPECT_EQ(GetName(names, -1), "");
            EXPECT_EQ(GetName(names, 10), "");

            result = GetName(names, 0);
            EXPECT_EQ(result, "zero");

            result = GetName(names, 1);
            EXPECT_EQ(result, "one");
        }

        names[1] = "other one";

        {
            std::string result = GetName(names, 1);
            EXPECT_EQ(result, "other one");
        }
    }

}
}