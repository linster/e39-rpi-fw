//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H
#define PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H

#include "../BaseScanProgram.h"

namespace video::scanProgram::scanPrograms::bootsplash {

    class BootsplashScanProgram : public BaseScanProgram {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;

    public:

        enum BootSplashType {
            LINSTER_OS, BMW, GOOSE
        };

        BootsplashScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger
        );

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
    };
} // bootsplash

#endif //PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H
