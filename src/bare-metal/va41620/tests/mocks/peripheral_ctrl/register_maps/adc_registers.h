#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace ADC_CTRL {
    using CHAN_EN = RegSubValue<0, 16>;
    using CHAN_TAG_EN = RegSubValue<16>;
    using SWEEP_EN = RegSubValue<17>;
    using EXT_TRIG_EN = RegSubValue<18>;
    using MANUAL_TRIG = RegSubValue<19>;
    using CONV_CNT = RegSubValue<20,4>;
}

namespace ADC_STATUS {
    using FIFO_ENTRY_CNT = RegSubValue<0, 6>;
    using ADC_BUSY = RegSubValue<7>;
}

struct ADCStruct {
    Register CTRL;
    Register FIFO_DATA;
    Register STATUS;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register IRQ_CLR;
    Register RXFIFOIRQTRG;
    Register FIFO_CLR;
    Register PERID;
};


}

