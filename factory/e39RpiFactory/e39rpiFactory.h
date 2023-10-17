//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_E39RPIFACTORY_H
#define PICOTEMPLATE_E39RPIFACTORY_H

#include <factory/ApplicationContainer.h>

namespace pico {
    namespace di {

        class e39rpiFactory {

        public:
            ApplicationContainer getApplicationContainer();

            //Clean up all the memory
            void deallocateApplicationContainer();
        };

    } // pico
} // di

#endif //PICOTEMPLATE_E39RPIFACTORY_H
