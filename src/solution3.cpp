#include <solution3.hpp>

#include <algorithm>
#include <math.h>

namespace Quiz {

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

}