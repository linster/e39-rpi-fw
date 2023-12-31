//
// Created by stefan on 12/31/23.
//

#ifndef PICOTEMPLATE_PICOBUSTOPOLOGY_H
#define PICOTEMPLATE_PICOBUSTOPOLOGY_H

namespace pico::ibus::topology {

    enum BusTopology {

        //Take the piece of paper with all the pico bus topologies and ascii-art them in here.

        //Then, we make a TopologyManager that knows how to set up each pin and uart
        //Then, we put that inside the dmaManager.

        //Then, we gotta figure out how to use the ToplogyManager with the ObserverRegistry to dispatch packets?
        //We pretty much just want to not forward the knob turned packets from the car while the video source isn't set to PI.
        //Other than that, we can forward everything both ways.
    };
}
#endif //PICOTEMPLATE_PICOBUSTOPOLOGY_H
