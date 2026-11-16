//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H
#define PICOTEMPLATE_BOOTSPLASHSCANPROGRAM_H

#include <video/scanProgram/scanPrograms/BaseScanProgram.h>
#include <video/scanProgram/scanPrograms/graphicsLib/graphicsLib.h>

using video::scanVideo::graphics::command::PxCoord;

namespace video::scanProgram::scanPrograms::bootsplash {

    class BootsplashScanProgram : public BaseScanProgram {


    public:
        enum BootSplashType {
            LINSTER_OS, BMW, GOOSE
        };

        /** Which splash to draw the next time this program starts. Set it before swapping to the
         *  program (core 0); it's read on core 1 in onScanProgramStart(). */
        void setBootSplashType(BootSplashType type) { bootSplashType = type; }

        BootsplashScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib
        );

        static void drawLinsterOs(
                std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib,
                DisplayMetrics displayMetrics
                );

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;

        BootSplashType bootSplashType = LINSTER_OS;
//        BootSplashType bootSplashType = GOOSE;

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
