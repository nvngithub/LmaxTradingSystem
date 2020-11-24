#ifndef REQUEST_BASE_H
#define REQUEST_BASE_H

#include <map>
#include <string>

#include "../../globals/globals.hpp"

class RequestBase {
    public:
        enum class RequestType {
            MARKET_DATA, 
            AUTHENTICATION
        };

        RequestType type;

        virtual std::string ToFix() = 0;

    protected:
        std::map<std::string, std::string> fixedValues;
        bool isMarketDataConnection = false;

        std::string GetFixHeader(const std::string& msgType);
        std::string GetFixTrailer(const std::string& fix);
        void SetSeqNum(std::string& fix, int seqnum);
        void SetMsgLength(std::string& fix);
};

#endif
