//
// Created by stefan on 11/20/22.
//

#include "BaseScanProgram.h"

#include <stdio.h>

#include "pico.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"

namespace video::scanProgram::scanPrograms {


    void BaseScanProgram::init(
            std::shared_ptr<pico::logger::BaseLogger> logger
            ) {

        this->logger = logger;

        mutex_init(&this->isScanProgramRunningMutex);

        logger->d("BaseScanProgram", "Completed init()");
    }


} // scanPrograms