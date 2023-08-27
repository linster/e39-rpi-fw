//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SCREENITEM_H
#define PICOTEMPLATE_SCREENITEM_H

#include <string>

namespace video::ScreenManager {

        class ScreenItem {

        private:
            bool isFocused;
        public:

            virtual std::string getLabel();

            virtual void onItemFocused();
            virtual void onItemUnfocused();

            virtual void onItemClicked();

            bool getIsFocused();
        };

    } // screenManager

#endif //PICOTEMPLATE_SCREENITEM_H
