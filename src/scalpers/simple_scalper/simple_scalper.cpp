#include "simple_scalper.hpp"

void SimpleScalper::GenericMarketDataResponseHandler(std::unique_ptr<ResponseBase> response) {
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "GenericMarketDataResponseHandler::Callback invoked from service.");

    if(dynamic_cast<LoginResponse*>(response.get())) {
        Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "GenericMarketDataResponseHandler::Starting heartbeat.");
        this->marketDataService->StartHeartBeat();

        std::vector<Instrument> instruments;
        instruments.emplace_back(Instrument("4001")); // EURUSD
        this->marketDataService->AddRequest(std::make_unique<MarketDataRequest>(MarketDataRequest(std::move(instruments), true)));
    }
}

void SimpleScalper::GenericTradingResponseHandler(std::unique_ptr<ResponseBase> response) {
    Logger::Instance()->Add(Logger::LogType::GENERAL_TRADING, Logger::LogLevel::INFORMATION, "GenericTradingResponseHandler::Callback invoked from service.");

    if(dynamic_cast<LoginResponse*>(response.get())) {
        Logger::Instance()->Add(Logger::LogType::GENERAL_TRADING, Logger::LogLevel::INFORMATION, "GenericTradingResponseHandler::Starting heartbeat.");
        this->tradingService->StartHeartBeat();
    }
}

void SimpleScalper::Start() {
    /*ClientSubscription genericMarketDataSubscription;
    genericMarketDataSubscription.callback = std::bind(&SimpleScalper::GenericMarketDataResponseHandler, this, std::placeholders::_1);
    this->marketDataService->Subscribe(std::move(genericMarketDataSubscription));

    this->marketDataService->AddRequest(std::make_unique<AutheticateRequest>(AutheticateRequest("naveen289", "Nyenburgh12!@", true)));*/

    ClientSubscription genericTradingSubscription;
    genericTradingSubscription.callback = std::bind(&SimpleScalper::GenericTradingResponseHandler, this, std::placeholders::_1);
    this->tradingService->Subscribe(std::move(genericTradingSubscription));

    this->tradingService->AddRequest(std::make_unique<AutheticateRequest>(AutheticateRequest("naveen289", "Nyenburgh12!@", false)));

    /*std::this_thread::sleep_for(std::chrono::seconds(5));

    std::unique_ptr<SingleOrderRequest> order = std::make_unique<SingleOrderRequest>(SingleOrderRequest(false));
    order->side = SingleOrderRequest::OrderSide::BUY;
    order->orderType = SingleOrderRequest::OrderType::MARKET;
    order->qty = 0.1;
    order->instrummentID = "4001";
    this->tradingService->AddRequest(std::move(order));*/
    
}

void SimpleScalper::End() {

}