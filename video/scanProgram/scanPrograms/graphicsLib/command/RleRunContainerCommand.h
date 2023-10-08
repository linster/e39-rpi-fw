//
// Created by stefan on 10/8/23.
//

#ifndef PICOTEMPLATE_RLERUNCONTAINERCOMMAND_H
#define PICOTEMPLATE_RLERUNCONTAINERCOMMAND_H

#include "BaseCommand.h"
namespace video::scanVideo::graphics::command {

    class RleRunContainerCommand : public BaseCommand{


        //This is here so that we can include a continuous RLE run
        //into the command stack, and drawing order is preserved.

    private:

        std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs;
        uint16_t topLine;
        uint16_t bottomLine;

    public:

        RleRunContainerCommand(
                std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs
                );

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;
        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;

    };

} // command

#endif //PICOTEMPLATE_RLERUNCONTAINERCOMMAND_H
