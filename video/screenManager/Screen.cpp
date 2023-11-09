//
// Created by stefan on 6/18/23.
//

#include "Screen.h"


namespace video::ScreenManager {

    void Screen::initialize(
            std::shared_ptr<pico::logger::BaseLogger> logger
            ) {
        this->logger = logger;

        logger->d(getTag(), "Initializing screen");
        mutex_init(&this->screenMutex);
        logger->d(getTag(), "Initialized screen mutex");
        focusFirstItem();
    }

    void Screen::focusFirstItem() {
        logger->d(getTag(), "Focus first item");
        if (!getScreenItems().empty()) {
            focusedIndex = 0;
        }
        logger->d(getTag(), fmt::format("Current focusedIndex: {}", focusedIndex));

        getScreenItems()[focusedIndex]->setIsFocused(true);
    }

    void Screen::focusNextItem(int clicks) {
        logger->d(getTag(), fmt::format("focusNextItem: Current focusedIndex: {}", focusedIndex));
        if (clicks < 0 ) {
            logger->w(getTag(), "Asked for a negative number for forward clicks");
            return;
        }
        if (clicks == 0) {
            return;
        }
        if (clicks == 1) {
            int oldFocusedIndex = focusedIndex;
            callOnUnFocusItem(oldFocusedIndex);
            focusedIndex = focusedIndex + 1;
            if (focusedIndex == this->getScreenItems().size()) {
                focusedIndex = 0;
            }
            callOnFocusItem(focusedIndex);
            logger->d(getTag(), fmt::format("focusNextItem: New focusedIndex: {}", focusedIndex));
        }

        if (clicks > 1) {
            int toClick = clicks;
            while (toClick > 0) {
                focusNextItem(toClick--);
            }
        }
    }

    void Screen::focusPreviousItem(int clicks) {
        logger->d(getTag(), fmt::format("focusPreviousItem: Current focusedIndex: {}", focusedIndex));
        if (clicks < 0 ) {
            logger->w(getTag(), "Asked for a negative number for backward clicks");
            return;
        }
        if (clicks == 0) {
            return;
        }
        if (clicks == 1) {
            int oldFocusedIndex = focusedIndex;
            callOnUnFocusItem(oldFocusedIndex);
            focusedIndex = focusedIndex - 1;
            if (focusedIndex < 0) {
                focusedIndex = this->getScreenItems().size() - 1;
            }
            callOnFocusItem(focusedIndex);
            logger->d(getTag(), fmt::format("focusPreviousItem: New focusedIndex: {}", focusedIndex));
        }

        if (clicks > 1) {
            int toClick = clicks;
            while (toClick > 0) {
                focusPreviousItem(toClick--);
            }
        }
    }

    void Screen::clickOnItem() {
        logger->d(getTag(), "Click on item");
        logger->d(getTag(), fmt::format("Current focusedIndex: {}", focusedIndex));

        auto item = getScreenItems()[focusedIndex];

        logger->d(getTag(), fmt::format("Dispatching click to item with label: {}", item->getLabel()));
        item->onItemClicked();
        logger->d(getTag(), fmt::format("Dispatched click to item with label: {}", item->getLabel()));
    }

    void Screen::callOnFocusItem(int index) {
        logger->d(getTag(), fmt::format("Focusing item with index: {}", index));

        auto item = getScreenItems()[index];

        item->setIsFocused(true);
        item->onItemFocused();

        if (this->debugPrintAsciiArt) {
            drawToLogger(logger);
        }

        logger->d(getTag(), fmt::format("Focused item with index: {}", index));
    }

    void Screen::callOnUnFocusItem(int index) {
        logger->d(getTag(), fmt::format("UnFocusing item with index: {}", index));

        auto item = getScreenItems()[index];

        item->setIsFocused(false);
        item->onItemUnfocused();

        logger->d(getTag(), fmt::format("UnFocused item with index: {}", index));
    }

    void Screen::drawToLogger(std::shared_ptr<pico::logger::BaseLogger> logger) {
        std::string debugTag = fmt::format("ASCII_SCREEN" , getTitle());

        logger->d(debugTag, fmt::format("=== {} ===", getTitle()));

        for (int i = 0; i < getScreenItems().size() -1 ; i++) {

            auto item = getScreenItems()[i];

            if (focusedIndex == i) {
                logger->d(debugTag, fmt::format("> {}", item->getLabel()));
            } else {
                logger->d(debugTag, fmt::format("- {}", item->getLabel()));
            }
        }

        logger->d(debugTag, fmt::format("^^^ {} ^^^", getTitle()));
    }



} // ScreenManager
