#ifndef SINGLE_ORDER_REQUEST_H
#define SINGLE_ORDER_REQUEST_H

#include <regex>

#include "../request_base.hpp"
#include "../../../logger/logger.hpp"

class SingleOrderRequest : public RequestBase {
    public:
        enum OrderSide {
            BUY = 1, 
            SELL = 2
        };

        enum OrderType {
            MARKET = 1,
            LIMIT = 2,
            STOP = 3,
            STOP_LIMIT = 4
        };

        std::string instrummentID;
        float qty;
        bool isBuy;
        SingleOrderRequest::OrderSide side;
        SingleOrderRequest::OrderType orderType;

        SingleOrderRequest(bool isMarketDataConnection);
        SingleOrderRequest(SingleOrderRequest&& singleOrderRequest);

        std::string ToFix() override;
};

#endif