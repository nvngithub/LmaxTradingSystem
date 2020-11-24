#ifndef AUTHENTICATE_REQUEST_H
#define AUTHENTICATE_REQUEST_H

#include "../request_base.hpp"
#include "../../../globals/globals.hpp"
#include "../../../logger/logger.hpp"

class AutheticateRequest : public RequestBase
{
    public:
        std::string Username;
        std::string Password;

        AutheticateRequest();

        AutheticateRequest(std::string&& username, std::string&& password, bool isMarketDataConnection);

        AutheticateRequest(AutheticateRequest&& autheticateRequest);

        ~AutheticateRequest() {
            Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "AutheticateRequest::Destroying instance.");
        }

        std::string ToFix() override;
};

#endif