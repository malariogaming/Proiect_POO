#include "BaseRequest.h"

BaseRequest::BaseRequest(int targetFloor) : targetFloor(targetFloor)
{
	this->timestamp = std::chrono::system_clock::now().time_since_epoch().count();
}

int BaseRequest::getTargetFloor() const
{
	return this->targetFloor;
}

bool BaseRequest::operator<(const BaseRequest& other) const
{
	return (this->timestamp < other.timestamp);
}

bool BaseRequest::operator==(const BaseRequest& other) const
{
	return (this->targetFloor == other.targetFloor);
}