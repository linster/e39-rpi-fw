//
// Created by stefan on 11/11/23.
//

#ifndef PICOTEMPLATE_TESTINGOUTPUTWRITER_H
#define PICOTEMPLATE_TESTINGOUTPUTWRITER_H

#include "BaseOutputWriter.h"
#include "ibus/outputWriterApi/radioControl/AudioFocusOutputWriter.h"
namespace pico::ibus::output::writer {

                class TestingOutputWriter : public BaseOutputWriter {

                    //An output writer that can be hooked up to a BMBT button (like telephone)
                    //for debugging on a real bench.

                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<dma::IDmaManager> dmaManager;

                    std::shared_ptr<radio::AudioFocusOutputWriter> audioFocusOutputWriter;


                    bool isEnabled = true;

                    //Just a toggle variable for audio source.
                    bool toggle = false;
                public:

                    TestingOutputWriter(
                            std::shared_ptr<logger::BaseLogger> logger,
                            std::shared_ptr<dma::IDmaManager> dmaManager,
                            std::shared_ptr<radio::AudioFocusOutputWriter> audioFocusOutputWriter
                            );

                    virtual void sendDummyPacket();

                protected:
                    std::string getTag() override { return "TestingOutputWriter"; };
                    std::shared_ptr<dma::IDmaManager> getDmaManager() override;
                };

            } // writer

#endif //PICOTEMPLATE_TESTINGOUTPUTWRITER_H
