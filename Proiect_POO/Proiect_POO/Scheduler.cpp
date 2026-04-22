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

BaseElevator* Scheduler::calculateNearestCar(int targetFloor, double requiredWeight, const std::vector<std::shared_ptr<BaseElevator>>& elevators)
{
    BaseElevator* bestCar = nullptr;
    int minScore = 999999;

    for (const auto& elev : elevators) {
        if (elev->getStatus() == ElevatorStatus::OUT_OF_SERVICE) continue;

        // 1. VERIFICARE CAPACITATE CURENTA
        if (elev->getCurrentWeight() + requiredWeight > elev->getMaxWeight()) continue;

        int currentFloor = elev->getCurrentFloor();
        int dist = std::abs(currentFloor - targetFloor);
        int score = dist;

        // 2. LOGICA DE DIRECTIE
        bool isMovingUp = (elev->getStatus() == ElevatorStatus::MOVING_UP);
        bool isMovingDown = (elev->getStatus() == ElevatorStatus::MOVING_DOWN);

        if (isMovingUp) {
            if (targetFloor > currentFloor) {
                // e in drumul nostru
                score -= 2;
            }
            else {
                // nu e in drumul nostru, ci trebuie sa se intoarca
                score += (elev->getMaxDestination() - currentFloor) + (elev->getMaxDestination() - targetFloor);
            }
        }
        else if (isMovingDown) {
            if (targetFloor < currentFloor) {
                // e in drumul nostru
                score -= 2;
            }
            else {
                // nu e in drumul nostru, ci trebuie sa se intoarca
                score += (currentFloor - elev->getMinDestination()) + (targetFloor - elev->getMinDestination());
            }
        }

        // 3. PENALIZARE PENTRU AGLOMERATIE
        score += (elev->getCargo().size() * 1);

        if (score < minScore) {
            minScore = score;
            bestCar = elev.get(); // bestCar este un pointer simplu, dar elev este un std::shared_ptr, deci avem nevoie de .get() aici
        }
    }
    return bestCar;
}

void Scheduler::processLOOKAlgorithm(Building& building, DatabaseManager& db) {

    auto& elevators = building.getElevators();
    auto& floors = building.getFloors();

    for (auto& elev : elevators) {
        if (elev->getStatus() == ElevatorStatus::OUT_OF_SERVICE) continue;

        int currentF = elev->getCurrentFloor();

        if (elev->hasPassengersForFloor(currentF)) {
            elev->openDoors();

            for (auto& item : elev->getCargo()) {
                if (item->getDestination() == currentF) {
                    // EXTRAGEM DATELE:
                    int idLift = elev->getId();
                    int deLaEtajul = item->getStartFloor();
                    int laEtajul = currentF;
                    std::string nume = item->getName();

                    // trimitem catre sql
                    try {
                        db.logRide(idLift, deLaEtajul, laEtajul, nume);
                    }
                    catch (const LogicException& e) {
                        std::cout << "[Simulare] Eroare la logarea in DB: " << e.what() << std::endl;
                    }
                }
            }

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
                std::shared_ptr<ITransportable> nextPerson = currentFloorQueue.front();
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
            // Cereri interne
            if (elev->hasDestination(i)) {
                if (i > currentF) requestsAbove = true;
                if (i < currentF) requestsBelow = true;
            }

            // Verificam cererile EXTERNE (Oamenii de pe hol) folosind Nearest Car
            for(int i = 0; i < floors.size(); ++i) {
                if (!floors[i].getWaitingQueue().empty()) {
                    double weightAtFloor = floors[i].getWaitingQueue().front()->getWeight();

                    if (calculateNearestCar(i, weightAtFloor, elevators) == elev.get()) {
                        if (i > currentF) requestsAbove = true;
                        if (i < currentF) requestsBelow = true;
                    }
                }
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