#include "StandardPassenger.h"

StandardPassenger::StandardPassenger(std::string name, double weight, int destinationFloor) : Person(name, weight, destinationFloor)
{
}

std::string StandardPassenger::getRole() const
{
	return "Standard (Etaj: " + std::to_string(destinationFloor) + ")";
}