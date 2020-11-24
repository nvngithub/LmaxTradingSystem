#include "globals.hpp"

std::atomic<int> Globals::marketDataSeqNum = 1;
std::atomic<int> Globals::orderSeqNum = 1;
std::atomic<int> Globals::orderId = 1;
const std::string Globals::SenderCompID = "naveen289";
const std::string Globals::TargetTradingCompID = "LMXBD";
const std::string Globals::TargetMarketDataCompID = "LMXBDM";

int Globals::GetMarketDataSeqNum() {
    return marketDataSeqNum++;
}

int Globals::GetOrderSeqNum() {
    return orderSeqNum++;
}

int Globals::GetOrderId() {
    return orderId++;
}