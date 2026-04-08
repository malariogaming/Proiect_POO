#pragma once
class ITransportable
{
protected:

public:
	virtual ~ITransportable() = default;
	virtual double getWeight() const = 0;
};