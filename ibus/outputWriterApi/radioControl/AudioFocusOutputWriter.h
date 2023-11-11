//
// Created by stefan on 11/11/23.
//

#ifndef PICOTEMPLATE_AUDIOFOCUSOUTPUTWRITER_H
#define PICOTEMPLATE_AUDIOFOCUSOUTPUTWRITER_H

#include "ibus/outputWriterApi/BaseOutputWriter.h"

namespace pico::ibus::output::writer::radio {

    class AudioFocusOutputWriter : public BaseOutputWriter {
    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<dma::IDmaManager> dmaManager;

    public:
        AudioFocusOutputWriter(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<dma::IDmaManager> dmaManager
                );


        virtual void audioSource_NavTv();
        virtual void audioSource_Aux();
    protected:
        std::string getTag() override { return "AudioFocusOutputWriter"; };
        std::shared_ptr<dma::IDmaManager> getDmaManager() override;
    };

} // radio

#endif //PICOTEMPLATE_AUDIOFOCUSOUTPUTWRITER_H
