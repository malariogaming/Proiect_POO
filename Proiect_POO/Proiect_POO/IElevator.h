#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "ITransportable.h"
#include "OverweightException.h"
#include "InvalidFloorException.h"

class IElevator
{
protected:

public:
	virtual ~IElevator() = default;

	virtual void moveTo(int floor) = 0;
	virtual void openDoors() = 0;
	virtual void closeDoors() = 0;
	virtual void loadItem(std::shared_ptr<ITransportable> item) = 0;
	virtual void unloadItem(std::shared_ptr<ITransportable> item) = 0;

	virtual int getCurrentFloor() const = 0;
	virtual int getId() const = 0;
};