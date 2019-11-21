//
// Created by Administrator on 2019/11/19.
//

#ifndef ROBOFRAMEWORK_TOTAL_HEADCONTROLUNIT_H
#define ROBOFRAMEWORK_TOTAL_HEADCONTROLUNIT_H

#include "GM6020.h"
#include "stm32el.h"
#include "PID.h"
#include "MotorNode.h"
#include "Kalman.h"
#include "MPU6500.h"
#define HOLD_PITCH_ANGLE_MAX 105
#define HOLD_PITCH_ANGLE_MID 95
#define HOLD_PITCH_ANGLE_MIN 75

#define HOLD_YAW_ANGLE_MAX 345
#define HOLD_YAW_ANGLE_MID 270
#define HOLD_YAW_ANGLE_MIN 190

namespace RoboFramework{

class HeadMotor:public MotorNode<GM6020>{
private:
    PID* s_pid = new PID(20000,20000,0,0,3500,110,500);//速度环
    PID* a_pid = new PID(100,0,0,0,0.07,0,0);//位置环
//    PID* p_speed_pid = new PID(20000,20000,0,0,2000,110,500);
//    PID* p_angle_pid = new PID(100,0,0,0,0.07,0,0);
public:
    using MotorNode::MotorNode;

    void run(float target) override;

    void run_angle(float target);
};

class HeadControlUnit{
private:
    static struct HeadControlSignal{
        int yaw_ch;
        int pitch_ch;//俯仰角 motor[1]
    }signal;
    static HeadMotor* motor_Yaw;
    static HeadMotor* motor_Pitch;
public:
    static HeadMotor *getMotorYaw();

    static HeadMotor *getMotorPitch();

private:
    static bool topMode;
    static bool busy;
public:
    static bool isBusy();

    static void setBusy(bool busy);

public:
    static HeadControlSignal *getSignal();

    static bool isTopMode();
    static void setTopMode(bool topMode);
public:
    static void Init();
};


class HeadControlThread:public ThreadTask{
public:
    void start() override;
};

}


#endif //ROBOFRAMEWORK_TOTAL_HEADCONTROLUNIT_H
