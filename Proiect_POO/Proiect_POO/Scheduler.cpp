#include "Scheduler.h"

Scheduler* Scheduler::instance = nullptr;

Scheduler::Scheduler()
{
	std::cout << "[Scheduler] Instanta creata cu succes" << std::endl;
}

Scheduler::~Scheduler()
{
	std::cout << "[Scheduler] Instanta distrusa cu succes" << std::endl;
}

Scheduler* Scheduler::getInstance()
{
	if (instance == nullptr) {
		instance = new Scheduler();
	}
	return instance;
}

void Scheduler::destroyInstance()
{
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

BaseElevator* Scheduler::calculateNearestCar(const IRequest& req, const std::vector<std::shared_ptr<BaseElevator>>& elevators)
{
    BaseElevator* bestCar = nullptr;
    int minDistance = 999999999;
    int targetFloor = req.getTargetFloor();

    for (const auto& elev : elevators) {
        int dist = std::abs(elev->getCurrentFloor() - targetFloor);

        if (dist < minDistance) {
            minDistance = dist;
            bestCar = elev.get(); // Returnam pointerul brut
        }
    }

    if (bestCar) {
        std::cout << "[Scheduler] Cel mai apropiat lift identificat: ID " << bestCar->getId() << std::endl;
    }

    return bestCar;
}

void Scheduler::processLOOKAlgorithm(Building& building) {
    auto& elevators = building.getElevators();
    auto& floors = building.getFloors();

    for (auto& elev : elevators) {
        if (elev->getStatus() == ElevatorStatus::OUT_OF_SERVICE) continue;

        int currentF = elev->getCurrentFloor();

        // 1. DESCARCARE PASAGERI

        if (elev->hasPassengersForFloor(currentF)) {
            elev->openDoors();
            elev->unloadPassengersAt(currentF);
            elev->closeDoors();
        }

        // 2. INCARCARE PASAGERI

        auto& currentFloorQueue = floors[currentF].getWaitingQueue();
        if (!currentFloorQueue.empty()) {
            elev->openDoors();
            // Incarcam cat mai multi oameni
            while (!currentFloorQueue.empty()) {
                // Luam primul om din coada
                std::shared_ptr<Person> nextPerson = currentFloorQueue.front();
                double personWeight = nextPerson->getWeight(); // Greutatea sa

                if (elev->canAcceptWeight(personWeight)) {
                    try {
                        elev->loadItem(nextPerson); // Il urcam in lift
                        floors[currentF].removePassenger(); // Il scoatem din coada etajului
                    }
                    catch (const OverweightException& e) {
                        // In cazul in care e aruncata vreo exceptie din cauza greutatii
                        std::cout << "[Senzor] Alarma greutate: " << e.what() << std::endl;
                        break;
                    }
                }
                else {
                    // Daca prima persoana din coada e prea grea sau liftul e aproape plin
                    std::cout << "[Lift " << elev->getId() << "] Lift plin sau persoana prea grea la etajul " << currentF << std::endl;
                    break;
                }
            }
            elev->closeDoors();
        }

        // 3. DECIZIA DE MISCARE (LOOK)
        bool requestsAbove = false;
        bool requestsBelow = false;

        for (int i = 0; i < floors.size(); ++i) {
            // Cereri externe
            if (!floors[i].getWaitingQueue().empty()) {
                if (i > currentF) requestsAbove = true;
                if (i < currentF) requestsBelow = true;
            }
            // Cereri interne
            if (elev->hasDestination(i)) {
                if (i > currentF) requestsAbove = true;
                if (i < currentF) requestsBelow = true;
            }
        }

        ElevatorStatus currentStatus = elev->getStatus();

        if (currentStatus == ElevatorStatus::MOVING_UP) {
            if (requestsAbove) elev->moveTo(currentF + 1);
            else if (requestsBelow) {
                elev->setStatus(ElevatorStatus::MOVING_DOWN);
                elev->moveTo(currentF - 1);
            }
            else elev->setStatus(ElevatorStatus::IDLE);
        }
        else if (currentStatus == ElevatorStatus::MOVING_DOWN) {
            if (requestsBelow) elev->moveTo(currentF - 1);
            else if (requestsAbove) {
                elev->setStatus(ElevatorStatus::MOVING_UP);
                elev->moveTo(currentF + 1);
            }
            else elev->setStatus(ElevatorStatus::IDLE);
        }
        else { // IDLE
            if (requestsAbove) {
                elev->setStatus(ElevatorStatus::MOVING_UP);
                elev->moveTo(currentF + 1);
            }
            else if (requestsBelow) {
                elev->setStatus(ElevatorStatus::MOVING_DOWN);
                elev->moveTo(currentF - 1);
            }
        }
    }
}