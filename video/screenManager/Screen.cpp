//
// Created by stefan on 6/18/23.
//

#include "Screen.h"


namespace video::ScreenManager {
    std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>> Screen::getScreenItems() {
        return {};
    }

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
            focusedIndex = focusedIndex + 1;
            if (focusedIndex == this->getScreenItems().size()) {
                focusedIndex = 0;
            }
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
            focusedIndex = focusedIndex - 1;
            if (focusedIndex < 0) {
                focusedIndex = this->getScreenItems().size() - 1;
            }
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



} // ScreenManager