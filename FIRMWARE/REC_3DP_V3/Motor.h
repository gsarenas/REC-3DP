#ifndef MOTOR_H
#define MOTOR_H

namespace Motor {
    void Init_Timer2();
    void Init();
    void run();

    extern int motSpeed;
    extern int motSpeed_;
}

#endif
