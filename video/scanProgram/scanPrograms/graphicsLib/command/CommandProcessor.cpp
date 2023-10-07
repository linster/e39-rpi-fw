//
// Created by stefan on 10/7/23.
//

#include "CommandProcessor.h"

namespace video::scanVideo::graphics::command {

    CommandProcessor::CommandProcessor(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
    }

    uint8_t CommandProcessor::getUserFrameState() {
        return userFrameState;
    }

    void CommandProcessor::setUserFrameState(uint8_t frameState) {
        this->userFrameState = frameState;
    }
} // command