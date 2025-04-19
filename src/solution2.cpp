/*********************************************************************************************************************
 * 
 *  2. Implement the functions of class PlayerManager
 * Don’t change the Player struct and don’t use any stl container for Player storage. You can add additional member variables to the PlayerManager.
 * 
 * Proposed changes:
 *      * GetNumPlayers and GetPlayerById should be const since they shouldn't modify the internal members of the PlayerManager class
 * 
 * Notes:
 *      * GetPlayerById return nullptr when provided id is not found
 *      * PlayerManager throw a PlayerAlreadyExistException if trying to add a player with an id that already exist
 *      * PlayerManager throw an InvalidPlayerException if try to delete a player that do not exist or from a Player
 *        instance not found in the manager
 *      * Use a bucket that has an player id to a player instance, the bucket will grow automatically to avoid 
 *        collision
 *      * When the bucket is growing, it will not invalidate the pointers to the player instances
 * 
 * ********************************************************************************************************************/
#include <solution2.hpp>

#include <print>
#include <vector>
#include <cstdlib>
#include <assert.h>
#include <string.h>

namespace Quiz {

    constexpr Player** AllocateBucket(std::size_t size) {
        return new Player*[size]{ nullptr };
    }

    constexpr std::size_t GetHash(int id, std::size_t bucketSize) {
        return static_cast<std::size_t>(id) % bucketSize;
    }

    typedef Player* PlayerPtr;
    constexpr void DeletePlayerRaw(PlayerPtr& playerPtr, int& numPlayer) {
        assert(playerPtr != nullptr);
        delete[] playerPtr->name;
        delete playerPtr;
        playerPtr = nullptr;
        --numPlayer;
    }

    void ReHash(
        Player**& orginalBucket,
        Player* newPlayer,
        std::size_t& oldBucketSize
    #ifdef DEBUG_HASH
        , int& rehashCount
    #endif
    ) {
        std::size_t newBucketSize = 4 * oldBucketSize;
        Player** newBucket = nullptr;
        bool hasCollistion = false;

        do {
    #ifdef DEBUG_HASH
            ++rehashCount;
    #endif
            delete[] newBucket;
            newBucket = new Player*[newBucketSize]{ nullptr };

            {
                size_t index = GetHash(newPlayer->id, newBucketSize);
                newBucket[index] = newPlayer;
            }

            hasCollistion = false;
            for (std::size_t i = 0; i < oldBucketSize; ++i) {
                if (!orginalBucket[i]) {
                    continue;
                }

                auto* oldPlayer = orginalBucket[i];

                size_t index = GetHash(oldPlayer->id, newBucketSize);

                if (newBucket[index]) {
                    hasCollistion = true;
                    newBucketSize *= 4;
                    break;
                }

                newBucket[index] = oldPlayer;
            }
        }
        while (hasCollistion);
        
        delete[] orginalBucket;
        oldBucketSize = newBucketSize;
        orginalBucket = newBucket;
    }

    PlayerManager::~PlayerManager() {
        this->DestroyAllPlayers();
    }

    void PlayerManager::DestroyAllPlayers() {
        for (std::size_t i = 0; i < bucketSize; ++i) {
            auto player = this->pPlayers[i];
            if (player) {
                this->DestroyPlayerById(player->id);
            }
        }
    }

    Player* PlayerManager::CreatePlayer(const char* name, int id) {
        if (!this->pPlayers) {
            this->pPlayers = AllocateBucket(INITIAL_SIZE);
            this->bucketSize = INITIAL_SIZE;
        }

        std::size_t index = GetHash(id, this->bucketSize);
        if (this->pPlayers[index] && this->pPlayers[index]->id == id) {
            throw PlayerAlreadyExistException();
        }

        std::size_t nameLength = ::strnlen(name, MAX_NAME_LENGTH);

        Player* newPlayer = new Player{
            .name = new char[nameLength + 1]{0}, 
            .id = id,
        };
        ::strncpy(newPlayer->name, name, nameLength);

        if (this->pPlayers[index]) {
            ReHash(
                this->pPlayers,
                newPlayer,
                this->bucketSize
    #ifdef DEBUG_HASH
                , this->rehashCount
    #endif            
            );
        }
        else {
            this->pPlayers[index] = newPlayer;
        }
        ++this->numPlayers;

        index = GetHash(id, this->bucketSize);
        return this->pPlayers[index];
    }

    void PlayerManager::DestroyPlayerById(int id) {
        if (this->bucketSize == 0) {
            throw InvalidPlayerException();
        }

        std::size_t index = GetHash(id, this->bucketSize);
        if (this->pPlayers[index] == nullptr || this->pPlayers[index]->id != id) {
            throw InvalidPlayerException();
        }

        DeletePlayerRaw(this->pPlayers[index], this->numPlayers);
    }

    void PlayerManager::DestroyPlayer(Player* player) {
        if (player == nullptr || this->bucketSize == 0) {
            throw InvalidPlayerException();
        }

        std::size_t index = GetHash(player->id, this->bucketSize);
        if (this->pPlayers[index] != player) {
            throw InvalidPlayerException();
        }

        DeletePlayerRaw(this->pPlayers[index], this->numPlayers);
    }

    int PlayerManager::GetNumPlayers() const { 
        return this->numPlayers; 
    }

    Player* PlayerManager::GetPlayerById(int id) const {
        if (this->bucketSize == 0) {
            return nullptr;
        }

        std::size_t index = GetHash(id, this->bucketSize);
        if (this->pPlayers[index] == nullptr || this->pPlayers[index]->id != id) {
            return nullptr;
        }

        return this->pPlayers[index];
    }

}