//
// Created by stefan on 9/1/23.
//

#include "ClockScanProgram.h"

namespace video::scanProgram::scanPrograms::clock {

    ClockScanProgram::ClockScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib
            ) {
        this->logger = logger;
        this->graphicsLib = graphicsLib;
        this->graphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
                );

        init(logger);
    }

    std::string ClockScanProgram::getTag() {
        return "ClockScanProgram";
    }

    void ClockScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
    }

    void ClockScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void ClockScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {

        uint16_t line_num = scanvideo_scanline_number(scanline_buffer->scanline_id);

        //Interval to skip
        uint16_t skipUntil = 204;

        if (line_num < skipUntil) {
            scanline_buffer->data_used = 0;
            scanline_buffer->status = SCANLINE_SKIPPED;
            return;
        }

        graphicsLib->writeSolidColourScanline(
                scanline_buffer,
                graphicsLib->getPalette()[15]
                );
    }

} // clock