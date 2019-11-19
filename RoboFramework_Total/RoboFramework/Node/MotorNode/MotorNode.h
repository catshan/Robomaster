//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_MOTORNODE_H
#define ROBOFRAMEWORK_TOTAL_MOTORNODE_H

#include "stm32el.h"
#include "PID.h"
#include "CanBusUnit.h"
#include "CanNode.h"
#include "MotorBase.h"
namespace RoboFramework{
template <class M>
class MotorNode:public CanNode{
private:
    MotorBase* motor;
public:
    MotorBase *getMotor() const ;

public:
    MotorNode(u8 index);
    void sendData(short data);
    virtual void run(float target) = 0;
    void OnUpData() override ;
};

template<class M>MotorNode<M>::MotorNode(u8 index) {
    motor = (MotorBase*)new M;
    motor->deCodeIndex(index);
    sendID = motor->getTxId();
    listenID = motor->getRxId();
    sendIndexStart = motor->getSendDataIndex();
    CanBusUnit::AddNode(CanType ::Can1,this);
}

template<class M>void MotorNode<M>::sendData(short data) {
txBuffer[0] = data>>8;
txBuffer[1] = data&0xff;
}

template<class M>void MotorNode<M>::OnUpData() {
    motor->upData(rxBuffer);
}

template<class M>
MotorBase *MotorNode<M>::getMotor() const {
    return motor;
}


}



#endif //ROBOFRAMEWORK_TOTAL_MOTORNODE_H
