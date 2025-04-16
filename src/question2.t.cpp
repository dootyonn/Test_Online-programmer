#include <gtest/gtest.h>
#include <print>

using byte = unsigned char; // might want to use std::byte instead
constexpr std::size_t bytes_in_int = sizeof(int) / sizeof(byte);

auto int_to_bytes(int value)
{
    return std::bit_cast<std::array<byte, bytes_in_int>>(value);
}


TEST(question2, test1) {
    std::size_t value = std::hash<int>{}(123456);

    std::println("hash: {}", value);

    EXPECT_TRUE(false);
}