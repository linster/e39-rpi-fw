//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SCREENITEM_H
#define PICOTEMPLATE_SCREENITEM_H

#include <string>

namespace video::ScreenManager {

        class ScreenItem {

        private:
            bool isFocused = false;
        public:

            virtual std::string getLabel() = 0;

            virtual void onItemFocused() {};
            virtual void onItemUnfocused() { };

            virtual void onItemClicked() = 0;

            bool getIsFocused();
            void setIsFocused(bool isFocused);
        };

    } // screenManager

#endif //PICOTEMPLATE_SCREENITEM_H
