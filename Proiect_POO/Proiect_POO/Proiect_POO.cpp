#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include "OverweightException.h"
#include "InvalidFloorException.h"
#include "StandardPassenger.h"
#include "VIPPassenger.h"
#include "CargoBox.h"
#include "PassengerElevator.h"
#include "FreightElevator.h"
#include "EmergencyElevator.h"
#include "InternalRequest.h"
#include "ExternalRequest.h"
#include "Building.h"
#include "Scheduler.h"
#include "DatabaseManager.h"
#include <QApplication>
#include "MainWindow.h"

//void testareSistem() {
//    std::cout << "\n--- Incepem testarea ierarhiei de exceptii ---\n" << std::endl;
//
//    try {
//        std::cout << "[Test 1] Incercam sa incarcam liftul 5 cu prea multe colete..." << std::endl;
//        throw OverweightException(5, 45.5); // ID lift: 5, Exces: 45.5 kg
//    }
//    catch (const MyException& e) {
//        std::cout << "Prins: " << e.what() << "\n" << std::endl;
//    }
//
//    try {
//        std::cout << "[Test 2] Utilizatorul a apasat butonul pentru etajul 13..." << std::endl;
//        throw InvalidFloorException(13);
//    }
//    catch (const MyException& e) {
//        std::cout << "Prins: " << e.what() << "\n" << std::endl;
//    }
//
//    try {
//        std::cout << "[Test 3] Eroare generica de configurare..." << std::endl;
//        throw LogicException("Baza de date nu a putut fi initializata", 500);
//    }
//    catch (const MyException& e) {
//        std::cout << "Prins: " << e.what() << std::endl;
//    }
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testarePasageri() {
//    std::cout << "\n--- Incepem testarea ierarhiei de pasageri ---\n" << std::endl;
//
//    std::vector<std::shared_ptr<Person>> listaAsteptare;
//
//    listaAsteptare.push_back(std::make_shared<StandardPassenger>("Mario", 75.0, 0, 5));
//    listaAsteptare.push_back(std::make_shared<VIPPassenger>("Seful Cel Mare", 90.0, 10, 2, 1));
//
//    for (auto& p : listaAsteptare) {
//        std::cout << *p << std::endl;
//    }
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testareCargo() {
//    std::cout << "\n--- Testare Cargo Box ---\n" << std::endl;
//
//    CargoBox cutie(120.5, true, 0, 5);
//    std::cout << cutie << std::endl;
//
//    ITransportable* cevaDeTransportat = &cutie;
//    std::cout << "Greutatea prin interfata: " << cevaDeTransportat->getWeight() << " kg" << std::endl;
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testareLifturiSpecifice() {
//    std::cout << "\n--- Testare Ierarhie Lifturi ---\n" << std::endl;
//
//    try {
//        std::cout << "[Test 1] PassengerElevator:\n" << std::endl;
//        auto pLift = std::make_shared<PassengerElevator>(1, 500.0, 0, 10, 2); // ID 101, 500kg, 2 persoane max
//
//        pLift->playElevatorMusic();
//
//        auto p1 = std::make_shared<StandardPassenger>("Ion", 80.0, 2, 5);
//        auto p2 = std::make_shared<StandardPassenger>("Maria", 60.0, 4, 3);
//        auto p3 = std::make_shared<StandardPassenger>("Vasile", 90.0, 6, 8);
//
//        pLift->loadItem(p1);
//        pLift->loadItem(p2);
//
//        std::cout << "Incercam sa incarcam a treia persoana (limitata la 2)" << std::endl;
//        if (!pLift->canAcceptWeight(p3->getWeight())) {
//            std::cout << ">> Liftul a refuzat corect a treia persoana (capacitate persoane atinsa)" << std::endl;
//        }
//
//        pLift->moveTo(5);
//    }
//    catch (const MyException& e) {
//        std::cout << "EROARE NEASTEPTATA: " << e.what() << std::endl;
//    }
//
//    std::cout << "------------------------------------" << std::endl;
//
//    try {
//        std::cout << "[Test 2] FreightElevator:\n" << std::endl;
//        auto fLift = std::make_shared<FreightElevator>(2, 2000.0, 0, 10);
//
//        fLift->enableHeavyMode();
//        auto seif = std::make_shared<CargoBox>(1500.0, false, 2, 5);
//
//        fLift->loadItem(seif);
//        std::cout << "Status: " << fLift->getType() << " la etajul " << fLift->getCurrentFloor() << std::endl;
//
//        fLift->moveTo(5);
//    }
//    catch (const MyException& e) {
//        std::cout << "Eroare: " << e.what() << std::endl;
//    }
//
//    std::cout << "------------------------------------" << std::endl;
//
//    try {
//        std::cout << "[Test 3] EmergencyElevator:\n" << std::endl;
//        auto eLift = std::make_shared<EmergencyElevator>(3, 300.0, 0, 10);
//
//        eLift->activateRedCode();
//
//        std::cout << "Incercam sa incarcam ceva mult prea greu pentru liftul de urgenta" << std::endl;
//        auto utilaj = std::make_shared<CargoBox>(500.0, false, 3, 5);
//
//        eLift->loadItem(utilaj);
//    }
//    catch (const OverweightException& e) {
//        std::cout << ">> CAPTURAT CORECT: " << e.what() << std::endl;
//    }
//    catch (const MyException& e) {
//        std::cout << "Eroare generica: " << e.what() << std::endl;
//    }
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testareRequesturi() {
//    std::cout << "\n--- Testare ierarhie requesturi ---\n" << std::endl;
//
//    auto req1 = std::make_shared<InternalRequest>(10, 1);
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//    auto req2 = std::make_shared<ExternalRequest>(0, "UP");
//
//    std::cout << "[Cerere 1] Target: " << req1->getTargetFloor()
//        << " | Urgent: " << (req1->isUrgent() ? "DA" : "NU") << std::endl;
//
//    std::cout << "[Cerere 2] Target: " << req2->getTargetFloor()
//        << " | Urgent: " << (req2->isUrgent() ? "DA" : "NU") << std::endl;
//
//    InternalRequest reqIdentic(10, 2);
//    if (*req1 == reqIdentic) {
//        std::cout << ">> OK: Operatorul == a detectat aceeasi destinatie (Etajul 10)" << std::endl;
//    }
//    else {
//        std::cout << ">> EROARE: Etajele nu au fost detectate ca fiind identice" << std::endl;
//    }
//
//    if (*req1 < *req2) {
//        std::cout << ">> OK: Operatorul < a confirmat ca req1 a sosit inaintea req2" << std::endl;
//    }
//    else {
//        std::cout << ">> EROARE: Ordonarea cronologica a esuat" << std::endl;
//    }
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testBuilding() {
//    std::cout << "\n--- Testare building ---\n" << std::endl;
//
//    Building b(10);
//
//    auto lift = std::make_shared<PassengerElevator>(1, 500, 0, 10, 5);
//    b += lift;
//
//    auto p = std::make_shared<StandardPassenger>("Andrei", 75, 0, 7);
//    b.getFloors()[0].addPassenger(p);
//
//    b.initSimulation();
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void ruleazaSimulare() {
//    std::cout << "\n--- Testare un lift ---\n" << std::endl;
//
//    DatabaseManager db;
//    db.connectToDB("elevator_sim.db");
//
//    Building cladire(6);
//
//    auto lift = std::make_shared<PassengerElevator>(1, 400.0, 0, 5, 4);
//    cladire += lift;
//
//    auto gigel = std::make_shared<StandardPassenger>("Gigel", 80.0, 0, 3);
//    cladire.getFloors()[0].addPassenger(gigel);
//
//    auto maria = std::make_shared<StandardPassenger>("Maria", 60.0, 2, 5);
//    cladire.getFloors()[2].addPassenger(maria);
//
//    auto mihai = std::make_shared<StandardPassenger>("Mihai", 75.0, 3, 1);
//    cladire.getFloors()[3].addPassenger(mihai);
//
//    Scheduler* creier = Scheduler::getInstance();
//    bool continua = true;
//    int secunda = 0;
//
//    while (continua && secunda < 30) {
//        std::cout << "\n[Timp: " << secunda << "s] ---" << std::endl;
//
//        creier->processLOOKAlgorithm(cladire, db);
//
//        // Verificam daca mai sunt oameni care asteapta la vreun etaj
//        bool oameniLaEtaj = false;
//        for (auto& f : cladire.getFloors()) {
//            if (!f.getWaitingQueue().empty()) oameniLaEtaj = true;
//        }
//
//        // Verificam daca mai sunt oameni in lift
//        bool oameniInLift = (lift->getStatus() != ElevatorStatus::IDLE);
//
//        // Daca totul e gol si liftul e IDLE, oprim
//        if (!oameniLaEtaj && !oameniInLift) {
//            std::cout << "\n[Simulare] Succes! Toti pasagerii au ajuns la destinatie" << std::endl;
//            continua = false;
//        }
//
//        secunda++;
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
//
//    Scheduler::destroyInstance();
//
//    db.closeDB();
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}
//
//void testSimulareDouaLifturi() {
//    std::cout << "\n--- Testare doua lifturi ---\n" << std::endl;
//
//    DatabaseManager db;
//    db.connectToDB("elevator_sim.db");
//
//    Building cladire(10);
//
//    // Adaugam doua lifturi
//    auto liftA = std::make_shared<PassengerElevator>(1, 400.0, 0, 9, 4);
//    auto liftB = std::make_shared<PassengerElevator>(2, 400.0, 0, 9, 4);
//
//    cladire += liftA;
//    cladire += liftB;
//
//    // Punem lifturile in pozitii diferite pentru inceput
//    liftB->moveTo(8); // Trimitem liftul B la etajul 8 manual pentru test
//    liftB->setStatus(ElevatorStatus::IDLE);
//
//    // Adaugam un pasager la etajul 5 care vrea la parter
//    auto pasager = std::make_shared<StandardPassenger>("Andrei", 70.0, 5, 0);
//    cladire.getFloors()[5].addPassenger(pasager);
//
//    Scheduler* creier = Scheduler::getInstance();
//    bool continua = true;
//    int secunda = 0;
//
//    while (continua && secunda < 10) {
//        std::cout << "\n[Timp: " << secunda << "s] ---" << std::endl;
//
//        creier->processLOOKAlgorithm(cladire, db);
//
//        secunda++;
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
//
//    Scheduler::destroyInstance();
//
//    db.closeDB();
//
//    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
//}

int main(int argc, char *argv[])
{
    //try {
    //    //TESTE INITIALE:
    //    // 
    //    //testareSistem();
    //    //testarePasageri();
    //    //testareCargo();
    //    //testareLifturiSpecifice();
    //    //testareRequesturi();
    //    //testBuilding();
    //    //
    //    //ruleazaSimulare();
    //    //testSimulareDouaLifturi();
    //}
    //catch (const std::exception& e) {
    //    std::cout << "Eroare: " << e.what() << std::endl;
    //}

    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}