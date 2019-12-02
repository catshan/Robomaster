//
// Created by Administrator on 2019/11/17.
//

#ifndef ROBOFRAMEWORK_TOTAL_CHASSISCONTROLUNIT_H
#define ROBOFRAMEWORK_TOTAL_CHASSISCONTROLUNIT_H

#include "stm32el.h"
#include "PID.h"
#include "MotorNode.h"
#include "M3508.h"

#define CHASSIS_SPEED_MAX 8000
#define CHASSIS_SPEED_BASE (660.0/CHASSIS_SPEED_MAX)
#define CHASSIS_TOP_MAX 1000
#define CAHSSIS_TOP_BASE (660.0/CHASSIS_TOP_MAX)

namespace RoboFramework{

class ChassisMotor:public MotorNode<M3508>{
private:
    PID* pid = new PID(10000,1000,0,0,2,0,0);
public:
    using MotorNode::MotorNode;

    void run(float target) override;
};


class ChassisControlUnit{
private:
    static struct ChassisControlSignal{
        int forward_back_ch;
        int left_right_ch;
        int rotate_ch;
    }signal;
    static ChassisMotor* motor[4];
    static bool topMode;
public:
    static bool isTopMode();
    static void setTopMode(bool topMode);

    static ChassisMotor **getMotor();

public:
    static void Init();

    static ChassisControlSignal* GetControlSignal();
};

class ChassisControlThread:public ThreadTask{
public:
    void start() override;
};
}


#endif //ROBOFRAMEWORK_TOTAL_CHASSISCONTROLUNIT_H
