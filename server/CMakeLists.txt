cmake_minimum_required(VERSION 3.27)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Network REQUIRED)

add_executable(server
        main.cpp
        bootstrap.h
        bootstrap.cpp
        Controllers/Controller.cpp
        Controllers/Controller.h
        TcpSocket.cpp
        TcpSocket.h
        Controllers/AuthorizationController.cpp
        Controllers/AuthorizationController.h
        Controllers/Validation/Validator.cpp
        Controllers/Validation/Validator.h
        Controllers/Validation/Rules.cpp
        Controllers/Validation/Rules.h
        Database/DB.cpp
        Database/DB.h
        exceptions/ModelNotFoundException.cpp
        exceptions/ModelNotFoundException.h
        Controllers/GameController.cpp
        Controllers/GameController.h
        Player.cpp
        Player.h
        Cache.cpp
        Cache.h
)

target_link_libraries(server Qt6::Core Qt6::Network)
