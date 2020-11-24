#ifndef LOGIN_RESPONSE_H
#define LOGIN_RESPONSE_H

#include "../response_base.hpp"

class LoginResponse : public ResponseBase {
    public:
        void FromFix(std::string fix) override;
};

#endif