#include "FreightElevator.h"

FreightElevator::FreightElevator(int id, double maxWeight, int minFloor, int maxFloor) : BaseElevator(id, maxWeight, minFloor, maxFloor)
{
}

std::string FreightElevator::getType() const
{
	return "Freight Elevator";
}

void FreightElevator::enableHeavyMode()
{
	std::cout << "[Lift " << id << "] Sistemele hidraulice au fost intarite pentru sarcini grele" << std::endl;
}

bool FreightElevator::canAcceptWeight(double weight) const
{
	return BaseElevator::canAcceptWeight(weight);
}