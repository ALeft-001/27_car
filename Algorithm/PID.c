#include "pid.h"

void single_pid_ctrl(pid_ctrl_t *pid)
{
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max, pid->integral_max);

	pid->pout = pid->kp * pid->err;
	pid->iout = pid->ki * pid->integral;
	pid->dout = pid->kd * (pid->err - pid->last_err);
	pid->last_dout = pid->dout;

	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
	pid->last_err = pid->err;
}
