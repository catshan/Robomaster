//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_MOTORBASE_H
#define ROBOFRAMEWORK_TOTAL_MOTORBASE_H

#include "stm32el.h"
namespace RoboFramework{
    class MotorBase{
    protected:
        short speed = 0;
        short angle = 0;
        short current = 0;
        u8 temperature = 0;
    public:
        short getSpeed() const {
            return speed;
        }

        short getAngle() const {
            return angle;
        }

        short getCurrent() const {
            return current;
        }

        u8 getTemperature() const {
            return temperature;
        }

        u16 getTxId() const {
            return txID;
        }

        u16 getRxId() const {
            return rxID;
        }

        u8 getSendDataIndex() const {
            return sendDataIndex;
        }

    public:
        virtual void upData(u8* data) = 0;
        virtual void deCodeIndex(u8 index) = 0;
    protected:

        u16 txID = 0;
        u16 rxID = 0;
        u8 sendDataIndex = 0;
    };
}
#endif //ROBOFRAMEWORK_TOTAL_MOTORBASE_H
