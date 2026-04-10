#pragma once
#include <vector>
#include <memory>
#include "Person.h"
class Floor
{
protected:
	int level;
	std::vector<std::shared_ptr<ITransportable>> waitingQueue;
public:
	Floor(int level);

	void addPassenger(std::shared_ptr<ITransportable> p);
	void removePassenger();

	int getLevel() const;
	const std::vector<std::shared_ptr<ITransportable>>& getWaitingQueue() const;
};