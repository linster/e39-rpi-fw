//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_MENUSCANPROGRAM_H
#define PICOTEMPLATE_MENUSCANPROGRAM_H

#include "../BaseScanProgram.h"
#include "../graphicsLib/graphicsLib.h"

namespace video::scanProgram::scanPrograms::menu {

    class MenuScanProgram : public BaseScanProgram{

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;

    public:
        MenuScanProgram(
                std::shared_ptr<pico::logger::BaseLogger> logger,
                std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib
        );

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
    };

            } // menu

#endif //PICOTEMPLATE_MENUSCANPROGRAM_H
