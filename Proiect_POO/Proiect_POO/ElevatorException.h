#pragma once
#include "MyException.h"
class ElevatorException :
    public MyException
{
protected:
    int elevatorId;
public:
    ElevatorException(std::string descriere, int elevatorId);
    virtual ~ElevatorException() override = default;

    virtual std::string what() const override;
};