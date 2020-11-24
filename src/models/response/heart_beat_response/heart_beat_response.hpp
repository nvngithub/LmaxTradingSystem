#ifndef HEART_BEAT_RESPONSE_H
#define HEART_BEAT_RESPONSE_H

#include "../response_base.hpp"

class HeartBeatResponse : public ResponseBase {
        public: 
            void FromFix(std::string fix) override;
};

#endif