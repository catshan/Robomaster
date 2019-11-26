//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_M3508_H
#define ROBOFRAMEWORK_TOTAL_M3508_H

#include "stm32el.h"
#include "MotorBase.h"
namespace RoboFramework{
class M3508:public MotorBase{
public:
    void upData(u8 *data) override {
        angle = ((short)data[0]<<8)+data[1];
        speed = ((short)data[2]<<8)+data[3];
        current = ((short)data[4]<<8)+data[5];
        temperature = data[6];
    }

    void deCodeIndex(u8 index) override {
        rxID = 0x200 + index;
        txID = index>4?0x1ff:0x200;
        index --;
        index %= 4;
        sendDataIndex = index * 2;
    }
};
}
#endif //ROBOFRAMEWORK_TOTAL_M3508_H
