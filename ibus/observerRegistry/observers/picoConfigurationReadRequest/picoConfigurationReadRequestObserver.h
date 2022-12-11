//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H
#define PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H

namespace pico {
    namespace ibus {
        namespace observers {

            class picoConfigurationReadRequestObserver {

                //TODO the RPI sends a magic packet asking for our config.
                //TODO we listen for it here, then use it to kick-off a read of the
                //TODO config store, and emit it.
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H
