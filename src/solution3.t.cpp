#include <gtest/gtest.h>
#include <solution3.hpp>

namespace Quiz {
namespace Tests {

    TEST(solution3, testGetDistance) {
        {
            Vector v1{ .x = 1.0f, .y = 2.0f, .z = 3.0f };
            Vector v2{ .x = 1.0f, .y = 2.0f, .z = 3.0f };

            float result = GetDistance(v1, v2);

            EXPECT_FLOAT_EQ(0.0f, result);
        }

        {
            Vector v1{ .x = 1.0f, .y = 2.0f, .z = 3.0f };
            Vector v2{ .x = 2.0f, .y = 4.0f, .z = 5.0f };

            float result = GetDistance(v1, v2);

            EXPECT_FLOAT_EQ(3.0f, result);
        }

        {
            Vector v1{ .x = 0.0f, .y = -1.0f, .z = -3.0f };
            Vector v2{ .x = -1.0f, .y = 1.0f, .z = -1.0f };

            float result = GetDistance(v1, v2);

            EXPECT_FLOAT_EQ(3.0f, result);
        }
    }

    TEST(solution3, testGetClosestVector) {
        {
            Vector from{ .x = 1.0f, .y = 2.0f, .z = 3.0f };
            std::vector<Vector> vectors;

            auto result = GetClosestVector(from, vectors);
            EXPECT_FALSE(result.has_value());
        }
    }

}
}