#include <solution1.hpp>

namespace Quiz {

    typedef std::list< float >::const_iterator const_floatiter;

    float GetSum(const std::list< float >& numbers)
    {
        float sum = 0.0f;
        for (const_floatiter it = numbers.begin(); it != numbers.end(); ++it) {
            sum += *it;
        }

        return sum;
    }

    typedef std::map< int, std::string >::const_iterator const_nameiter;

    std::string GetName(const std::map< int, std::string >& names, int id)
    {
        const_nameiter it = names.find(id);
        if (it == names.end()) {
            throw IdNotFound();
        }

        return it->second;
    }
}
