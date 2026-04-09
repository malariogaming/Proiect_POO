#pragma once
#include <string>
class ITransportable
{
protected:

public:
	virtual ~ITransportable() = default;
	virtual double getWeight() const = 0;
	virtual int getDestination() const = 0;

	virtual std::string getName() const = 0;
	virtual int getStartFloor() const = 0;
};