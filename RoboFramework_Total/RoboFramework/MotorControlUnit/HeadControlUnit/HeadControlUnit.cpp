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

short HeadMotor::PID_speed_out(float target,short current) {
    s_pid->setTarget(target);
    return s_pid->calculate(current);
}

short HeadMotor::PID_angle_out(float target) {
    a_pid->setTarget(target);
    return a_pid->calculate(getMotor()->getAngle());
}

void HeadMotor::sendPID(short Speed_or_Angle) {
    sendData(Speed_or_Angle);
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
MPU6500* mpu6500 = nullptr;
bool HeadControlUnit::topMode = false;
bool HeadControlUnit::busy = false;
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

    if(!HeadControlUnit::isBusy()){
        float yaw = KalmanYaw->getAngle(motor_Yaw->getMotor()->getAngle(),mpu6500->GetYawAngle(),0.001); //todo
        float pitch = KalmanPitch->getAngle(motor_Pitch->getMotor()->getAngle(),mpu6500->GetPitchAngle(),0.001);
        if(yaw <= 0){
            yaw += 360;
        }
        if(pitch <= 0){
            pitch += 360;
        }
        motor_Yaw->getMotor()->setAngle(yaw);
        motor_Pitch->getMotor()->setAngle(pitch);
    }

    HeadControlUnit::RunYaw(HOLD_YAW_ANGLE_MID);
    HeadControlUnit::RunPitch(HOLD_PITCH_ANGLE_MID);

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

bool HeadControlUnit::isBusy() {
    return busy;
}

void HeadControlUnit::setBusy(bool busy) {
    HeadControlUnit::busy = busy;
}

void HeadControlUnit::RunYaw(short angle_Yaw) {
    /*RunYaw */
    if(angle_Yaw > HOLD_YAW_ANGLE_MAX)
    {
        angle_Yaw = HOLD_YAW_ANGLE_MAX;
    }
    if(angle_Yaw < HOLD_YAW_ANGLE_MIN )
    {
        angle_Yaw = HOLD_YAW_ANGLE_MIN;
    }

    /*计算位置环PID 输出目标速度*/
    short targetSpeed_Yaw = 0;
    short currentSpeed_Yaw = 0;
    short currentAngle_Yaw = 0;
    if(!HeadControlUnit::isBusy()){
        currentSpeed_Yaw = mpu6500->GetYawAngle();
    }else{
        //陀螺仪数据获取失败
        return;
    }
    currentAngle_Yaw = HeadControlUnit::getMotorYaw()->getMotor()->getAngle();
    if(currentAngle_Yaw <=0){
        currentAngle_Yaw += 360;
        HeadControlUnit::getMotorYaw()->getMotor()->setAngle(currentAngle_Yaw);
    }

    if (HeadControlUnit::isTopMode()) {
        targetSpeed_Yaw = HeadControlUnit::getMotorYaw()->PID_angle_out(angle_Yaw);
        short chassisSpeed = -ChassisControlUnit::GetRevolveSpeed()/19/60.0;
        if(ChassisControlUnit::isRevolve()){
            currentSpeed_Yaw -= chassisSpeed; //与底盘陀螺仪做速度补偿
        }
    }
    /*  如果当前位置等于或超过极限位置,不允许继续前进*/
    short targetCurrent_Yaw = HeadControlUnit::getMotorYaw()->PID_speed_out(targetSpeed_Yaw,currentSpeed_Yaw);
    if(HeadControlUnit::getMotorYaw()->getMotor()->getAngle() >= HOLD_YAW_ANGLE_MAX){
        if (targetCurrent_Yaw > 0){
            targetCurrent_Yaw = 0;
        }
    }else if(HeadControlUnit::getMotorYaw()->getMotor()->getAngle() <= HOLD_YAW_ANGLE_MID){
        if(targetCurrent_Yaw < 0){
            targetCurrent_Yaw = 0;
        }
    }
    HeadControlUnit::getMotorYaw()->sendPID(targetCurrent_Yaw);

}

void HeadControlUnit::RunPitch(short angle_Pitch) {
    /*RunPitch*/
    if(angle_Pitch > HOLD_PITCH_ANGLE_MAX)
    {
        angle_Pitch = HOLD_PITCH_ANGLE_MAX;
    }
    if(angle_Pitch < HOLD_PITCH_ANGLE_MIN )
    {
        angle_Pitch = HOLD_PITCH_ANGLE_MIN;
    }

    short targetSpeed_Pitch = HeadControlUnit::getMotorPitch()->PID_angle_out(angle_Pitch);
    short currentSpeed_Pitch = 0;
    if(!HeadControlUnit::isBusy()){
        currentSpeed_Pitch = mpu6500->GetPitchAngle();
    }else{
        //陀螺仪数据获取失败
        return;
    }
    /*  如果当前位置等于或超过极限位置,不允许继续前进*/
    short targetCurrent_Pitch = HeadControlUnit::getMotorPitch()->PID_speed_out(targetSpeed_Pitch,currentSpeed_Pitch);
    if(HeadControlUnit::getMotorPitch()->getMotor()->getAngle() >= HOLD_PITCH_ANGLE_MAX){
        if(targetCurrent_Pitch > 0){
            targetCurrent_Pitch = 0;
        }
    }
    if(HeadControlUnit::getMotorPitch()->getMotor()->getAngle() <= HOLD_PITCH_ANGLE_MIN){
        if(targetCurrent_Pitch < 0){
            targetCurrent_Pitch = 0;
        }
    }

    HeadControlUnit::getMotorPitch()->sendPID(targetCurrent_Pitch);
}


void RoboFramework::HeadControlThread::start() {
    short angle_Yaw = 0;
    short angle_Pitch = 0;
    for (;; Delay::ms(3)) {
        HeadControlUnit::setBusy(true);
        mpu6500->Update();
        HeadControlUnit::setBusy(false);
        auto angleStruct = HeadControlUnit::getSignal();
        //todo angle
        angle_Pitch += angleStruct->pitch_ch *0.2;
        if(HeadControlUnit::isTopMode()){
            angle_Yaw -= angleStruct->yaw_ch * 0.5;
        }else{
            angle_Yaw = HOLD_YAW_ANGLE_MID;
        }
        HeadControlUnit::RunYaw(angle_Yaw);
        HeadControlUnit::RunPitch(angle_Pitch);
        NOP();
    }
}
