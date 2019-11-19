//
// Created by Administrator on 2019/11/18.
//
#include "stm32el.h"
#include "MotorBase.h"
#ifndef ROBOFRAMEWORK_TOTAL_GM6020_H
#define ROBOFRAMEWORK_TOTAL_GM6020_H
namespace RoboFramework{
class GM6020:public MotorBase{
public:
    void upData(u8 *data) override {
        angle = ((short)data[0]<<8) + data[1];
        speed = ((short)data[2]<<8) + data[3];
        current = ((short)data[4]<<8) + data[5];
        temperature = data[6];
    }

    void deCodeIndex(u8 index) override {
        rxID = 0x200 + index;
        txID = index > 4?0x2ff:0x1ff;
        index--;
        index %= 4;
        sendDataIndex = index*2;
    }
};


}
#endif //ROBOFRAMEWORK_TOTAL_GM6020_H
