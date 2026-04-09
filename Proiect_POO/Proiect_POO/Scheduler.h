#pragma once
#include <vector>
#include <memory>
#include "BaseElevator.h"
#include "IRequest.h"
#include "Building.h"
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

	BaseElevator* calculateNearestCar(const IRequest& req, const std::vector<std::shared_ptr<BaseElevator>>& elevators);
	void processLOOKAlgorithm(Building& building);
};