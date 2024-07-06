cmake_minimum_required(VERSION 3.27)
project(PlanetVsZombies)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.7.0\\mingw_64\\lib\\cmake")

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
)

target_link_libraries(PlanetVsZombies
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Network
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
