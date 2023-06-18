//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SCREENMANAGER_H
#define PICOTEMPLATE_SCREENMANAGER_H

#include "Screen.h"

namespace video {
    namespace ScreenManager {

        class ScreenManager {


        private:
            //Someday add support for navigating between screens. For now, just
            //hard-code in the one screen.
            std::shared_ptr<Screen> currentScreen;

        public:
            std::shared_ptr<Screen> getCurrentScreen();
            void switchToScreen(std::shared_ptr<Screen> screen);

            void focusFirstItem();
            void focusNextItem(int clicks);
            void focusPreviousItem(int clicks);
            void clickOnItem();

        };

    } // video
} // ScreenManager

#endif //PICOTEMPLATE_SCREENMANAGER_H
