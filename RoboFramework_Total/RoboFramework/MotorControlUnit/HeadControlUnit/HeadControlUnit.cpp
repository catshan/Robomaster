//
// Created by Administrator on 2019/11/19.
//

#include "HeadControlUnit.h"
using namespace RoboFramework;
void RoboFramework::HeadMotor::run(float target) {
    s_pid->setTarget(target);
    short in = getMotor()->getSpeed();
    short out = s_pid->calculate(in);
    sendData(out);

}

void RoboFramework::HeadMotor::run_angle(float target) {
    a_pid->setTarget(target);
    short in = getMotor()->getAngle();
    short out = a_pid->calculate(in);
    sendData(out);
}

RoboFramework::HeadControlUnit::HeadControlSignal *RoboFramework::HeadControlUnit::getSignal() {
    return &signal;
}


bool RoboFramework::HeadControlUnit::isTopMode() {
    return topMode;
}

void RoboFramework::HeadControlUnit::setTopMode(bool topMode) {
    HeadControlUnit::topMode = topMode;
}


HeadMotor* HeadControlUnit::motor_Yaw = nullptr;
HeadMotor* HeadControlUnit::motor_Pitch = nullptr;
Kalman* KalmanYaw = nullptr;
Kalman* KalmanPitch = nullptr;
Kalman* KalmanTargetYaw = nullptr;
Kalman* KalmanTargetPitch = nullptr;
bool HeadControlUnit::topMode = false;
HeadControlUnit::HeadControlSignal HeadControlUnit::signal = {0,0};
Thread<HeadControlThread>* headTread = nullptr;

void RoboFramework::HeadControlUnit::Init() {
    signal.pitch_ch = 0;
    signal.yaw_ch = 0;
    motor_Yaw = new HeadMotor(1);
    motor_Pitch = new HeadMotor(2);
    KalmanYaw = new Kalman();
    KalmanPitch = new Kalman();
    KalmanTargetYaw = new Kalman();
    KalmanTargetPitch = new Kalman();

    KalmanYaw->setQangle(0.1);
    KalmanTargetYaw->setQangle(0.1);
    KalmanTargetYaw->setAngle(HOLD_YAW_ANGLE_MID);

    KalmanPitch->setQangle(0.03);
    KalmanTargetPitch->setQangle(0.03);
    headTread = new Thread<HeadControlThread>("head",512);
    headTread->Login();
}

HeadMotor *HeadControlUnit::getMotorYaw() {
    return motor_Yaw;
}

HeadMotor *HeadControlUnit::getMotorPitch() {
    return motor_Pitch;
}


MPU6500* mpu6500 = nullptr;
void RoboFramework::HeadControlThread::start() {
    mpu6500->Update();
    short angle = 0;
    short speed = 0;

    for (float t = 0;; Delay::ms(3), t += 0.1) {


        auto angleStruct = HeadControlUnit::getSignal();
        angle = angleStruct->pitch_ch;
        //todo speed
        if (HeadControlUnit::isTopMode()) {
            //todo YawSpeed with chassis
            HeadControlUnit::getMotorYaw()->run(speed);
        }else{
            HeadControlUnit::getMotorYaw()->run(0);
        }


        if(angle > HOLD_PITCH_ANGLE_MAX)
        {
            angle = HOLD_PITCH_ANGLE_MAX;
        }
        if(angle < HOLD_PITCH_ANGLE_MIN )
        {
            angle = HOLD_PITCH_ANGLE_MIN;
        }


        /*TODO  如果当前位置等于或超过极限位置,不允许继续前进*/
        if(HeadControlUnit::getMotorPitch()->getMotor()->getAngle() >= HOLD_PITCH_ANGLE_MAX){
            //todo get pitchAngle from MPU6500 or MotorPitch ?
            HeadControlUnit::getMotorPitch()->run(0);
        }
        if(HeadControlUnit::getMotorPitch()->getMotor()->getAngle() <= HOLD_PITCH_ANGLE_MIN){
            HeadControlUnit::getMotorPitch()->run(0);
        }

        HeadControlUnit::getMotorPitch()->run_angle(angle);


        NOP();
    }
}
