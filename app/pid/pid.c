/**
 * @file pid.c
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief PID controller and other helper functions
 * @version 0.1
 * @date 2024-09-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <errno.h>
#include "pid.h"

uint16_t to_rpm(uint64_t period_ns, uint8_t steps)
{
    if(!period_ns | !steps)
        return 0;

    uint64_t period_us = period_ns / 1000UL;
    uint64_t tmp = (60 * 1000000UL) / (period_us * steps);
    return (uint16_t)tmp;
}

double calculate(struct pid_params *param, double setpoint, double pv)
{
    if(!param  | !param->dt)
        return -EFAULT;

    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = param->kp * error;

    // Integral term
    param->_integral += error * param->dt;
    double Iout = param->ki * param->_integral;

    // Derivative term
    double derivative = (error - param->_pre_error) / param->dt;
    double Dout = param->kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > param->max )
        output = param->max;
    else if( output < param->min )
        output = param->min;

    // Save error to previous error
    param->_pre_error = error;

    return output;
}