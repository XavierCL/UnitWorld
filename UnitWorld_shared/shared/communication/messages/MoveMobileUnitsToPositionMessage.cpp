#include "MoveMobileUnitsToPositionMessage.h"

#include <nlohmann/json.hpp>

using namespace uw;

MoveMobileUnitsToPositionMessage::MoveMobileUnitsToPositionMessage(const std::vector<xg::Guid>& singuityIds, const CommunicatedVector2D& destination) :
    _singuityIds(singuityIds),
    _destination(destination)
{}

std::shared_ptr<MoveMobileUnitsToPositionMessage> MoveMobileUnitsToPositionMessage::fromJson(const std::string& json)
{
    nlohmann::json parsedData = nlohmann::json::parse(json);

    std::vector<xg::Guid> singuityIds;
    for (const auto singuityId : parsedData.at(MOBILE_UNITS_ID_LABEL))
    {
        singuityIds.emplace_back(singuityId.get<std::string>());
    }

    return std::make_shared<MoveMobileUnitsToPositionMessage>(
        std::move(singuityIds),
        CommunicatedVector2D::fromJson(parsedData.at(DESTINATION_POSITION_LABEL))
    );
}

MessageType MoveMobileUnitsToPositionMessage::messageType() const
{
    return MessageType::MoveMobileUnitsToPositionMessageType;
}

std::string MoveMobileUnitsToPositionMessage::toJsonData() const
{
    std::vector<std::string> singuityStringIds;
    for (const auto singuityId : _singuityIds)
    {
        singuityStringIds.emplace_back(singuityId.str());
    }

    nlohmann::json parsedDestination(_destination.toJson());

    nlohmann::json returnedJson({
        {MOBILE_UNITS_ID_LABEL, singuityStringIds},
        {DESTINATION_POSITION_LABEL, parsedDestination}
    });

    return returnedJson.dump();
}

std::vector<xg::Guid> MoveMobileUnitsToPositionMessage::singuityIds() const
{
    return _singuityIds;
}

CommunicatedVector2D MoveMobileUnitsToPositionMessage::destination() const
{
    return _destination;
}

const std::string MoveMobileUnitsToPositionMessage::MOBILE_UNITS_ID_LABEL = "m";
const std::string MoveMobileUnitsToPositionMessage::DESTINATION_POSITION_LABEL = "d";