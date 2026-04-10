#include "VIPPassenger.h"

VIPPassenger::VIPPassenger(std::string name, double weight, int startFloor, int destinationFloor, int priorityLevel) : Person(name, weight, startFloor, destinationFloor), priorityLevel(priorityLevel)
{
}

std::string VIPPassenger::getRole() const
{
	return "VIP (Etaj: " + std::to_string(destinationFloor) + ", Prioritate: " + std::to_string(priorityLevel) + ")";
}

std::string VIPPassenger::getIcon() const
{
	return "🌟";
}
