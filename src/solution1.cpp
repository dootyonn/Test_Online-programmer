#include <solution1.hpp>

/*********************************************************************************************************************
 * 
 *   1. Implement the two functions without any c++11 or above features:
 * 
 * Proposed changes:
 *      * In GetSums, the list is passed by copy, it should be passed by const reference to avoid the unnecessary
 *        copy
 *      * In GetName, the map is passed by copy, it should be passed by const reference to avoid the unnecessary copy
 * 
 * Notes:
 *      * This file is compiled in a library that is configure to only use C++ 98 to match the requirement
 *      * The id might not be in the map, an empty string make sense for a name not found
 * 
 * *******************************************************************************************************************/

namespace Quiz {

    

    float GetSum(const std::vector< float >& numbers)
    {
        float sum = 0.0f;
        typedef std::vector< float >::const_iterator const_floatiter;
        for (const_floatiter it = numbers.begin(); it != numbers.end(); ++it) {
            sum += *it;
        }

        return sum;
    }

    std::string GetName(const std::map< int, std::string >& names, int id)
    {
        typedef std::map< int, std::string >::const_iterator const_nameiter;
        const_nameiter it = names.find(id);
        if (it == names.end()) {
            return "";
        }

        return it->second;
    }
}
