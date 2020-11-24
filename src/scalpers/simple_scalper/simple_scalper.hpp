#ifndef SIMPLE_SCALPER_H
#define SIMPLE_SCALPER_H

#include <functional>
#include <chrono>
#include <vector>
#include <memory>

#include "../scalper.hpp"
#include "../../logger/logger.hpp"
#include "../../models/instrument.hpp"
#include "../../models/subscription/client_subscription.hpp"
#include "../../models/request/authenticate/autheticate_request.hpp"
#include "../../models/request/market_data_request/market_data_request.hpp"
#include "../../models/request/single_order_request/single_order_request.hpp"
#include "../../models/response/response_base.hpp"
#include "../../models/response/login_response/login_response.hpp"

class SimpleScalper : public Scalper {
    private:
        void GenericMarketDataResponseHandler(std::unique_ptr<ResponseBase> response);
        void GenericTradingResponseHandler(std::unique_ptr<ResponseBase> response);

    public:
        void Start() override;

        void End() override;
};

#endif