//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_DEMOSCANPROGRAM_H
#define PICOTEMPLATE_DEMOSCANPROGRAM_H

#include "../BaseScanProgram.h"


#define MIN_COLOR_RUN 3

namespace video::scanProgram::scanPrograms::demo {

    class DemoScanProgram : public BaseScanProgram {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;


        int32_t single_color_scanline(uint32_t *buf, size_t buf_length, int width, uint32_t color16);

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
