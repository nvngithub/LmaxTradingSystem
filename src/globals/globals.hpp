#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <atomic>

class Globals
{
    private:
        static std::atomic<int> marketDataSeqNum;
        static std::atomic<int> orderSeqNum;
        static std::atomic<int> orderId;

    public:
        static const int MarketDataDepth = 5;
        static const int BufferLength = 8190;
        static const std::string SenderCompID;
        static const std::string TargetTradingCompID;
        static const std::string TargetMarketDataCompID;

        static int GetMarketDataSeqNum();
        static int GetOrderSeqNum();
        static int GetOrderId();
};
#endif