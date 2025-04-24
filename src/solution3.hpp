#ifndef SOLUTION3_HPP
#define SOLUTION3_HPP

#include <list>
#include <optional>
#include <valarray>

namespace Quiz {

    struct alignas(32) Vector
    {
        float x, y, z;
    };

    float GetSquareDistance(const Vector& a, const Vector& b);

    float GetDistance(const Vector& a, const Vector& b);

    std::optional<Vector> GetClosestVector(const Vector& from, std::list< Vector > vectors);

}

#endif