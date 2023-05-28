#ifndef TEMPERATURA_H
#define TEMPERATURA_H

namespace Temperature {
    void Init_Timer3();
    void Init();
    float analog2temp();
    void manage_heater();

    extern int maxTempAllowed;

    extern float tempValue;
    extern float lastTempValue;
    extern int adcValue;
    extern int targetTemp;

    extern float Kp;
    extern float Ki;
    extern float Kd;
    extern float pid_input;
    extern float pid_output;
    extern float error;
    extern float proportional;
    extern float last_error;
    extern float derivative;
    extern float derivative_error;
    extern float integral;
    extern float integral_error;

    extern long currMillis_Temperature;
}  // namespace Temperature

#endif
