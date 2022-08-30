/**
 * PID controller implementation.
 * 
 * Note on 'derivative-on-measurement': Since the 'error signal' effectively going into the differentiator 
 * does not depend on the setpoint: e[n] = 0 - measurement, and therefore 
 * (e[n] - e[n - 1]) = (0 - measurement) - (0 - prevMeasurement) = -Kd * (measurement - prevMeasurement). 
 * (Note the minus sign compared to derivative-on-error!) 
 * I've included the minus sign in the code, so gains will have the effect as normal.
 * 
 * Adapted from: https://github.com/pms67/PID
 * 
 *
 * Copyright (c) 2020 Philip Salmony
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 */

//#ifndef PID_CONTROLLER_H
//#define PID_CONTROLLER_H

typedef struct {

	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	
	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;

} PIDController;

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);

//#endif

