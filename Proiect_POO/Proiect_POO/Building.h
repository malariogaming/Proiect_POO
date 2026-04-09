#pragma once
#include <vector>
#include <memory>
#include "Floor.h"
#include "BaseElevator.h"
class Building
{
protected:
	std::vector<Floor> floors;
	std::vector<std::shared_ptr<BaseElevator>> elevators;
public:
	Building(int numFloors);

	Building& operator+=(std::shared_ptr<BaseElevator> elevator);

	std::vector<Floor>& getFloors();
	const std::vector<std::shared_ptr<BaseElevator>>& getElevators() const;

	void initSimulation();
};