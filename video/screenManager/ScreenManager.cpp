//
// Created by stefan on 6/18/23.
//

#include "ScreenManager.h"

namespace video {
    namespace ScreenManager {

        ScreenManager::ScreenManager() = default;

        void ScreenManager::registerScreen(std::shared_ptr<Screen> screen) {
            this->currentScreen = screen;
        }

        void ScreenManager::clickOnItem() {
            currentScreen->clickOnItem();
        }

        void ScreenManager::focusFirstItem() {
            currentScreen->focusFirstItem();
            //callOnFocusChangeCallback();
        }

        void ScreenManager::focusPreviousItem(int clicks) {
            currentScreen->focusPreviousItem(clicks);
            callOnFocusChangeCallback();
        }

        void ScreenManager::focusNextItem(int clicks) {
            currentScreen->focusNextItem(clicks);
            callOnFocusChangeCallback();
        }

        std::shared_ptr<Screen> ScreenManager::getCurrentScreen() {
            return currentScreen;
        }

        void ScreenManager::callOnFocusChangeCallback() {
            onFocusedChangedCallback();
        }

        void ScreenManager::registerOnFocusChangeListener(std::function<void()> onFocusChange) {
            onFocusedChangedCallback = onFocusChange;
        }

        void ScreenManager::unregisterOnFocusChangeListener() {
            onFocusedChangedCallback = std::function<void()> {};
        }


    } // video
} // ScreenManager