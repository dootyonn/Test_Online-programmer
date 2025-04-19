#include <gtest/gtest.h>
#include <solution2.hpp>

namespace Quiz {
namespace Tests {
    
    TEST(solution2, TestCreatePlayer) {
        PlayerManager playerManager;

        EXPECT_EQ(playerManager.GetNumPlayers(), 0);

        Player* player1 = nullptr;
        Player* player3 = nullptr;

        {
            const int id = 1;

            auto* player = playerManager.GetPlayerById(id);
            EXPECT_EQ(player, nullptr);
        }

        {
            const std::string name("Player1");
            const int id = 1;
            player1 = playerManager.CreatePlayer(name.c_str(), id);

            EXPECT_TRUE(player1 != nullptr);
            EXPECT_TRUE(::strncmp(name.c_str(), player1->name, MAX_NAME_LENGTH) == 0);
            EXPECT_EQ(player1->id, id);

            EXPECT_EQ(player1, playerManager.GetPlayerById(player1->id));
            EXPECT_EQ(playerManager.GetNumPlayers(), 1);
        }

        {
            const std::string name("Player2");
            const int id = 1;
            EXPECT_THROW(playerManager.CreatePlayer(name.c_str(), id), PlayerManager::PlayerAlreadyExistException);

            EXPECT_EQ(playerManager.GetNumPlayers(), 1);
        }

        {
            const std::string name("Player3");
            const int id = 3;
            player3 = playerManager.CreatePlayer(name.c_str(), id);

            EXPECT_TRUE(player3 != nullptr);
            EXPECT_TRUE(::strncmp(name.c_str(), player3->name, MAX_NAME_LENGTH) == 0);
            EXPECT_EQ(player3->id, id);

            EXPECT_EQ(player3, playerManager.GetPlayerById(player3->id));
            EXPECT_EQ(playerManager.GetNumPlayers(), 2);
        }

        {
            const std::string name("Player3");
            const int id = 33; // Hash Collision
            auto* player = playerManager.CreatePlayer(name.c_str(), id);

            EXPECT_TRUE(player != nullptr);
            EXPECT_TRUE(::strncmp(name.c_str(), player->name, MAX_NAME_LENGTH) == 0);
            EXPECT_EQ(player->id, id);

            EXPECT_EQ(player, playerManager.GetPlayerById(player->id));
            EXPECT_EQ(playerManager.GetNumPlayers(), 3);

            // playerPtr shouldn't be invalidated
            EXPECT_EQ(player1->id, 1);
            EXPECT_EQ(player3->id, 3);
        }
    }

    TEST(solution2, TestDestroyAPlayer) {
        PlayerManager playerManager;

        EXPECT_EQ(playerManager.GetNumPlayers(), 0);

        {
            constexpr int invalidId = -1;
            EXPECT_THROW(playerManager.DestroyPlayerById(invalidId), PlayerManager::InvalidPlayerException);
        }

        {
            auto playerPtr = std::make_unique<Player>();
            playerPtr->id = 1;

            EXPECT_THROW(playerManager.DestroyPlayer(playerPtr.get()), PlayerManager::InvalidPlayerException);
        }

        {
            const std::string name("Player1");
            const int id = 1;
            auto* player = playerManager.CreatePlayer(name.c_str(), id);

            playerManager.DestroyPlayer(player);

            EXPECT_EQ(playerManager.GetPlayerById(id), nullptr);
        }

        {
            EXPECT_THROW(playerManager.DestroyPlayer(nullptr), PlayerManager::InvalidPlayerException);
            
            constexpr int invalidId = -1;
            EXPECT_THROW(playerManager.DestroyPlayerById(invalidId), PlayerManager::InvalidPlayerException);
        }

        {
            const std::string name("Player1");
            const int id = 1;
            auto* player = playerManager.CreatePlayer(name.c_str(), id);

            playerManager.DestroyPlayerById(id);

            EXPECT_EQ(playerManager.GetPlayerById(id), nullptr);
        }

        {
            const std::string name("Player1");
            const int id = 1;
            auto* player = playerManager.CreatePlayer(name.c_str(), id);

            auto playerPtr = std::make_unique<Player>();
            playerPtr->id = id;
            
            EXPECT_THROW(playerManager.DestroyPlayer(playerPtr.get()), PlayerManager::InvalidPlayerException);
        }

        {
            const int id1 = 2;
            const int id2 = 3;
            {
                const std::string name("Player2");
                auto* player = playerManager.CreatePlayer(name.c_str(), id1);
            }
            {
                const std::string name("Player3");
                auto* player = playerManager.CreatePlayer(name.c_str(), id2);
            }

            playerManager.DestroyAllPlayers();

            EXPECT_EQ(playerManager.GetPlayerById(id1), nullptr);
            EXPECT_EQ(playerManager.GetPlayerById(id2), nullptr);
        }
    }

    std::vector<int> createRandomValues(
        std::size_t count,
        std::size_t min,
        std::size_t max
    ) {
        std::vector<int> result;
        result.reserve(count);

        srand(time(0));

        auto range = max - min;
        assert(range > 0);

        for (std::size_t i = 0; i < count; ++i) {
            result.push_back((rand() % range) + min);
        }

        return result;
    }

    bool oneOverFive() {
        return (rand() % 5) == 0;
    }

    TEST(solution2, TestStressPlayerCreation) {
        constexpr std::size_t COUNT = 1000;
        constexpr std::size_t MIN_VALUE = 1000;
        constexpr std::size_t MAX_VALUE = 10000;

        std::vector<int> values = createRandomValues(COUNT, MIN_VALUE, MAX_VALUE);
        PlayerManager playerManager;

        std::vector<int> toDelete;
        toDelete.reserve(COUNT / 4);

        std::size_t count = 0;
        for (int value : values) {
            auto* playerExist = playerManager.GetPlayerById(value);

            if (!playerExist) {
                if (oneOverFive) {
                    toDelete.push_back(value);
                }

                std::string name = std::format("Player{}", value);
                auto* player = playerManager.CreatePlayer(name.c_str(), value);

                EXPECT_TRUE(player != nullptr);
                EXPECT_STREQ(player->name, name.c_str());

                auto* other = playerManager.GetPlayerById(player->id);
                EXPECT_EQ(player, other);

                ++count;

                EXPECT_EQ(playerManager.GetNumPlayers(), count);
            }
        }

        EXPECT_EQ(playerManager.GetNumPlayers(), count);

        std::size_t reverseCount = count;
        for (int value : toDelete) {
            {
                auto* player = playerManager.GetPlayerById(value);
                EXPECT_TRUE(player != nullptr && player->id == value);
            }

            playerManager.DestroyPlayerById(value);
            
            --reverseCount;
            EXPECT_EQ(playerManager.GetNumPlayers(), reverseCount);
        }

        playerManager.DestroyAllPlayers();
        EXPECT_EQ(playerManager.GetNumPlayers(), 0);
    }

}
}