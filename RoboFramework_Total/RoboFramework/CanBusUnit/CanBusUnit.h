//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_CANBUSUNIT_H
#define ROBOFRAMEWORK_TOTAL_CANBUSUNIT_H

#include "stm32el.h"
#include "vector"
#include "CanNode.h"

namespace RoboFramework{
enum class CanType{
    Can1,Can2
};
class CanBusUnit{
private:
    static CanBus<Can1 >*canBus1;
    static CanBus<Can2 >*canBus2;
    static std::vector<CanNode*>*can1NodeList;
    static std::vector<CanNode*>*can2NodeList;
    static std::vector<u16>*sendIDList;
public:
    static CanBus<Can1> *getCanBus1();

    static CanBus<Can2> *getCanBus2();

    static std::vector<CanNode *> *getCan1NodeList();

    static std::vector<CanNode *> *getCan2NodeList();

    static std::vector<u16> *getSendIdList();

public:
    static void Init(u32 Can1Band,u32 Can2Band);
    static void AddNode(CanType type,CanNode* node);
};
class CanBusThread:public ThreadTask{
public:
    void start() override;
};
}


#endif //ROBOFRAMEWORK_TOTAL_CANBUSUNIT_H
