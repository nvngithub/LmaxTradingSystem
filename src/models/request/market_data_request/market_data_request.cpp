#include "market_data_request.hpp"

MarketDataRequest::MarketDataRequest(std::vector<Instrument>&& items, bool isMarketDataConnection) : instruments(std::move(items)) {
    this->isMarketDataConnection = isMarketDataConnection;
    
    this->fixedValues.insert(std::make_pair("262", "REQ00001")); // SubscriptionRequestType: 0 - subscribe, 1 - unsubscribe
    this->fixedValues.insert(std::make_pair("263", "1")); // SubscriptionRequestType: 0 - subscribe, 1 - unsubscribe
    this->fixedValues.insert(std::make_pair("264", std::to_string(::Globals::MarketDataDepth)));
    this->fixedValues.insert(std::make_pair("265", "0")); // 0 - full refresh
    this->fixedValues.insert(std::make_pair("267", "2")); // TODO: not sure of this
    //this->fixedValues.insert(std::make_pair("269", "0")); // 0 - bids
    //this->fixedValues.insert(std::make_pair("269", "1")); // 1 - asks
}

std::string MarketDataRequest::ToFix() {
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "MarketDataRequest::Converting data to fix");

    std::string fix = RequestBase::GetFixHeader("V");

    for (auto& [key, value] : this->fixedValues) {
        fix += key + "=" + value + (char)0x01;
    }

    std::string bids{"269=0"};
    fix += bids + (char)0x01; // 0 - bids
    std::string asks{"269=1"};
    fix += asks + (char)0x01; // 1 - asks

    fix += "146=" + std::to_string(this->instruments.size()) + (char)0x01;
    for(auto& instrument : this->instruments) {
        fix += "48=" + instrument.SecurityID + (char)0x01;
        fix += "22=" + instrument.SecurityIDSource + (char)0x01;
    }

    RequestBase::SetSeqNum(fix, Globals::GetMarketDataSeqNum());
    RequestBase::SetMsgLength(fix);

    fix += RequestBase::GetFixTrailer(fix);

    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "MarketDataRequest::Market data request in fix: " + fix);

    return fix;
}