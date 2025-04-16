#include <gtest/gtest.h>
#include <list>
#include <optional>
#include <algorithm>
#include <math.h>

struct Vector
{
    float x, y, z;
};


float GetSquareDistance(const Vector& a, const Vector& b) {
    float result = 0.0f;
    result += (a.x - b.x) * (a.x - b.x);
    result += (a.y - b.y) * (a.y - b.y);
    result += (a.z - b.z) * (a.z - b.z);
    return result;
    
}

float GetDistance(const Vector& a, const Vector& b)
{
    return sqrt(GetSquareDistance(a, b));
}

std::optional<Vector> GetClosestVector(Vector from, std::list< Vector > vectors)
{
    if (vectors.empty()) {
        return std::optional<Vector>();
    }

    auto it = std::min_element(vectors.begin(), vectors.end(), [&from](
        const Vector& v1, const Vector& v2
    ) { return GetSquareDistance(from, v1) < GetSquareDistance(from, v2); });

    return it == end(vectors) ? std::optional<Vector>() : *it;
}


TEST(question3, testGetDistance) {
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

TEST(question3, testGetClosestVector) {
    {
        Vector from{ .x = 1.0f, .y = 2.0f, .z = 3.0f };
        std::list<Vector> vectors;

        auto result = GetClosestVector(from, vectors);
        EXPECT_FALSE(result.has_value());
    }
}