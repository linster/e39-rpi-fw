//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_NOOPSCANPROGRAM_H
#define PICOTEMPLATE_NOOPSCANPROGRAM_H

#include "../BaseScanProgram.h"

namespace video::scanProgram::scanPrograms::noop {

                class NoopScanProgram : public BaseScanProgram {

                private:
                    std::shared_ptr<pico::logger::BaseLogger> logger;
                public:

                    NoopScanProgram(
                            std::shared_ptr<pico::logger::BaseLogger> logger
                    );

                protected:
                    std::string getTag() override;
                    void onScanProgramStart() override;
                    void onScanProgramStop() override;
                    void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
                };

            } // noop

#endif //PICOTEMPLATE_NOOPSCANPROGRAM_H
