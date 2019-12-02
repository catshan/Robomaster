//
// Created by Administrator on 2019/11/18.
//

#include "ShootControlUnit.h"
using namespace RoboFramework;
TaggleMotor* ShootControlUnit::motor_Tag[1] = {nullptr};
Thread<ShootControlThread>* shootThread = nullptr;
ShootMotor* ShootControlUnit::motor[2] = {nullptr};
bool ShootControlUnit::shootMode = false;
bool ShootControlUnit::singleMode = false;
bool ShootControlUnit::stopMode = false;
bool ShootControlUnit::openMode = false;
PWM<Tim2>* servoMotor = new PWM<Tim2 >(20,PWM_Channel::CH1,"PA0");
void RoboFramework::ShootMotor::run(float target) {
    pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = pid->calculate(in);
    sendData(out);
}

void RoboFramework::ShootControlThread::start() {
    for(;;Delay::ms(1)) {
        if(ShootControlUnit::isOpenMode()){
            servoMotor->run(14);
        }if(!ShootControlUnit::isOpenMode()){
            servoMotor->run(33.5);
        }if(ShootControlUnit::isShootMode()) {
            ShootControlUnit::getMotorTag()[0]->run(TAGGLE_SPEED);
            ShootControlUnit::getMotor()[0]->run(SHOOT_SPEED);
            ShootControlUnit::getMotor()[1]->run(-SHOOT_SPEED);
        }if(ShootControlUnit::isStopMode()){
            ShootControlUnit::getMotorTag()[0]->run(0);
            ShootControlUnit::getMotor()[0]->run(0);
            ShootControlUnit::getMotor()[1]->run(0);
        }

    }
}


RoboFramework::ShootMotor **RoboFramework::ShootControlUnit::getMotor() {
    return motor;
}

void RoboFramework::ShootControlUnit::Init() {
    IOPort led = Gpio::Register("PG13");
    led = 1;
    motor_Tag[0] = new TaggleMotor(5,CanType::Can2);
    motor[0] = new ShootMotor(5,CanType::Can1);
    motor[1] = new ShootMotor(6,CanType::Can1);
    shootThread = new Thread<ShootControlThread>("Shoot",256);
    shootThread->Login();
}


void RoboFramework::ShootControlUnit::setShootMode(bool shootMode) {
    ShootControlUnit::shootMode = shootMode;
}

bool RoboFramework::ShootControlUnit::isShootMode() {
    return shootMode;
}

TaggleMotor **ShootControlUnit::getMotorTag() {
    return motor_Tag;
}

bool ShootControlUnit::isSingleMode() {
    return singleMode;
}

void ShootControlUnit::setSingleMode(bool singleMode) {
    ShootControlUnit::singleMode = singleMode;
}

bool ShootControlUnit::isStopMode() {
    return stopMode;
}

void ShootControlUnit::setStopMode(bool stopMode) {
    ShootControlUnit::stopMode = stopMode;
}

bool ShootControlUnit::isOpenMode() {
    return openMode;
}

void ShootControlUnit::setOpenMode(bool openMode) {
    ShootControlUnit::openMode = openMode;
}

void TaggleMotor::run(float target) {
    pid->setTarget(target);
    short in =  getMotor()->getSpeed();
    short out = pid->calculate(in);
    sendData(out);
}
//
//void TaggleMotor::run_single(float target) {
//    pid1->setTarget(target);
//    sendData(pid1->calculate(getMotor()->getSpeed()));
//    Delay::ms(100);
//    sendData(0);
//}

