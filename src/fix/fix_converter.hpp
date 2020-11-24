#ifndef FIX_CONVERTER_H
#define FIX_CONVERTER_H

#include <string>
#include <memory>
#include <regex>

#include "../models/request/request_base.hpp"
#include "../models/request/authenticate/autheticate_request.hpp"
#include "../models/request/heart_beat_request/heart_beat_request.hpp"
#include "../models/request/market_data_request/market_data_request.hpp"
#include "../models/request/single_order_request/single_order_request.hpp"
#include "../models/response/service_test_request/service_test_request.hpp"

#include "../models/response/response_base.hpp"
#include "../models/response/heart_beat_response/heart_beat_response.hpp"
#include "../models/response/market_data_response/market_data_response.hpp"
#include "../models/response/login_response/login_response.hpp"

class FixConverter {
    private:
        static std::string ConvertToFix_(std::unique_ptr<RequestBase>&& request);
        static std::unique_ptr<ResponseBase> ConvertFromFix_(std::string fix);

    public:
        template<typename T>
        static std::string ConvertToFix(T&& request) {
            return ConvertToFix_(std::move(request));
        }

        template<typename T>
        static T ConvertFromFix(std::string fix) {
            return ConvertFromFix_(fix);
        }
};

#endif