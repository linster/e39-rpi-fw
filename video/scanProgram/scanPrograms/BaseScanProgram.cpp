//
// Created by stefan on 11/20/22.
//

#include "BaseScanProgram.h"

#include <stdio.h>


namespace video::scanProgram::scanPrograms {


    //Call this from sub-classes
    void BaseScanProgram::init(
            std::shared_ptr<pico::logger::BaseLogger> logger
            ) {

        this->logger = logger;
        //(No critical section here any more. Each one claimed one of the RP2040's 8 non-shared spin
        //locks, and renderCs was no longer used: see callRender.)
    }

    void BaseScanProgram::startScanProgram() {
        logger->d(getTag(), "Starting ScanProgram");
        onScanProgramStart();
        desiredCpu0State.store(STARTED);
        logger->d(getTag(), "Completed onScanProgramStart()");
    }

    void BaseScanProgram::stopScanProgram() {
        logger->d(getTag(), "Stopping ScanProgram");
        onScanProgramStop();
        desiredCpu0State.store(STOPPED);
        logger->d(getTag(), "Stopped ScanProgram");
    }

    void BaseScanProgram::serviceCpu0() {
        uint8_t desired = desiredCpu0State.load();
        if (desired == appliedCpu0State) {
            return;
        }
        appliedCpu0State = desired;
        if (desired == STARTED) {
            onScanProgramStartCpu0();
        } else {
            onScanProgramStopCpu0();
        }
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
        //No critical section here. render() used to run inside a critical section, which disabled interrupts on
        //core 1 for the whole line. scanvideo's per-line interrupts (PIO0_IRQ_0 / DMA_IRQ_0) run on
        //core 1 too and must launch each line's DMA within a few microseconds; with interrupts held off
        //during slow lines they ran late every line and the display went blank a few frames after
        //boot. Nothing else ever took it; CommandProcessor locks the state it shares with core 0.
        render(scanlineBuffer);
        scanvideo_end_scanline_generation(scanlineBuffer);
    }


} // scanPrograms