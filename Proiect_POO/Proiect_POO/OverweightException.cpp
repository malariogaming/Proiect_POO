#include "OverweightException.h"

OverweightException::OverweightException(int elevatorId, double excessWeight) : ElevatorException("Greutate depasita!", elevatorId), excessWeight(excessWeight)
{
}

std::string OverweightException::what() const
{
	return ElevatorException::what() + " | Exces: " + std::to_string(excessWeight) + " kg";
}