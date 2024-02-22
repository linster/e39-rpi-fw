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


    //Call this from sub-classes
    void BaseScanProgram::init(
            std::shared_ptr<pico::logger::BaseLogger> logger
            ) {

        this->logger = logger;
        critical_section_init(&renderCs);
    }

    void BaseScanProgram::startScanProgram() {
        logger->d(getTag(), "Starting ScanProgram");
        onScanProgramStart();
        logger->d(getTag(), "Completed onScanProgramStart()");
    }

    void BaseScanProgram::stopScanProgram() {
        logger->d(getTag(), "Stopping ScanProgram");
        onScanProgramStop();
        logger->d(getTag(), "Stopped ScanProgram");
    }

    void BaseScanProgram::cpu0setup() {
    }

    void BaseScanProgram::onCpu0Loop() {

    }

    void BaseScanProgram::cpu1Setup() {

    }

    void BaseScanProgram::onCpu1Loop() {
        callRender();
    }

    scanvideo_mode_t BaseScanProgram::getScanVideoMode() {
        return mode_bmbt;
    }

    scanvideo_timing_t BaseScanProgram::getScanProgramTiming() {
        return vga_timing_bmbt;
    }

    uint16_t BaseScanProgram::getDisplayHeightPx() {
        return getScanVideoMode().height; //Vertical is height
    }

    uint16_t BaseScanProgram::getDisplayWidthPx() {
        return getScanVideoMode().width; //Horizontal is width
    }

    void BaseScanProgram::callRender() {
        scanvideo_scanline_buffer_t *scanlineBuffer = scanvideo_begin_scanline_generation(true);
        critical_section_enter_blocking(&renderCs);
        render(scanlineBuffer);
        critical_section_exit(&renderCs);
        scanvideo_end_scanline_generation(scanlineBuffer);
//        critical_section_exit(&renderCs);
    }


} // scanPrograms