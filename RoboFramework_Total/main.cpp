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
    //HeadControlUnit::Init();
//PWM<Tim2 >pwm(20,PWM_Channel::CH1,"PA0");
//pwm.run(1);
    RemoteTaskLogin();
    CanBusTaskLogin();
    IOPort led = Gpio::Register("PG13");
    led = 1;
    for(;;Delay::ms(1000)){
    }
}