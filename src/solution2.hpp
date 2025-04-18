#ifndef SOLUTION2_HPP
#define SOLUTION2_HPP

#include <exception>

namespace Quiz {

    struct Player
    {
        char* name;
        int id;
    };

    class PlayerManager
    {
    public:
        PlayerManager() 
        : numPlayers(0)
        , pPlayers(nullptr)
        , bucketSize(0)
    #ifdef DEBUG_HASH
        , rehashCount(0)
    #endif    
        {}

        ~PlayerManager();

        // destroys all players and deallocates their memory
        void DestroyAllPlayers();

        // allocates memory and returns the new player object
        Player* CreatePlayer(const char* name, int id);

        // destroys the player object and deallocates its memory
        void DestroyPlayerById(int id);
        void DestroyPlayer(Player* pPlayer);

        // returns the number of currently allocated players
        int GetNumPlayers() const { return this->numPlayers; }

        // returns a player by ID
        Player* GetPlayerById(int id) const ;

    private:
        Player** pPlayers;
        int numPlayers;

        // you can add additional member variables and functions if you need
    public:
        class PlayerAlreadyExistException : public std::exception {};
        class InvalidPlayerException : public std::exception {};

    private:
        std::size_t bucketSize;
    #ifdef DEBUG_HASH
        int rehashCount;
    #endif
    };

    static constexpr std::size_t INITIAL_SIZE = 32;
    static constexpr std::size_t MAX_NAME_LENGTH = 100;

}

#endif