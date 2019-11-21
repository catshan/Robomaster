//
// Created by Administrator on 2019/11/17.
//

#include "ChassisControlUnit.h"
using namespace RoboFramework;
void RoboFramework::ChassisMotor::run(float target) {
    pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = pid->calculate(in);
    sendData(out);
}

bool RoboFramework::ChassisControlUnit::isTopMode() {
    return topMode;
}

void RoboFramework::ChassisControlUnit::setTopMode(bool topMode) {
    ChassisControlUnit::topMode = topMode;
}

RoboFramework::ChassisMotor **RoboFramework::ChassisControlUnit::getMotor() {
    return motor;
}

Thread<ChassisControlThread>* chassisThread = nullptr;
ChassisControlUnit::ChassisControlSignal ChassisControlUnit::signal = {0,0,0};
ChassisMotor* ChassisControlUnit::motor[4] = {nullptr};
bool ChassisControlUnit::topMode = false;
void RoboFramework::ChassisControlUnit::Init() {
    signal.forward_back_ch = 0;
    signal.left_right_ch = 0;
    signal.rotate_ch = 0;
    for (int i = 0; i <4 ; i++) {
        motor[i] = new ChassisMotor(i+1);
    }
    chassisThread = new Thread<ChassisControlThread>("Chassis",512);
    chassisThread->Login();
}

ChassisControlUnit::ChassisControlSignal *ChassisControlUnit::GetControlSignal() {
    return &signal;
}
auto speedStruct = *ChassisControlUnit::GetControlSignal();
int ChassisControlUnit::GetRevolveSpeed() { return speedStruct.rotate_ch; }
void RoboFramework::ChassisControlThread::start() {
    short inVal[4];
    u8 index1 = 0,index2 = 3;
    for(float t = 0;;Delay::ms(3),t+=0.1){

        inVal[0] = -speedStruct.forward_back_ch + speedStruct.left_right_ch +
                   speedStruct.rotate_ch;
        inVal[1] = -speedStruct.forward_back_ch - speedStruct.left_right_ch +
                   speedStruct.rotate_ch;
        inVal[2] = speedStruct.forward_back_ch + speedStruct.left_right_ch +
                   speedStruct.rotate_ch;
        inVal[3] = speedStruct.forward_back_ch - speedStruct.left_right_ch +
                   speedStruct.rotate_ch;

        if(ChassisControlUnit::isTopMode()){
            inVal[index1] += 3000;
            inVal[index2] += 3000;
        }

        for(int i=0;i<4;i++){
            ChassisControlUnit::getMotor()[i]->run(inVal[i]);
        }
        NOP();

    }
}
