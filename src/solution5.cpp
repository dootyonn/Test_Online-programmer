/*
Solution Details:
    -  
    - Process the UI update in a background thread so the service update are not blocked by UI updates


*/
#include <solution5.hpp>

#include <print>

namespace Quiz {

    CMatchmaking::CMatchmaking(IMatchmakingService* servicePtr)
    : pService(servicePtr)
    , processUpdatesThread([this]() { this->DoRequestUpdateBG(); })
    {
        updateQueueIsRunning.store(true);
    }

    CMatchmaking::~CMatchmaking()
    {
        ProcessDone();
        this->updateQueueIsRunning.store(false);

        //this->requestUpdateMutex.unlock();
        this->waitForUpdate.notify_one();
        this->processUpdatesThread.join();
    }

    void CMatchmaking::ProcessDone() {
        std::shared_ptr<RequestSession> activeSession;
        {
            std::scoped_lock lock(this->activeSessionMutex);
            activeSession = this->activeSession;
            this->activeSession = nullptr;
        }

        if (activeSession) {
            std::println("Session Done - id: {}", activeSession->id);
            activeSession->pUI->ClearUserCb();
        }
    }

    void CMatchmaking::ProcessCancel() {
        std::println("Cancel");

        {
            std::scoped_lock lock(this->activeSessionMutex);
            this->pService->CancelMatchRequest(this->activeSession->id);
        }

        this->ProcessDone();
    }

    void CMatchmaking::StartMatchRequest(IMatchmakingUI* pUI)
    {
        {
            std::scoped_lock lock(this->activeSessionMutex);

            if (this->activeSession) {
                throw SessionIsAlreadyActiveException();
            }
            
            pUI->SetUserCb([this] () { this->ProcessCancel(); });
            TRequestId requestId = this->pService->RequestMatch([this] (EMatchState state, void* state_data) {
                this->ProcessMatchMakingServiceUpdate(state, state_data);
            });

            if (requestId == INVALID_REQUEST) {
                throw InvalidRequest();
            }

            std::println("Session Start - id: {}", requestId);
            this->activeSession = std::make_shared<RequestSession>(requestId, pUI);
        }
    }

    void CMatchmaking::ProcessMatchMakingServiceUpdate(EMatchState state, void* state_data) {
        TRequestId requestId{};

        {
            std::scoped_lock lock(this->activeSessionMutex);
            if (this->activeSession == nullptr) {
                return;
            }

            requestId = this->activeSession->id;
        }
        RequestUpdate update{ .id = requestId };

        switch (state)
        {
        case EMatchState::EWaiting:
            update.waiting = (*static_cast<SMatchRequestState_Waiting*>(state_data));
            break;
        case EMatchState::EFoundMatch:
            update.matchFound = (*static_cast<SMatchRequestState_MatchFound*>(state_data));
            break;
        case EMatchState::ENoMatchFound:
            update.matchNotFound = (*static_cast<SMatchRequestState_MatchNotFound*>(state_data));
            break;
        default:
            break;
        }

        {
            std::scoped_lock lock(this->requestUpdateMutex);
            this->updates.push(update);
        }
        this->waitForUpdate.notify_one();
    }

    void CMatchmaking::DoRequestUpdateBG() {
        std::println("Start BG");

        while (this->updateQueueIsRunning) {
            std::unique_lock requestLock(this->requestUpdateMutex);
            this->waitForUpdate.wait(requestLock, [this] () {
                return !this->updates.empty() || !this->updateQueueIsRunning;
            });

            std::println("BG: has received new Request Update...");

            while (!this->updates.empty()) {
                std::shared_ptr<RequestSession> activeSession;
                {
                    std::scoped_lock lock(this->activeSessionMutex);
                    activeSession = this->activeSession;
                }

                if (activeSession) {
                    auto update = this->updates.front();

                    if (update.waiting.has_value()) {
                        std::println("BG: waiting event");

                        std::string message = std::format("There's {} users waiting...", update.waiting.value().numberInQueue);
                        activeSession->pUI->SetMatchSearchState(message.c_str());
                    }
                    else if (update.matchFound.has_value()) {
                        std::println("BG: match found event");

                        const auto& matchFound = update.matchFound.value();
                        activeSession->pUI->OnMatchFound(matchFound.hostname.c_str(), matchFound.port);
                        this->ProcessDone();
                    }
                    else if (update.matchNotFound.has_value()) {
                        std::println("BG: match not found event");

                        activeSession->pUI->OnMatchNoFound(update.matchNotFound.value().reason.c_str());
                        this->ProcessDone();
                    }
                }

                this->updates.pop();
            }
        }

        std::println("End BG");
    }

}