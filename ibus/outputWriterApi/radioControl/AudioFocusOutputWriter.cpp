//
// Created by stefan on 11/11/23.
//

#include "AudioFocusOutputWriter.h"

namespace pico::ibus::output::writer::radio {
    AudioFocusOutputWriter::AudioFocusOutputWriter(std::shared_ptr<logger::BaseLogger> logger,
                                                   std::shared_ptr<dma::IDmaManager> dmaManager) {

        this->logger = logger;
        this->dmaManager = dmaManager;
        if (this->dmaManager == nullptr) {
            logger->wtf("AudioFocusOutputWriter", "dmaManager is null");
        }
    }

    std::shared_ptr<dma::IDmaManager> AudioFocusOutputWriter::getDmaManager() {
        return this->dmaManager;
    }

    void AudioFocusOutputWriter::audioSource_NavTv() {
        // To switch radio to NAV/TV
        // Src Device: GT
        // Dest Device: RAD
        // Datagram: 3B 05 68 4E 01 00 19

        //Mode button on BMBT won't work till aux sent.
        logger->d(getTag(), "Request Audio Source NAV/TV.");

        std::vector<uint8_t> raw = std::vector<uint8_t>();
        raw.push_back(0x3B);
        raw.push_back(0x05);
        raw.push_back(0x68);
        raw.push_back(0x4E);
        raw.push_back(0x01);
        raw.push_back(0x00);
        raw.push_back(0x19);

        data::IbusPacket packet = data::IbusPacket(raw);

        getDmaManager()->cpu0scheduleOutgoingMessage(packet);
        getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(packet);
    }

    void AudioFocusOutputWriter::audioSource_Aux() {
        // To switch radio to AUX:
        // Src Device: GT
        // Dest Device: RAD
        // Datagram: 3B 05 68 4E 00 00 18

        logger->d(getTag(), "Request Audio Source Aux");

        std::vector<uint8_t> raw = std::vector<uint8_t>();
        raw.push_back(0x3B);
        raw.push_back(0x05);
        raw.push_back(0x68);
        raw.push_back(0x4E);
        raw.push_back(0x00);
        raw.push_back(0x00);
        raw.push_back(0x18);

        data::IbusPacket packet = data::IbusPacket(raw);

        getDmaManager()->cpu0scheduleOutgoingMessage(packet);
        getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(packet);

    }


} // radio