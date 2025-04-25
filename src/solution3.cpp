/*********************************************************************************************************************
 * 
 *  3. Implement the following two functions:
 * 
 *  Proposed changes:
 *      * Change order of Vector members x, z, y to make it more user-friendly when using the bracket constructor.
 *      * Pass arguments by const reference, since sizeof(Vector) > sizeof(void*).
 *      * GetClosestVector should return an Optional<Vector> to handle the case of the vector list being empty, unless
 *        the function use a contract that guaranteed that the list is not empty.
 *  Notes:
 *      * GetClosestVector use the square distance between vector to avoid to compute the square root. The square 
 *        distance is proportional to the distance.
 * 
 * *******************************************************************************************************************/

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

    std::optional<Vector> GetClosestVector(const Vector& from, const std::vector< Vector >& vectors)
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