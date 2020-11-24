#ifndef MARKET_DATA_RESPONSE_H
#define MARKET_DATA_RESPONSE_H

#include "../response_base.hpp"

class MarketDataResponse : public ResponseBase {
    public:
        void FromFix(std::string fix) override;
};

#endif