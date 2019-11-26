//
// Created by Administrator on 2019/11/18.
//

#ifndef ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H
#define ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H

#include "stm32el.h"
#include "PID.h"
#include "MotorNode.h"
#include "M2006.h"
#include "M3508.h"
#define TAGGLE_SPEED 2500  //拨弹电机为5
#define SHOOT_SPEED 12000   //发射电机为6,7

namespace RoboFramework{
class ShootMotor :public MotorNode<M3508>{
private:
    PID* pid = new PID(10000,1000,0,0,1.5,0.001,2);
public:
    using MotorNode::MotorNode;

    void run(float target) override;
};
class TaggleMotor:public MotorNode<M2006>{
private:
    PID* pid = new PID(10000,1000,0,0,1.5,0.001,2);
    PID* pid1 = new PID(1000,1000,0,0,8,0.01,1);
public:
    using MotorNode::MotorNode;
    void run(float target) override;
    void run_single(float target);
};
class ShootControlUnit{
private:
    static ShootMotor* motor[2];
    static TaggleMotor* motor_Tag[1];

public:
    static TaggleMotor **getMotorTag();

private:
    static bool shootMode;
    static bool singleMode;
    static bool stopMode;
    static bool openMode;
public:
    static bool isOpenMode();

    static void setOpenMode(bool openMode);

public:
    static bool isStopMode();

    static void setStopMode(bool stopMode);

public:
    static bool isSingleMode();

    static void setSingleMode(bool singleMode);

public:
    static bool isShootMode();

public:
    static void setShootMode(bool shootMode);

public:
    static ShootMotor **getMotor();

    static void Init();
};


class ShootControlThread:public ThreadTask{
public:
    void start() override;
};
}


#endif //ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H
