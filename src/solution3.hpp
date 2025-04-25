#ifndef SOLUTION3_HPP
#define SOLUTION3_HPP

#include <vector>
#include <optional>
#include <valarray>

namespace Quiz {

    struct alignas(sizeof(float) * 4) Vector
    {
        float x, y, z;
    };

    float GetSquareDistance(const Vector& a, const Vector& b);

    float GetDistance(const Vector& a, const Vector& b);

    std::optional<Vector> GetClosestVector(const Vector& from, const std::vector< Vector >& vectors);

}

#endif