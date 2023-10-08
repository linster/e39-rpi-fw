//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H
#define PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H

#include "../BaseScanProgram.h"
#include "../graphicsLib/graphicsLib.h"

namespace video::scanProgram::scanPrograms::bootsplash {

    class BootsplashScanProgram : public BaseScanProgram {


    public:

        enum BootSplashType {
            LINSTER_OS, BMW, GOOSE
        };

        BootsplashScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib
        );

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;

        BootSplashType bootSplashType = LINSTER_OS;

        void onScanProgramStart_drawLinsterOS();
        void onScanProgramStart_BMW();
        void onScanProgramStart_Goose();

        void render_LinsterOS(scanvideo_scanline_buffer_t *scanline_buffer);
        void render_BMW(scanvideo_scanline_buffer_t *scanline_buffer);
        void render_Goose(scanvideo_scanline_buffer_t *scanline_buffer);

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;

    };
} // bootsplash

#endif //PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H
