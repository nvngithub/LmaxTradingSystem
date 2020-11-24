#include "heart_beat_request.hpp"

HeartBeatRequest::HeartBeatRequest() {
    
}

HeartBeatRequest::HeartBeatRequest(std::string&& testReqId, bool isMarketDataConnection) : HeartBeatRequest() {
    this->isMarketDataConnection = isMarketDataConnection;
    this->TestReqID = std::move(testReqId);
}

std::string HeartBeatRequest::ToFix() {
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "HeartBeatRequest::Converting data to fix");

    std::string fix = RequestBase::GetFixHeader("0");

    if(this->TestReqID.empty() == false) {
        fix += "112=" + this->TestReqID + (char)0x01;
    }

    RequestBase::SetSeqNum(fix, (this->isMarketDataConnection ? Globals::GetMarketDataSeqNum() : Globals::GetOrderSeqNum()));
    RequestBase::SetMsgLength(fix);

    fix += RequestBase::GetFixTrailer(fix);

    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "HeartBeatRequest::HeartBeat request in fix: " + fix);

    return fix;
}