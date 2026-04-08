#include <iostream>
#include <vector>
#include <memory>
#include "OverweightException.h"
#include "InvalidFloorException.h"
#include "StandardPassenger.h"
#include "VIPPassenger.h"
#include "CargoBox.h"

void testareSistem() {
    std::cout << "--- Incepem testarea ierarhiei de exceptii ---\n" << std::endl;

    try {
        std::cout << "[Test 1] Incercam sa incarcam liftul 5 cu prea multe colete..." << std::endl;
        throw OverweightException(5, 45.5); // ID lift: 5, Exces: 45.5 kg
    }
    catch (const MyException& e) {
        std::cout << "Prins: " << e.what() << "\n" << std::endl;
    }

    try {
        std::cout << "[Test 2] Utilizatorul a apasat butonul pentru etajul 13..." << std::endl;
        throw InvalidFloorException(13);
    }
    catch (const MyException& e) {
        std::cout << "Prins: " << e.what() << "\n" << std::endl;
    }

    try {
        std::cout << "[Test 3] Eroare generica de configurare..." << std::endl;
        throw LogicException("Baza de date nu a putut fi initializata", 500);
    }
    catch (const MyException& e) {
        std::cout << "Prins: " << e.what() << "\n" << std::endl;
    }

    std::cout << "--- Testare finalizata ---\n" << std::endl;
}

void testarePasageri() {
    std::cout << "--- Incepem testarea ierarhiei de pasageri ---\n" << std::endl;

    std::vector<std::shared_ptr<Person>> listaAsteptare;

    listaAsteptare.push_back(std::make_shared<StandardPassenger>("Mario", 75.0, 5));
    listaAsteptare.push_back(std::make_shared<VIPPassenger>("Seful Cel Mare", 90.0, 10, 1));

    for (auto& p : listaAsteptare) {
        std::cout << *p << std::endl;
    }

    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
}

void testareCargo() {
    std::cout << "--- Testare Cargo Box ---\n" << std::endl;

    CargoBox cutie(120.5, true);
    std::cout << cutie << std::endl;

    ITransportable* cevaDeTransportat = &cutie;
    std::cout << "Greutatea prin interfata: " << cevaDeTransportat->getWeight() << " kg" << std::endl;

    std::cout << "\n--- Testare finalizata ---\n" << std::endl;
}

int main()
{
    //testareSistem();
    //testarePasageri();
    //testareCargo();





    return 0;
}