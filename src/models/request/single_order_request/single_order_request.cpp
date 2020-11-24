#include "single_order_request.hpp"

SingleOrderRequest::SingleOrderRequest(bool isMarketDataConnection) {
    this->isMarketDataConnection = isMarketDataConnection;

    this->fixedValues.insert(std::make_pair("22", "8"));
    this->fixedValues.insert(std::make_pair("59", "3"));
}

SingleOrderRequest::SingleOrderRequest(SingleOrderRequest&& singleOrderRequest) : SingleOrderRequest(singleOrderRequest.isMarketDataConnection) {
}

std::string GetTimestamp(std::string& fix) {
    std::regex timestampRegex("52=[\\d]{8}-[\\d]{2}:[\\d]{2}:[\\d]{2}\\.[\\d]{3}");
    std::smatch matches;

    std::regex_search(fix, matches, timestampRegex);
    if(matches.size() > 0) {
        std::string match = static_cast<std::string>(matches[0]);
        return match.substr(3, match.size() - 3);
    }

    return {};
}

std::string SingleOrderRequest::ToFix() {
    Logger::Instance()->Add(Logger::LogType::GENERAL_TRADING, Logger::LogLevel::INFORMATION, "SingleOrderRequest::Converting data to fix");

    std::string fix = RequestBase::GetFixHeader("D");

    for (auto& [key, value] : this->fixedValues) {
        fix += key + "=" + value + (char)0x01;
    }

    fix += "60=" + GetTimestamp(fix) + (char)0x01;
    fix += "11=" + std::to_string(::Globals::GetOrderId()) + (char)0x01;
    fix += "40=" + std::to_string(this->orderType) + (char)0x01;
    fix += "54=" + std::to_string(this->side) + (char)0x01;
    std::string quantity { std::to_string(this->qty) };
    fix += "38=" + (quantity.erase(quantity.find_last_not_of("0") + 1)) + (char)0x01;
    fix += "48=" + this->instrummentID + (char)0x01;

    RequestBase::SetSeqNum(fix, Globals::GetOrderSeqNum());
    RequestBase::SetMsgLength(fix);

    fix += RequestBase::GetFixTrailer(fix);

    Logger::Instance()->Add(Logger::LogType::GENERAL_TRADING, Logger::LogLevel::INFORMATION, "SingleOrderRequest::Single order request in fix: " + fix);

    return fix;
}