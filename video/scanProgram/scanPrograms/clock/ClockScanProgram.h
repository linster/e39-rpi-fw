//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_CLOCKSCANPROGRAM_H
#define PICOTEMPLATE_CLOCKSCANPROGRAM_H

#include "../BaseScanProgram.h"

namespace video::scanProgram::scanPrograms::clock {

    //Shown when the door handle is pulled up but the system isn't starting yet.
    class ClockScanProgram : public BaseScanProgram {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;

    public:
        ClockScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger
        );

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
    };

} // clock

#endif //PICOTEMPLATE_CLOCKSCANPROGRAM_H
