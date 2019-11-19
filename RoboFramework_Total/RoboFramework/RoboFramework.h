//
// Created by Administrator on 2019/11/17.
//

#ifndef ROBOFRAMEWORK_TOTAL_ROBOFRAMEWORK_H
#define ROBOFRAMEWORK_TOTAL_ROBOFRAMEWORK_H

#include "RemoteUnit.h"
#include "ChassisControlUnit.h"
using namespace RoboFramework;
void RemoteUnitInit();
void CanBusUnitInit();
void RemoteTaskLogin();
void CanBusTaskLogin();

namespace RoboFramework{
class RemoteControl:public RemoteListenerInterface{
public:
    void OnGetDataReady(RemoteData &data) override;
};
}


#endif //ROBOFRAMEWORK_TOTAL_ROBOFRAMEWORK_H
