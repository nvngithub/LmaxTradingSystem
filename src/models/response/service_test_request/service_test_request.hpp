#ifndef SERVICE_TEST_REQUEST_H
#define SERVICE_TEST_REQUEST_H

#include "../response_base.hpp"

class ServiceTestRequest : public ResponseBase {
    public:
        void FromFix(std::string fix) override;
};

#endif