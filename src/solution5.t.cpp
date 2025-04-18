#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <solution5.hpp>

#include <chrono>
#include <map>

using namespace ::testing;

namespace Quiz {
namespace Test {

    struct TestCMatchmaking : public CMatchmaking {
        TestCMatchmaking(IMatchmakingService* pService) 
        : CMatchmaking(pService)
        { }

        void DoProcessMatchMakingServiceUpdate(EMatchState state, void* state_data) {
            this->ProcessMatchMakingServiceUpdate(state, state_data);
        }

        void DoProcessCancel() {
            this->ProcessCancel();
        }

    };

    class MockMatchmakingService : public IMatchmakingService {
    public:
        MOCK_METHOD(TRequestId, RequestMatch, (const IMatchmakingService::TCallback& cb), (override));
        MOCK_METHOD(void, CancelMatchRequest, (TRequestId id), (override));
    };


    class MoxkMatchmakingUI : public IMatchmakingUI {
    public:
        MOCK_METHOD(void, SetMatchSearchState, (const char* message), (override));

        MOCK_METHOD(void, OnMatchFound, (const char* host, int port), (override));
        MOCK_METHOD(void, OnMatchNoFound, (const char* reason), (override));
        
        MOCK_METHOD(void, SetUserCb, (const IMatchmakingUI::TUserCancelCb& cb), (override));
        MOCK_METHOD(void, ClearUserCb, (), (override));
    };


    TEST(solution5, testStartMatchServiceWillCallRequestMatch) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            CMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Cleanup
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    TEST(solution5, testStartMatchServiceWithInvalidRequestId) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            CMatchmaking matchMaking(&service);

            const TRequestId requestId = INVALID_REQUEST;
            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            EXPECT_THROW(matchMaking.StartMatchRequest(&ui), CMatchmaking::InvalidRequest);

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    TEST(solution5, testStartMatchServiceTwiceWithoutCompletingTheSessionWillThrow) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            CMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Cleanup
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            EXPECT_THROW(matchMaking.StartMatchRequest(&ui), CMatchmaking::SessionIsAlreadyActiveException);            
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    TEST(solution5, testProcessMatchMakingUpdate) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            TestCMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Process updates
            EXPECT_CALL(ui, SetMatchSearchState(_));
            EXPECT_CALL(ui, SetMatchSearchState(_));

            // Cleanup
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            {
                SMatchRequestState_Waiting update { .numberInQueue = 12 };
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::EWaiting, (void*)(&update));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            {
                SMatchRequestState_Waiting update { .numberInQueue = 8 };
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::EWaiting, (void*)(&update));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }


    TEST(solution5, testProcessMatchMakingCancel) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            TestCMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Process updates
            EXPECT_CALL(ui, SetMatchSearchState(_));

            // Cleanup
            EXPECT_CALL(service, CancelMatchRequest(requestId));
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            {
                SMatchRequestState_Waiting update { .numberInQueue = 12 };
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::EWaiting, (void*)(&update));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            {
                matchMaking.DoProcessCancel();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    TEST(solution5, testProcessMatchMakingFound) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            TestCMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            const int port = 12345;

            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Process updates
            EXPECT_CALL(ui, OnMatchFound(_, port));

            // Cleanup
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            {
                SMatchRequestState_MatchFound update { .hostname = "hostname1", .port = port };
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::EFoundMatch, (void*)(&update));
                // ignored
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::EFoundMatch, (void*)(&update));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    TEST(solution5, testProcessMatchMakingNotFound) {
        MockMatchmakingService service;
        MoxkMatchmakingUI ui;

        {
            InSequence sequence;
            TestCMatchmaking matchMaking(&service);

            const TRequestId requestId = 123;
            const std::string reason = "Not Found...";

            // Setup
            EXPECT_CALL(ui, SetUserCb(_));
            EXPECT_CALL(service, RequestMatch(_)).WillOnce(Return(requestId));

            // Process updates
            EXPECT_CALL(ui, OnMatchNoFound(_));

            // Cleanup
            EXPECT_CALL(ui, ClearUserCb());

            matchMaking.StartMatchRequest(&ui);

            {
                SMatchRequestState_MatchNotFound update { .reason = reason };
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::ENoMatchFound, (void*)(&update));
                // ignored
                matchMaking.DoProcessMatchMakingServiceUpdate(EMatchState::ENoMatchFound, (void*)(&update));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

}
}