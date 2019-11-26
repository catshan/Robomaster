//
// Created by Administrator on 2019/11/15.
//

#ifndef ROBOFRAMEWORK_TOTAL_PID_H
#define ROBOFRAMEWORK_TOTAL_PID_H

#include "stm32el.h"

class PID {
protected:
    float target;//目标值
    float kp;
    float ki;
    float kd;

    float measure;//测量值
    float err;//误差值
    float last_err;//上次误差

    float p_out;
    float i_out;
    float d_out;

    float output;//本次输出
    float last_output;//上次输出

    float MaxOutput;//输出限幅
    float IntegralLimit;
public:
    void setIntegralLimit(float integralLimit);

protected:
    float DeadBand;//死区，积分限幅（绝对值）
public:

    void setMaxOutput(float maxOutput);

    void setTarget(float target);

    void setMeasure(float measure);

    float getTarget() const;

    float getMaxOutput() const;

    float getOutput() const;

    float getKp() const;

    float getKi() const;

    float getKd() const;

    void clear();

public:
    PID(u16 MaxOutput,u16 IntegralLimit, float deadband,int target,float kp,float ki, float kd);
    void reset(float kp,float ki,float kd);
    float calculate(float measure);
};


#endif //ROBOFRAMEWORK_TOTAL_PID_H
