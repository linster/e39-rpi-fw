//
// Created by stefan on 6/18/23.
//

#include "ScreenManager.h"

namespace video {
    namespace ScreenManager {

        ScreenManager::ScreenManager(std::shared_ptr<Screen> initialScreen) {
            this->currentScreen = initialScreen;
        }

        void ScreenManager::clickOnItem() {
            currentScreen->clickOnItem();
        }

        void ScreenManager::focusFirstItem() {
            currentScreen->focusFirstItem();
        }

        void ScreenManager::focusPreviousItem(int clicks) {
            currentScreen->focusPreviousItem(clicks);
        }

        void ScreenManager::focusNextItem(int clicks) {
            currentScreen->focusNextItem(clicks);
        }

        std::shared_ptr<Screen> ScreenManager::getCurrentScreen() {
            return currentScreen;
        }
    } // video
} // ScreenManager