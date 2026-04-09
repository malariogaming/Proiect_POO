#pragma once
#include <string>
class IRequest
{
protected:

public:
	virtual ~IRequest() = default;
	virtual int getTargetFloor() const = 0;
};