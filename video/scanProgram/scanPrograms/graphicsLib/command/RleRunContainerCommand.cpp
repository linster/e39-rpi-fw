//
// Created by stefan on 10/8/23.
//

#include "RleRunContainerCommand.h"

namespace video::scanVideo::graphics::command {


    RleRunContainerCommand::RleRunContainerCommand(
            std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs) {

        this->runs = runs;

        //end() can't be dereferenced (and begin() can't either when empty).
        if (this->runs.empty()) {
            this->topLine = 0;
            this->bottomLine = 0;
        } else {
            this->topLine = this->runs.begin()->first;
            this->bottomLine = this->runs.rbegin()->first;
        }
    }


    std::map<uint16_t, std::vector<RleRun>> RleRunContainerCommand::getRleRunsForShape() {
        return runs;
    }

    std::pair<uint16_t, uint16_t> RleRunContainerCommand::getAffectedScanlines() {
        return std::pair<uint16_t, uint16_t>(topLine, bottomLine);
    }
} // command