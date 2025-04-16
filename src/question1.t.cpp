#include <gtest/gtest.h>
#include <list>
#include <exception>

typedef std::list< float >::const_iterator const_floatiter;

float GetSum(const std::list< float >& numbers)
{
    float sum = 0.0f;
    for (const_floatiter it = numbers.begin(); it != numbers.end(); ++it) {
        sum += *it;
    }

    return sum;
}

struct IdNotFound : public std::exception {};

typedef std::map< int, std::string >::const_iterator const_nameiter;

std::string GetName(const std::map< int, std::string >& names, int id)
{
    const_nameiter it = names.find(id);
    if (it == names.end()) {
        throw IdNotFound();
    }

    return it->second;
}

TEST(question1, testGetSum) {
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
}

TEST(question1, testGetNames) {
    std::map< int, std::string > names;

    {
        EXPECT_THROW(GetName(names, 0), IdNotFound);
        EXPECT_THROW(GetName(names, 10), IdNotFound);
    }

    names[0] = "zero";
    names[1] = "one";

    {
        std::string result;
        EXPECT_THROW(GetName(names, -1), IdNotFound);
        EXPECT_THROW(GetName(names, 10), IdNotFound);

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