#include "RemoteUnit.h"
#include "ChassisControlUnit.h"
#include "ShootControlUnit.h"
#include "HeadControlUnit.h"
#include "RoboFramework.h"


void Main(){
    RemoteUnitInit();
    CanBusUnitInit();
    RemoteUnit::BindListener(new RemoteControl());

    ChassisControlUnit::Init();
    ShootControlUnit::Init();
    HeadControlUnit::Init();

    RemoteTaskLogin();
    CanBusTaskLogin();

    for(;;Delay::ms(1)){}
}