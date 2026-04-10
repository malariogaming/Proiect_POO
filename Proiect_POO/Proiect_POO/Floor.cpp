#include "Floor.h"

Floor::Floor(int level) : level(level)
{
}

void Floor::addPassenger(std::shared_ptr<ITransportable> p)
{
	this->waitingQueue.push_back(p);
	std::cout << "[Etaj " << this->level << "] Entitate noua adaugata in asteptare" << std::endl;
}

void Floor::removePassenger()
{
	if (!(this->waitingQueue.empty())) {
		this->waitingQueue.erase(this->waitingQueue.begin());
		std::cout << "[Etaj " << this->level << "] O entitate a urcat in lift" << std::endl;
	}
}

int Floor::getLevel() const
{
	return this->level;
}

const std::vector<std::shared_ptr<ITransportable>>& Floor::getWaitingQueue() const
{
	return this->waitingQueue;
}