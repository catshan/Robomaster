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
        ShootControlUnit::getMotor()[0]->run(TAGGLE_SPEED);
    }
    ShootControlUnit::getMotor()[1]->run(SHOOT_SPEED);
    ShootControlUnit::getMotor()[2]->run(-SHOOT_SPEED);
}


RoboFramework::ShootMotor **RoboFramework::ShootControlUnit::getMotor() {
    return motor;
}
Thread<ShootControlThread>* shootThread = nullptr;
ShootMotor* ShootControlUnit::motor[3] = {nullptr};
bool ShootControlUnit::shootMode = false;
void RoboFramework::ShootControlUnit::Init() {
    motor[0] = new ShootMotor(5);
    motor[1] = new ShootMotor(6);
    motor[2] = new ShootMotor(7);
    shootThread = new Thread<ShootControlThread>("Shoot",512);
    shootThread->Login();
}


void RoboFramework::ShootControlUnit::setShootMode(bool shootMode) {
    ShootControlUnit::shootMode = shootMode;
}

bool RoboFramework::ShootControlUnit::isShootMode() {
    return shootMode;
}
