#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct
{
    float kp;
    float ki;
    float kd;
    float err;
    float last_err;
    float integral;
    float integral_max;
    float pout;
    float iout;
    float dout;
    float last_dout;
    float out;
    float out_max;
} pid_ctrl_t;

#ifndef constrain
#define constrain(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))//这个可以新建一个math.h去单独放自己需要的函数
#endif

void single_pid_ctrl(pid_ctrl_t *pid);

#endif
