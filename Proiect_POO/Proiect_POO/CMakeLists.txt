cmake_minimum_required(VERSION 3.16)

project(ProiectLifturi VERSION 1.0 LANGUAGES CXX C)   

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/msvc2022_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(SOURCES
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    AddPassengerDialog.cpp
    AddPassengerDialog.h
    AddPassengerDialog.ui
    Proiect_POO.cpp
    BaseElevator.cpp
    BaseRequest.cpp
    Building.cpp
    CargoBox.cpp
    DatabaseManager.cpp
    ElevatorException.cpp
    EmergencyElevator.cpp
    ExternalRequest.cpp
    Floor.cpp
    FreightElevator.cpp
    IElevator.cpp
    InternalRequest.cpp
    InvalidFloorException.cpp
    IRequest.cpp
    ITransportable.cpp
    LogicException.cpp
    MyException.cpp
    OverweightException.cpp
    PassengerElevator.cpp
    Person.cpp
    Scheduler.cpp
    StandardPassenger.cpp
    VIPPassenger.cpp
    sqlite3.c
)

add_executable(ProiectLifturi WIN32 ${SOURCES})

target_link_libraries(ProiectLifturi PRIVATE Qt6::Widgets)

if(WIN32)
    set(DEBUG_SUFFIX)
    if(MSVC)
        set(DEBUG_SUFFIX "d")
    endif()
endif()