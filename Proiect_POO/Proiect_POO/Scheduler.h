#pragma once
#include <vector>
#include <memory>
#include "BaseElevator.h"
#include "IRequest.h"
#include "Building.h"
#include "DatabaseManager.h"
class Scheduler
{
protected:
	static Scheduler* instance;
	Scheduler();
	Scheduler(const Scheduler& obj) = delete;
	Scheduler& operator = (const Scheduler& obj) = delete;
	~Scheduler();
public:
	static Scheduler* getInstance();
	static void destroyInstance();

	BaseElevator* calculateNearestCar(int targetFloor, double requiredWeight, const std::vector<std::shared_ptr<BaseElevator>>& elevators);
	void processLOOKAlgorithm(Building& building, DatabaseManager& db);
};