#ifndef MARKET_DATA_F
#define MARKET_DATA_F

#include <string>
#include <vector>

#include "../../instrument.hpp"
#include "../request_base.hpp"
#include "../../../globals/globals.hpp"
#include "../../../logger/logger.hpp"

class MarketDataRequest : public RequestBase {
    public:
        MarketDataRequest(std::vector<Instrument>&& instrumemnts, bool isMarketDataConnection);

        MarketDataRequest(const MarketDataRequest& marketDataRequest) = delete;
        MarketDataRequest(MarketDataRequest&& marketDataRequest) noexcept = default;

        std::vector<Instrument> instruments;
        std::string ToFix() override;

        ~MarketDataRequest() {
            Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "MarketDataRequest::Destroying instance.");
        }
};

#endif