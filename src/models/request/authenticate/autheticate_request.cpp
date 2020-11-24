#include "autheticate_request.hpp"

AutheticateRequest::AutheticateRequest() {
    this->fixedValues.insert(std::make_pair("98", "0"));
    this->fixedValues.insert(std::make_pair("108", "30"));
    this->fixedValues.insert(std::make_pair("141", "Y"));
}

AutheticateRequest::AutheticateRequest(std::string&& username, std::string&& password, bool isMarketDataConnection) : AutheticateRequest() {
    this->isMarketDataConnection = isMarketDataConnection;
    this->Username = std::move(username);
    this->Password = std::move(password);
}

AutheticateRequest::AutheticateRequest(AutheticateRequest&& autheticateRequest) : AutheticateRequest() {
    this->isMarketDataConnection = autheticateRequest.isMarketDataConnection;
    this->Username = std::move(autheticateRequest.Username);
    this->Password = std::move(autheticateRequest.Password);
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "AutheticateRequest::Move constructor called");
}

std::string AutheticateRequest::ToFix() {
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "AutheticateRequest::Converting data to fix");

    std::string fix = RequestBase::GetFixHeader("A");

    for (auto& [key, value] : this->fixedValues) {
        fix += key + "=" + value + (char)0x01;
    }

    fix += "553=" + this->Username + (char)0x01;
    fix += "554=" + this->Password + (char)0x01;

    RequestBase::SetSeqNum(fix, (this->isMarketDataConnection ? Globals::GetMarketDataSeqNum() : Globals::GetOrderSeqNum()));
    RequestBase::SetMsgLength(fix);

    fix += RequestBase::GetFixTrailer(fix);

    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "AutheticateRequest::Authentication request in fix: " + fix);

    return fix;
}

