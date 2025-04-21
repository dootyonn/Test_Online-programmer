#ifndef SOLUTION5_HPP
#define SOLUTION5_HPP

#include <optional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

#include <queue>
#include <string>
#include <functional>
#include <exception>

namespace Quiz
{
    
    typedef int TRequestId;
    constexpr TRequestId INVALID_REQUEST = 0;

    enum class EMatchState : int
    {
        EWaiting,
        EFoundMatch,
        ENoMatchFound
    };

    // EWaiting: regularly reported while waiting
    struct SMatchRequestState_Waiting
    {
        int numberInQueue{ 0 };
    };

    // EFoundMatch: reported when match was found
    struct SMatchRequestState_MatchFound
    {
        std::string hostname;
        int port;
    };

    // ENoMatchFound: reported when no match was found
    struct SMatchRequestState_MatchNotFound
    {
        std::string reason;
    };

    // matchmaking service
    struct IMatchmakingService
    {
        // access to the matchmaker service instance
        // static IMatchmakingService* GetService();

        // function to request a match
        // will call callback while request is active to notify about state
        typedef std::function<void(EMatchState state, void* state_data)> TCallback;
        virtual TRequestId RequestMatch(const TCallback& cb) = 0;

        // cancel pending match request.
        // No further callbacks are called and the request got cancelled
        virtual void CancelMatchRequest(TRequestId id) = 0;
    };

    // matchmaking UI
    struct IMatchmakingUI
    {
        // update the match request dialog status message
        virtual void SetMatchSearchState(const char* message) = 0;

        // call one of the two functions to end the dialog
        virtual void OnMatchFound(const char* host, int port) = 0;
        virtual void OnMatchNoFound(const char* reason) = 0;
        
        // callback for user 'cancel' button on dialog
        // when called, the UI will close and no other action is needed
        typedef std::function<void()> TUserCancelCb;
        virtual void SetUserCb(const TUserCancelCb& cb) = 0;
        virtual void ClearUserCb() = 0;
    };

    // Please implement the following class
    // - request a match from the IMatchmakingService
    // - update the UI of the match request state while request is active
    // - call OnMatchFound/OnMatchNoFound once the matchmaking service has a result
    // - handle user callback to cancel the pending request
    // - cleanup properly
    // - validate input parameters and class usage
    class CMatchmaking
    {
    public:
        CMatchmaking(IMatchmakingService* pService);
        ~CMatchmaking();

        // called by UI, user pressed 'search match' button
        void StartMatchRequest(IMatchmakingUI* pUI);

    private:
        // you can add member variables and functions
        struct RequestSession {
            RequestSession(TRequestId requestId, IMatchmakingUI* ui)
            : id(requestId)
            , pUI(ui)
            {}

            TRequestId id;
            IMatchmakingUI* pUI;
        };

        struct RequestUpdate {
            TRequestId id{0};
            std::optional<SMatchRequestState_Waiting> waiting;
            std::optional<SMatchRequestState_MatchFound> matchFound;
            std::optional<SMatchRequestState_MatchNotFound> matchNotFound;
        };

        void DoRequestUpdateBG();

    public:
        struct SessionIsAlreadyActiveException : public std::exception {};
        struct InvalidRequest : public std::exception {};

        CMatchmaking(const CMatchmaking&) = delete;
        CMatchmaking& operator=(const CMatchmaking&)= delete;

    protected:
        void ProcessMatchMakingServiceUpdate(EMatchState state, void* state_data);
        void ProcessCancel();
        void ProcessDone();

    private:
        std::thread processUpdatesThread;
        IMatchmakingService* pService;

        std::mutex activeSessionMutex;
        std::shared_ptr<RequestSession> activeSession;

        std::mutex requestUpdateMutex;
        std::condition_variable waitForUpdate;
        std::queue<RequestUpdate> updates;

        std::atomic<bool> updateQueueIsRunning;
        
    };

} 
    

#endif