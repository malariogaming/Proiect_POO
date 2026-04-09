#include "InternalRequest.h"

InternalRequest::InternalRequest(int targetFloor, int elevatorId) : BaseRequest(targetFloor), elevatorId(elevatorId)
{
}

bool InternalRequest::isUrgent() const
{
	return (targetFloor == 0); //situatii de urgenta precum evacuarea
}