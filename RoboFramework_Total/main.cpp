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

    for(;;Delay::ms(1)){
        IOPort led1 = Gpio::Register("PG1");
        IOPort led2 = Gpio::Register("PG2");
        IOPort led3 = Gpio::Register("PG3");
        IOPort led4 = Gpio::Register("PG4");
        IOPort led5 = Gpio::Register("PG5");
        IOPort led6 = Gpio::Register("PG6");
        IOPort led7 = Gpio::Register("PG7");
        IOPort led8 = Gpio::Register("PG8");
    }
}