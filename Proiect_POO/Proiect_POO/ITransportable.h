#pragma once
class ITransportable
{
protected:

public:
	virtual ~ITransportable() = default;
	virtual double getWeight() const = 0;
	virtual int getDestination() const = 0;
};