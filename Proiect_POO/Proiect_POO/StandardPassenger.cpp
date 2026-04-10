#include "StandardPassenger.h"

StandardPassenger::StandardPassenger(std::string name, double weight, int startFloor, int destinationFloor) : Person(name, weight, startFloor, destinationFloor)
{
}

std::string StandardPassenger::getRole() const
{
	return "Standard (Etaj: " + std::to_string(destinationFloor) + ")";
}

std::string StandardPassenger::getIcon() const
{
	return "👤";
}
