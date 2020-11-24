#ifndef HEART_BEAT_REQUEST_H
#define HEART_BEAT_REQUEST_H

#include <string>

#include "../request_base.hpp"
#include "../../../globals/globals.hpp"
#include "../../../logger/logger.hpp"

class HeartBeatRequest : public RequestBase {
    public:
        std::string TestReqID {};

        HeartBeatRequest();
        HeartBeatRequest(std::string&& testReqId, bool isMarketDataConnection);
        HeartBeatRequest(HeartBeatRequest&&) = default;
        std::string ToFix() override;
};

#endif