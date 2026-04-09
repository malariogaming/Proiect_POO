#pragma once
#include <string>

class DatabaseManager {
private:
    void* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool connectToDB(const std::string& dbName = "elevator_sim.db");
    void logRide(int elevId, int startFloor, int endFloor, const std::string& passengerName);
    void closeDB();
};