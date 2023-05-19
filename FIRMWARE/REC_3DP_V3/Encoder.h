#ifndef ENCODER_H
#define ENCODER_H

#include "REC_3DP_Config.h"

namespace Encoder {
    void Init();
    void encoder_isr();

    extern int enc_counter;
    extern int enc_last;
}

#endif
