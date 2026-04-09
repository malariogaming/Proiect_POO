#include "Floor.h"

Floor::Floor(int level) : level(level)
{
}

void Floor::addPassenger(std::shared_ptr<Person> p)
{
	this->waitingQueue.push_back(p);
	std::cout << "[Etaj " << this->level << "] Pasager nou adaugat in asteptare" << std::endl;
}

void Floor::removePassenger()
{
	if (!(this->waitingQueue.empty())) {
		this->waitingQueue.erase(this->waitingQueue.begin());
		std::cout << "[Etaj " << this->level << "] Un pasager a urcat in lift" << std::endl;
	}
}

int Floor::getLevel() const
{
	return this->level;
}

const std::vector<std::shared_ptr<Person>>& Floor::getWaitingQueue() const
{
	return this->waitingQueue;
}