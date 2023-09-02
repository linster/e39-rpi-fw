//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_DEMOSCANPROGRAM_H
#define PICOTEMPLATE_DEMOSCANPROGRAM_H

#include "../BaseScanProgram.h"

namespace video::scanProgram::scanPrograms::demo {

    class DemoScanProgram : public BaseScanProgram {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;

    public:
        DemoScanProgram(
                std::shared_ptr<pico::logger::BaseLogger> logger
        );

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
    };

            } // demo

#endif //PICOTEMPLATE_DEMOSCANPROGRAM_H
