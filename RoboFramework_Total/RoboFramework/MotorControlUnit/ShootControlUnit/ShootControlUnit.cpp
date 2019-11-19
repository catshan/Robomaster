//
// Created by Administrator on 2019/11/18.
//

#include "ShootControlUnit.h"
using namespace RoboFramework;
void RoboFramework::ShootMotor::run(float target) {
    pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = pid->calculate(in);
    sendData(out);
}

void RoboFramework::ShootControlThread::start() {
    if(ShootControlUnit::isShootMode()){
        ShootControlUnit::getMotor()->run(SHOOT_SPEED);
    }
}


RoboFramework::ShootMotor *RoboFramework::ShootControlUnit::getMotor() {
    return motor;
}
Thread<ShootControlThread>* shootThread = nullptr;
ShootMotor* ShootControlUnit::motor = nullptr;
bool ShootControlUnit::shootMode = false;
void RoboFramework::ShootControlUnit::Init() {
    motor = new ShootMotor(5);
    shootThread = new Thread<ShootControlThread>("Shoot",512);
    shootThread->Login();
}


void RoboFramework::ShootControlUnit::setShootMode(bool shootMode) {
    ShootControlUnit::shootMode = shootMode;
}

bool RoboFramework::ShootControlUnit::isShootMode() {
    return shootMode;
}
