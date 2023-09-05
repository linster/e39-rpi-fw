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


        mutex_init(&this->isScanProgramRunningMutex);


        logger->d(getTag(), "Completed init()");
    }

    void BaseScanProgram::startScanProgram() {
        logger->d(getTag(), "Starting ScanProgram");

        mutex_enter_blocking(&this->isScanProgramRunningMutex);
        scanvideo_timing_enable(true); //TODO make sure this is called from CPU0
        this->isScanProgramRunning = true;
        mutex_exit(&this->isScanProgramRunningMutex);

        logger->d(getTag(), "Started ScanProgram");

        onScanProgramStart();
    }

    void BaseScanProgram::stopScanProgram() {
        logger->d(getTag(), "Stopping ScanProgram");
        onScanProgramStop();
        mutex_enter_blocking(&this->isScanProgramRunningMutex);

        scanvideo_timing_enable(false); //TODO make sure this is called from CPU0


        this->isScanProgramRunning = false;
        mutex_exit(&this->isScanProgramRunningMutex);
        logger->d(getTag(), "Stopped ScanProgram");
    }

    bool BaseScanProgram::shouldKeepRunning() {
        bool ret;
        mutex_enter_blocking(&this->isScanProgramRunningMutex);
        ret = this->isScanProgramRunning;
        mutex_exit(&this->isScanProgramRunningMutex);
        return ret;
    }

    void BaseScanProgram::cpu0setup() {

    }

    void BaseScanProgram::onCpu0Loop() {
        //Tight-loop on CPU0 to ask the scan program to fill the buffer.
        scanvideo_scanline_buffer_t *scanlineBuffer = scanvideo_begin_scanline_generation(true);
        render(scanlineBuffer);
        scanvideo_end_scanline_generation(scanlineBuffer);
    }

    void BaseScanProgram::cpu1Setup() {
        //NOOP
        //We're doing rendering on cpu0. We have LIN interrupts on cpu1.
    }

    void BaseScanProgram::onCpu1Loop() {
        //NOOP
        //We're doing rendering on cpu0. We have LIN interrupts on cpu1.
    }

    scanvideo_mode_t BaseScanProgram::getScanVideoMode() {
        return mode_bmbt;
    }

    scanvideo_timing_t BaseScanProgram::getScanProgramTiming() {
        return vga_timing_bmbt;
    }

    uint16_t BaseScanProgram::getDisplayHeightPx() {
        return getScanProgramTiming().v_active; //Vertical is height
    }

    uint16_t BaseScanProgram::getDisplayWidthPx() {
        return getScanProgramTiming().h_active; //Horizontal is width
    }


} // scanPrograms