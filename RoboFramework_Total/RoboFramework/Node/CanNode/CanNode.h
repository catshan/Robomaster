//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_CANNODE_H
#define ROBOFRAMEWORK_TOTAL_CANNODE_H

#include "stm32el.h"
namespace RoboFramework {

class CanNode{
protected:
    u16 sendID = 0;
    u16 listenID = 0;
    u8 sendIndexStart = 0;
    u8 txBuffer[2] = {0};
    u8 rxBuffer[8] = {0};
public:
    virtual void OnUpData() = 0;
public:
    u16 getSendId() const;

    u16 getListenId() const;

    u8 getSendIndexStart() const;

    const u8 *getTxBuffer() const;

    const u8 *getRxBuffer() const;
};

}
#endif //ROBOFRAMEWORK_TOTAL_CANNODE_H
