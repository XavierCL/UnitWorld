#pragma once

#include "Message.h"

#include "game/CommunicatedCompleteGameState.h"

#include "commons/Guid.hpp"

namespace uw
{
    class CompleteGameStateMessage : public Message
    {
    public:
        CompleteGameStateMessage(const CommunicatedCompleteGameState& completeGameState, const xg::Guid& currentPlayerId);

        static std::shared_ptr<CompleteGameStateMessage> fromJson(const std::string& jsonData);

        virtual MessageType messageType() const;

        virtual std::string toJsonData() const;

        CommunicatedCompleteGameState completeGameState() const;

        xg::Guid getCurrentPlayerId() const;

    private:

        const CommunicatedCompleteGameState _completeGameState;
        const xg::Guid _currentPlayerId;
    };
}