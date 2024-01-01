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
         *      +--------+                  +--------+       +---------+
         *      | Laptop |<==(usb stdio)===>|  Pico  |<=====>| MCP2020 | (UART0, 9600-8E1, IBUS raw)
         *      +--------+                  +--------+       +---------+
         *                                      ^^
         *                                      ||<......................(UART1, 9600-8E1, IBUS raw)
         *                                      vv
         *                                  +--------+
         *                                  |  Rpi  |
         *                                  +-------+
         *
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
         *                                  |  Rpi  |
         *                                  +-------+
         *
         */
         SLED_NO_PI

        //Then, we make a TopologyManager that knows how to set up each pin and uart
        //Then, we put that inside the dmaManager.

        //Then, we gotta figure out how to use the ToplogyManager with the ObserverRegistry to dispatch packets?
        //We pretty much just want to not forward the knob turned packets from the car while the video source isn't set to PI.
        //Other than that, we can forward everything both ways.
    };
}
#endif //PICOTEMPLATE_PICOBUSTOPOLOGY_H
