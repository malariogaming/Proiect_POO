#pragma once
#include <vector>
#include <memory>
#include "Person.h"
class Floor
{
protected:
	int level;
	std::vector<std::shared_ptr<Person>> waitingQueue;
public:
	Floor(int level);

	void addPassenger(std::shared_ptr<Person> p);
	void removePassenger();

	int getLevel() const;
	const std::vector<std::shared_ptr<Person>>& getWaitingQueue() const;
};