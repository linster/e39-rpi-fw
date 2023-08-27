//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SCREENMANAGER_H
#define PICOTEMPLATE_SCREENMANAGER_H

#include "Screen.h"

namespace video::ScreenManager {

        class ScreenManager {


        private:
            //Someday add support for navigating between screens. For now, just
            //hard-code in the one screen.
            std::shared_ptr<Screen> currentScreen;

        public:

            ScreenManager(
                    std::shared_ptr<Screen> initialScreen
                    );

            std::shared_ptr<Screen> getCurrentScreen();


            //void switchToScreen(std::shared_ptr<Screen> screen);

            void focusFirstItem();
            void focusNextItem(int clicks);
            void focusPreviousItem(int clicks);
            void clickOnItem();

        };

    } // ScreenManager

#endif //PICOTEMPLATE_SCREENMANAGER_H