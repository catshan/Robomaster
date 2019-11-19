//
// Created by Administrator on 2019/11/18.
//

#ifndef ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H
#define ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H
#include "stm32el.h"
#include "PID.h"
#include "MotorNode.h"
#include "M2006.h"
#define SHOOT_SPEED 1000


namespace RoboFramework{
class ShootMotor :public MotorNode<M2006>{
private:
    PID* pid = new PID(1000,1000,0,0,2,0,0);
public:
    using MotorNode::MotorNode;

    void run(float target) override;
};


class ShootControlUnit{
private:
    static ShootMotor* motor;
    static bool shootMode;
public:
    static bool isShootMode();

public:
    static void setShootMode(bool shootMode);

public:
    static ShootMotor *getMotor();

    static void Init();
};


class ShootControlThread:public ThreadTask{
public:
    void start() override;
};
}


#endif //ROBOFRAMEWORK_TOTAL_SHOOTCONTROLUNIT_H
