cmake_minimum_required(VERSION 3.27)
project(PlanetVsZombies)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(PlanetVsZombies main.cpp
    resources.qrc
    playground.h playground.cpp
        Card.cpp
        Card.h
        entities/zombie/Zombie.cpp
        entities/zombie/Zombie.h
        ground.h ground.cpp
        entities/zombie/RegularZombie.cpp
        entities/zombie/RegularZombie.h
        entities/GameEntity.cpp
        entities/GameEntity.h
        entities/plant/Plant.cpp
        entities/plant/Plant.h
        entities/plant/PeaShooter.cpp
        entities/plant/PeaShooter.h
        entities/zombie/TallZombie.cpp
        entities/zombie/TallZombie.h
        entities/zombie/BucketHeadZombie.cpp
        entities/zombie/BucketHeadZombie.h
        entities/zombie/LeafHeadZombie.cpp
        entities/zombie/LeafHeadZombie.h
        entities/zombie/PurpleHairZombie.cpp
        entities/zombie/PurpleHairZombie.h
        entities/zombie/AstronautZombie.cpp
        entities/zombie/AstronautZombie.h
        windows/Register/register.cpp
        windows/Register/register.h
        windows/Register/register.ui
        core/ClientSocket.cpp
        core/ClientSocket.h
        windows/Login/login.cpp
        windows/Login/login.h
        windows/Login/login.ui
        windows/Window.cpp
        windows/Window.h
        windows/Dashboard/dashboard.cpp
        windows/Dashboard/dashboard.h
        windows/Dashboard/dashboard.ui
        windows/mainwindow.cpp
        windows/mainwindow.h
        windows/ResetPassword/resetpassword.cpp
        windows/ResetPassword/resetpassword.h
        windows/ResetPassword/resetpassword.ui
        core/exceptions/ConnectionIsLostException.cpp
        core/exceptions/ConnectionIsLostException.h
        core/Player.cpp
        core/Player.h
        core/Cookie.cpp
        core/Cookie.h
        entities/plant/TwoPeaShooter.h
        entities/plant/TwoPeaShooter.cpp
        entities/plant/Walnut.h entities/plant/Walnut.cpp
        entities/plant/Plummine.h entities/plant/Plummine.cpp
        entities/plant/Jalapeno.h entities/plant/Jalapeno.cpp
        entities/plant/Boomrang.h entities/plant/Boomrang.cpp
        entities/plant/Bullet.h entities/plant/Bullet.cpp
        SunBrain.h SunBrain.cpp
        windows/History/history.cpp
        windows/History/history.h
        windows/History/history.ui
        windows/UpdateCredentials/updatecredentials.cpp
        windows/UpdateCredentials/updatecredentials.h
        windows/UpdateCredentials/updatecredentials.ui
        windows/HostConnector/hostconnector.cpp
        windows/HostConnector/hostconnector.h
        windows/HostConnector/hostconnector.ui
)

target_link_libraries(PlanetVsZombies
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)