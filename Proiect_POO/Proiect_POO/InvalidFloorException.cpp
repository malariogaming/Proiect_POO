#include "InvalidFloorException.h"

InvalidFloorException::InvalidFloorException(int requestedFloor) : LogicException("Etaj invalid solicitat", 404), requestedFloor(requestedFloor)
{
}

std::string InvalidFloorException::what() const
{
	return LogicException::what() + " | Etajul " + std::to_string(requestedFloor) + " nu exista";
}