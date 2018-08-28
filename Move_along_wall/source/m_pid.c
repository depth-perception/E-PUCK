#include "m_pid.h"

double kp = 1, ki = 1, kd = 1;
double error = 0, errSum = 0, dErr = 0, lastErr = 0;
double output = 0;
int sampleTime = 1;

double m_pid_compute(double error)
{
    
}
void m_pid_set(double Kp,double Ki,double Kd)
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}