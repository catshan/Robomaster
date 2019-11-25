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
    for(;;Delay::ms(1)) {
        if (ShootControlUnit::isShootMode()) {
            ShootControlUnit::getMotor()[0]->run(TAGGLE_SPEED);
            ShootControlUnit::getMotor()[0]->run(SHOOT_SPEED);
            ShootControlUnit::getMotor()[1]->run(SHOOT_SPEED);
        } else {
            ShootControlUnit::getMotor()[0]->run(0);
            ShootControlUnit::getMotor()[0]->run(0);
            ShootControlUnit::getMotor()[1]->run(0);
        }
    }
}


RoboFramework::ShootMotor **RoboFramework::ShootControlUnit::getMotor() {
    return shootMotor;
}
Thread<ShootControlThread>* shootThread = nullptr;
TaggleMotor* ShootControlUnit::taggleMotor[1] = {nullptr};
ShootMotor* ShootControlUnit::shootMotor[2] = {nullptr};
bool ShootControlUnit::shootMode = false;
void RoboFramework::ShootControlUnit::Init() {
    taggleMotor[0] = new TaggleMotor(5);
    shootMotor[0] = new ShootMotor(6);
    shootMotor[1] = new ShootMotor(7);
    shootThread = new Thread<ShootControlThread>("Shoot",512);
    shootThread->Login();
}


void RoboFramework::ShootControlUnit::setShootMode(bool shootMode) {
    ShootControlUnit::shootMode = shootMode;
}

bool RoboFramework::ShootControlUnit::isShootMode() {
    return shootMode;
}

TaggleMotor **ShootControlUnit::getTaggleMotor() {
    return taggleMotor;
}

void TaggleMotor::run(float target) {
    pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = pid->calculate(in);
    sendData(out);
}
