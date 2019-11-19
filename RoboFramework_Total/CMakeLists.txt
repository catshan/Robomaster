cmake_minimum_required(VERSION 3.14)

set(STM32EL_DIR $ENV{STM32EL_PATH} )
find_package(STM32EL)

project(RoboFramework_Total)

file(GLOB_RECURSE SOURCE "RoboFramework/*.*")

include_directories(
        RoboFramework
        RoboFramework/CanBusUnit
        RoboFramework/RemoteUnit
        RoboFramework/MotorControlUnit/ChassisControlUnit
        RoboFramework/MotorControlUnit/HeadControlUnit
        RoboFramework/MotorControlUnit/ShootControlUnit
        RoboFramework/Node/CanNode
        RoboFramework/Node/MotorNode
        RoboFramework/Node/MotorNode/Motor
        RoboFramework/PID)

add_executable(RoboFramework_Total ${SOURCE} main.cpp )