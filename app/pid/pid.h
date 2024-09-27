#ifndef PID_H
#define PID_H
/**
 * @file pid.h
 * @author Arash Golgol (arash.golgol@gmail.com)
 * @brief PID controller and other helper functions
 * @version 0.1
 * @date 2024-09-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Convert opto-coupler pulse duration into RPM
 * 
 * @param period_ns Period of single pulse in nS
 * @param steps Number of steps per each cycle of wheel
 * @return uint16_t Wheel duration in Round Per Minute
 */
uint16_t to_rpm(uint64_t period_ns, uint8_t steps);

struct pid_params {
    float kp;           // proportional gain
    float ki;           // integral gain
    float kd;           // differential gain
    float dt;           // loop interval time
    float min;          // minimum value of manipulated variable
    float max;          // maximum value of manipulated variable

    float _pre_error;   // private: accumulated error
    float _integral;    // private: integral value
};

/**
 * @brief Returns the manipulated variable given a setpoint 
 *        and current process value
 * 
 * @param param PID controller parameters
 * @param setpoint Target value
 * @param pv Current proccess value
 * @return double 
 */
double calculate(struct pid_params *param, double setpoint, double pv);

#endif // PID_H