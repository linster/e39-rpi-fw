//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_VIDEOSWITCH_H
#define PICOTEMPLATE_VIDEOSWITCH_H

#include <functional>
#include "VideoSource.h"

namespace pico::hardware::videoSwitch {

            class VideoSwitch {
            public:
                virtual void switchTo(VideoSource source) = 0;
                virtual VideoSource getPreviousVideoSource() = 0;

                /** Called after every switchTo() that changes the source, with the new source. */
                void setOnSourceChanged(std::function<void(VideoSource)> listener) { onSourceChanged = listener; }
            protected:
                void notifySourceChanged(VideoSource from, VideoSource to) {
                    if (from != to && onSourceChanged) { onSourceChanged(to); }
                }
            private:
                std::function<void(VideoSource)> onSourceChanged;
            };

        } // videoSwitch

#endif //PICOTEMPLATE_VIDEOSWITCH_H
