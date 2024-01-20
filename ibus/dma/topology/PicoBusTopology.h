//
// Created by stefan on 12/31/23.
//

#ifndef PICOTEMPLATE_PICOBUSTOPOLOGY_H
#define PICOTEMPLATE_PICOBUSTOPOLOGY_H

namespace pico::ibus::topology {

    enum BusTopology {

        /**
         *
         *
         *                                  +--------+
         *             ---------swd-------->|  Pico  |
         *            /                     |  Debug |
         *           /                      |  Probe |
         *          |                       +--------+
         *          |                            ^
         *          |                            | (swd)
         *          v                            v
         *      +--------+                     +--------+       +---------+
         *      | Laptop |<-(usb stdio logs)->|  Pico  |<=====>| MCP2020 | (UART0, 9600-8E1, IBUS raw)
         *      +--------+                    +--------+       +---------+
         *                                      ^^
         *                                      ||<........................(UART1, 9600-8E1, IBUS raw)
         *                                      ||
         *                       (AdaFruit 954 Serial <-> USB Adapter)
         *                                      ||
         *                                      vv
         *                                  +--------+
         *                                  |  Rpi  |
         *                                  +-------+
         *
         * We can't use UART1 to talk to the Rpi directly because when the Rpi is in display parallel mode
         * to get VGA-out, there aren't enough GPIO pins we can use (and they're all disabled for SPI
         * too). So, we run another USB cable from the mainboard to the pi.
         */
        CAR_WITH_PI,



        /**
         *                    .....(SWD, 115200-8N1. Debug log + Raw IBUS)
         *                   .
         *                  .               +--------+
         *             ====================>|  Pico  |
         *            //                    |  Debug |
         *           //                     |  Probe |
         *          ||                      +--------+
         *          ||                           ^^
         *          ||                           ||<.....................(UART1, 115200-8N1. Debug log + Raw IBUS)
         *          vv                           vv
         *      +--------+                  +--------+       +---------+
         *      | Laptop |<-(usb pwr only)->|  Pico  |<=====>| MCP2020 | (UART0, 9600-8E1 IBUS raw)
         *      +--------+                  +--------+       +---------+
         *
         *                                  +--------+
         *                                  |  Rpi   |
         *                                  +--------+
         *
         */
         SLED_NO_PI
    };
}
#endif //PICOTEMPLATE_PICOBUSTOPOLOGY_H
