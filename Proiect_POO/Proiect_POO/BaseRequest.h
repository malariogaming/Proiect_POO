#pragma once
#include <chrono>
#include "IRequest.h"
class BaseRequest :
    public IRequest
{
protected:
    int targetFloor;
    long timestamp;
public:
    BaseRequest(int targetFloor);
    virtual ~BaseRequest() override = default;

    int getTargetFloor() const override;
    virtual bool isUrgent() const = 0;

    bool operator<(const BaseRequest& other) const; //pentru timestamp
    bool operator==(const BaseRequest& other) const; //pentru etaj
};