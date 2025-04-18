#ifndef SOLUTION1_HPP
#define SOLUTION1_HPP

#include <list>
#include <map>
#include <string>

namespace Quiz {

    struct IdNotFound : public std::exception {};

    float GetSum(const std::list< float >& numbers);
    std::string GetName(const std::map< int, std::string >& names, int id);

}

#endif