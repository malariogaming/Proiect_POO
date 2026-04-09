#include "Building.h"

Building::Building(int numFloors)
{
	for (int i = 0; i < numFloors; ++i) {
		floors.push_back(Floor(i));
	}
	std::cout << "[Building] Structura creata cu " << numFloors << " etaje" << std::endl;
}

Building& Building::operator+=(std::shared_ptr<BaseElevator> elevator)
{
	if (elevator) {
		elevators.push_back(elevator);
		std::cout << "[Building] Liftul ID " << elevator->getId() << " a fost adaugat in sistem" << std::endl;
	}
	return *this;
}

std::vector<Floor>& Building::getFloors()
{
	return this->floors;
}

const std::vector<std::shared_ptr<BaseElevator>>& Building::getElevators() const
{
	return this->elevators;
}

void Building::initSimulation()
{
	std::cout << "[Building] Se initializeaza simularea" << std::endl;
	std::cout << "Status: " << floors.size() << " etaje active si " << elevators.size() << " lifturi operationale" << std::endl;
	
	// logica efectiva a simularii...
}