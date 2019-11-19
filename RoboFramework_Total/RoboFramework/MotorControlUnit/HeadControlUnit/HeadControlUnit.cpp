//
// Created by Administrator on 2019/11/19.
//

#include "HeadControlUnit.h"
using namespace RoboFramework;
void RoboFramework::HeadMotor::run(float target) {
    s_pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = s_pid->calculate(in);
    sendData(out);

}

void RoboFramework::HeadMotor::run_angle(float target) {
    a_pid->setTarget(target);
    short in = getMotor()->getAngle();
    short out = a_pid->calculate(in);
    sendData(out);
}

RoboFramework::HeadControlUnit::HeadControlSignal *RoboFramework::HeadControlUnit::getSignal() {
    return &signal;
}

RoboFramework::HeadMotor **RoboFramework::HeadControlUnit::getMotor() {
    return motor;
}

bool RoboFramework::HeadControlUnit::isTopMode() {
    return topMode;
}

void RoboFramework::HeadControlUnit::setTopMode(bool topMode) {
    HeadControlUnit::topMode = topMode;
}


HeadMotor* HeadControlUnit::motor[2] = {nullptr};
bool HeadControlUnit::topMode = false;
HeadControlUnit::HeadControlSignal HeadControlUnit::signal = {0,0};
Thread<HeadControlThread>* headTread = nullptr;

void RoboFramework::HeadControlUnit::Init() {
    signal.pitch_ch = 0;
    signal.yaw_ch = 0;
    for(int i = 0;i<2;i++){
        motor[i] = new HeadMotor(i+1);
    }
    headTread = new Thread<HeadControlThread>("head",512);
    headTread->Login();
}


void RoboFramework::HeadControlThread::start() {
    short angle = 0;
    short speed = 0;
    for (float t = 0;; Delay::ms(3), t += 0.1) {
        auto angleStruct = HeadControlUnit::getSignal();
        angle = angleStruct->pitch_ch;
        //todo speed
        if (HeadControlUnit::isTopMode()) {
            HeadControlUnit::getMotor()[0]->run(speed);
        }else{
            HeadControlUnit::getMotor()[0]->run(0);
        }

        HeadControlUnit::getMotor()[1]->run_angle(angle);

        NOP();
    }
}
