//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SCREEN_H
#define PICOTEMPLATE_SCREEN_H

#include <vector>
#include <string>
#include <memory>
#include "ScreenItem.h"
#include "../../logging/BaseLogger.h"
#include "pico/mutex.h"
#include "fmt/format.h"

namespace video::ScreenManager {

        class Screen {


        private:

            std::shared_ptr<pico::logger::BaseLogger> logger;

            /** Which item in the getScreenItems() currently has the selection focus? */
            int focusedIndex = -1;

            //TODO maybe a mutex for when it's okay to modify this focusedIndex?

            mutex_t screenMutex;
        protected:
            void initialize(
                    std::shared_ptr<pico::logger::BaseLogger> logger
                    );

            virtual std::string getTag() = 0;
        public:

            /** Subclasses must implement this to indicate items */
            virtual std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>>getScreenItems();

            /** A string that's a centered title */
            virtual std::string getTitle() = 0;

            void focusFirstItem();
            void focusNextItem(int clicks);
            void focusPreviousItem(int clicks);
            void clickOnItem();

            /** Draw an ascii art representation of the screen to the provided logger */
            void drawToLogger(std::shared_ptr<pico::logger::BaseLogger> logger);

        };

    } // ScreenManager

#endif //PICOTEMPLATE_SCREEN_H
