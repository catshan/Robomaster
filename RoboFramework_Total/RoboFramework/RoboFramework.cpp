//
// Created by Administrator on 2019/11/17.
//

#include <RemoteUnit/RemoteUnit.h>
#include <ShootControlUnit.h>
#include "RoboFramework.h"
#include "HeadControlUnit.h"
using namespace RoboFramework;
Thread<RemoteUnitThread>* remoteTask = nullptr;
Thread<CanBusThread>* canBusThread = nullptr;

void RemoteUnitInit(){
    remoteTask = new Thread<RemoteUnitThread>("RemoteUnit",256);
}

void RemoteTaskLogin(){
    remoteTask->Login();
}


void CanBusUnitInit(){
    CanBusUnit::Init(1000000,1000000);
    canBusThread = new Thread<CanBusThread>("CanBus",1024);
}

void CanBusTaskLogin(){
    canBusThread->Login();
}
void RemoteControl::OnGetDataReady(RemoteData &data) {
    if(!IsOnline(data)){
        ChassisControlUnit::GetControlSignal()->left_right_ch = 0;
        ChassisControlUnit::GetControlSignal()->forward_back_ch = 0;
        ChassisControlUnit::GetControlSignal()->rotate_ch = 0;
        return;
    }

    if(data.rc.s1 == 1){
        ChassisControlUnit::GetControlSignal()->rotate_ch = 2*CHASSIS_TOP_MAX;
        ChassisControlUnit::GetControlSignal()->left_right_ch = (data.rc.ch0 - REMOTE_MID_VAL)/CAHSSIS_TOP_BASE;
        ChassisControlUnit::GetControlSignal()->forward_back_ch = (data.rc.ch1 - REMOTE_MID_VAL)/CAHSSIS_TOP_BASE;
    } else{
        ChassisControlUnit::GetControlSignal()->rotate_ch = (data.rc.ch2 - REMOTE_MID_VAL)/CHASSIS_SPEED_BASE;
        ChassisControlUnit::GetControlSignal()->left_right_ch = (data.rc.ch0 - REMOTE_MID_VAL)/CHASSIS_SPEED_BASE;
        ChassisControlUnit::GetControlSignal()->forward_back_ch = (data.rc.ch1 - REMOTE_MID_VAL)/CHASSIS_SPEED_BASE;
    }

//    HeadControlUnit::getSignal()->pitch_ch = (data.rc.ch3 - HOLD_PITCH_ANGLE_MID);
//    //HeadControlUnit::getSignal()->pitch_ch = (data.mouse.y - HOLD_PITCH_ANGLE_MID);
//    HeadControlUnit::getSignal()->yaw_ch = (data.mouse.x - HOLD_YAW_ANGLE_MID);
//    HeadControlUnit::setTopMode(data.rc.s1==1);
    ChassisControlUnit::setTopMode(data.rc.s1==1);
    ShootControlUnit::setOpenMode(data.rc.s1==2);
    ShootControlUnit::setShootMode(data.rc.s2==1);
    ShootControlUnit::setSingleMode(data.rc.s2==2);
    ShootControlUnit::setStopMode(data.rc.s2==3);


}
