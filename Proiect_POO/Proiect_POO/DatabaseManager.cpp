#include <iostream>
#include "DatabaseManager.h"
#include "LogicException.h"
extern "C" {
    #include "sqlite3.h"
}

DatabaseManager::DatabaseManager() : db(nullptr)
{
}

DatabaseManager::~DatabaseManager()
{
	closeDB();
}

bool DatabaseManager::connectToDB(const std::string& dbName)
{
    int code = sqlite3_open(dbName.c_str(), (sqlite3**)&(this->db));

    if (code != SQLITE_OK) {
        throw LogicException("Eroare la deschiderea bazei de date: " + dbName, code);
    }

    // cream tabelul daca nu exista
    std::string sql = "CREATE TABLE IF NOT EXISTS RIDES("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "ELEV_ID INT NOT NULL, "
                      "START_FLOOR INT NOT NULL, "
                      "END_FLOOR INT NOT NULL, "
                      "PASSENGER_NAME TEXT NOT NULL, "
                      "TIMESTAMP DATETIME DEFAULT (datetime('now', 'localtime')));";

    char* messageError = nullptr;

    code = sqlite3_exec((sqlite3*)this->db, sql.c_str(), NULL, 0, &messageError);

    if (code != SQLITE_OK) {
        std::string errorStr = (messageError != nullptr) ? messageError : "Eroare necunoscuta";

        if (messageError) sqlite3_free(messageError);

        throw LogicException("Eroare SQL la initializarea tabelelor: " + errorStr, code);
    }

    std::cout << "[DB] Conectat cu succes la " << dbName << std::endl;
    return true;
}

void DatabaseManager::logRide(int elevId, int startFloor, int endFloor, const std::string& passengerName) {
    if (this->db == nullptr) {
        throw LogicException("Tentativa de logare a cursei esuata: Baza de date nu este conectata!", 0);
    }

    // construim interogarea SQL
    std::string sql = "INSERT INTO RIDES (ELEV_ID, START_FLOOR, END_FLOOR, PASSENGER_NAME) VALUES (" +
        std::to_string(elevId) + ", " +
        std::to_string(startFloor) + ", " +
        std::to_string(endFloor) + ", '" +
        passengerName + "');";

    char* messageError = nullptr;

    // inseram
    int exit = sqlite3_exec((sqlite3*)db, sql.c_str(), NULL, 0, &messageError);

    // verificam rezultatul
    if (exit != SQLITE_OK) {
        std::string errorStr = (messageError != nullptr) ? messageError : "Eroare SQL necunoscuta";

        if (messageError) sqlite3_free(messageError);

        throw LogicException("Eroare la inserarea cursei in baza de date: " + errorStr, exit);
    }

    std::cout << "[DB Log] Cursa salvata cu succes pentru: " << passengerName << std::endl;
}

void DatabaseManager::closeDB()
{
	if (this->db) {
		sqlite3_close((sqlite3*)this->db);
		this->db = nullptr;
	}
}