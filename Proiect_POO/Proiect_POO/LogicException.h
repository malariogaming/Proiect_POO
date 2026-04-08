#pragma once
#include "MyException.h"
class LogicException :
    public MyException
{
protected:
    int errorCode;
public:
    LogicException(std::string descriere, int errorCode);
    virtual ~LogicException() override = default;

    virtual std::string what() const override;
};