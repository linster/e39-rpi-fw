//
// Created by stefan on 6/18/23.
//

#include "ScreenItem.h"

namespace video {
    namespace ScreenManager {
        void ScreenItem::setIsFocused(bool isFocused) {
            this->isFocused = isFocused;
        }

        bool ScreenItem::getIsFocused() {
            return isFocused;
        }
    } // video
} // screenManager