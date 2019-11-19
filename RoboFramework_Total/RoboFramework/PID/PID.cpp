//
// Created by Administrator on 2019/11/15.
//

#include "PID.h"
#define ABS(x) ((x>0) ? x : -x)
void PID::setIntegralLimit(float integralLimit) {
    IntegralLimit = integralLimit;
}



void PID::setMaxOutput(float maxOutput) {
    MaxOutput = maxOutput;
}

void PID::setTarget(float target) {
    PID::target = target;
}

void PID::setMeasure(float measure) {
    PID::measure = measure;
}

float PID::getTarget() const {
    return target;
}

float PID::getMaxOutput() const {
    return MaxOutput;
}

float PID::getOutput() const {
    return output;
}

float PID::getKp() const {
    return kp;
}

float PID::getKi() const {
    return ki;
}

float PID::getKd() const {
    return kd;
}

void PID::clear() {
    target = 0;
    output = 0;
    p_out = 0;
    i_out = 0;
    d_out = 0;
    measure = 0;
}

PID::PID(u16 MaxOutput, u16 IntegralLimit, float deadband, int target, float kp, float ki, float kd) {
    this->DeadBand = deadband;
    this->IntegralLimit = IntegralLimit;
    this->MaxOutput = MaxOutput;
    this->target = target;
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->output = 0;
    this->p_out = 0;
    this->i_out = 0;
    this->d_out = 0;

}

void PID::reset(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float PID::calculate(float measure) {
    this->measure = measure;
    this->last_err = err;
    this->last_output = output;
    this->err = this->target - this->measure;
    //是否进入死区
    if(ABS(this->err)>this->DeadBand){
        this->p_out = this->kp * this->err;
        this->i_out = this->ki * this->err;
        this->d_out = this->kd * (this->err - this->last_err);
    }
    //积分是否超乎限制
    if(this->i_out > this->IntegralLimit){
        this->i_out = this->IntegralLimit;
    }
    if(this->i_out < -this->IntegralLimit){
        this->i_out = -this->IntegralLimit;
    }
    //this输出和
    this->output = this->p_out + this->i_out + this->d_out;
    //this->output  = this->output * 0.7f + this->last_output *0.3f; 滤波?
    if(this->output > this->MaxOutput){
        this->output = this->MaxOutput;
    }
    if(this->output < -(this->MaxOutput)){
        this->output = -(this->MaxOutput);
    }
    return this->output;
}
