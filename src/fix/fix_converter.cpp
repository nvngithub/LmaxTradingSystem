#include "fix_converter.hpp"

std::string FixConverter::ConvertToFix_(std::unique_ptr<RequestBase>&& request){
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "FixConverter::Start conversion.");
    if(dynamic_cast<AutheticateRequest*>(request.get())){
        return dynamic_cast<AutheticateRequest*>(request.get())->ToFix();
    }else if(dynamic_cast<HeartBeatRequest*>(request.get())){
        return dynamic_cast<HeartBeatRequest*>(request.get())->ToFix();
    }else if(dynamic_cast<MarketDataRequest*>(request.get())){
        return dynamic_cast<MarketDataRequest*>(request.get())->ToFix();
    }else if(dynamic_cast<SingleOrderRequest*>(request.get())){
        return dynamic_cast<SingleOrderRequest*>(request.get())->ToFix();
    }
}

std::unique_ptr<ResponseBase> FixConverter::ConvertFromFix_(std::string fix){
    std::regex fixType("35=[0-1A-Z]{1,2}");
    std::smatch matches;

    std::regex_search(fix, matches, fixType);

    if(matches.size() > 0) {
        std::string match = static_cast<std::string>(matches[0]);
        std::string type = match.substr(3, match.size() - 3);

        std::unique_ptr<ResponseBase> response;
        if(type == "A") {
             response = std::make_unique<LoginResponse>(LoginResponse());
        } else if(type == "0") {
            response = std::make_unique<HeartBeatResponse>(HeartBeatResponse());
        } else if(type == "1") {
            response = std::make_unique<ServiceTestRequest>(ServiceTestRequest());
        } else if(type == "W") {
            response = std::make_unique<MarketDataResponse>(MarketDataResponse());
        }

        response->FromFix(fix);

        return response;
    }

    return nullptr;
}