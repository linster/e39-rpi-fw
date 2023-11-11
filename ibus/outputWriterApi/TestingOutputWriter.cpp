//
// Created by stefan on 11/11/23.
//

#include "TestingOutputWriter.h"

namespace pico::ibus::output::writer {
    TestingOutputWriter::TestingOutputWriter(std::shared_ptr<logger::BaseLogger> logger,
                                             std::shared_ptr<dma::IDmaManager> dmaManager,
                                             std::shared_ptr<radio::AudioFocusOutputWriter> audioFocusOutputWriter) {
        this->logger = logger;
        this->dmaManager = dmaManager;
        this->audioFocusOutputWriter = audioFocusOutputWriter;
    }

    void TestingOutputWriter::sendDummyPacket() {
        if (!isEnabled) {
            return;
        }
        logger->d(getTag(), "Sending test packet to car");

        if (toggle) {
            audioFocusOutputWriter->audioSource_Aux();
        } else {
            audioFocusOutputWriter->audioSource_NavTv();
        }

        toggle = !toggle;

        logger->d(getTag(), "Sent test packet to car");
    }

    std::shared_ptr<dma::IDmaManager> TestingOutputWriter::getDmaManager() {
        return this->dmaManager;
    }


} // writer