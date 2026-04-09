#pragma once
#include "BaseRequest.h"
class InternalRequest :
    public BaseRequest
{
protected:
    int elevatorId;
public:
    InternalRequest(int targetFloor, int elevatorId);
    bool isUrgent() const override;
};