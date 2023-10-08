//
// Created by stefan on 10/7/23.
//

#include "graphicsLib.h"

namespace video::scanProgram {
    uint32_t *graphicsLib::getPalette() {
        return (uint32_t *) &pallet;
    }


    void graphicsLib::setDisplayMetrics(DisplayMetrics displayMetrics) {
        this->displayMetrics = displayMetrics;
        this->haveDisplayMetrics = true;
    }

    void graphicsLib::requireDisplayMetrics() {
        if (!haveDisplayMetrics) {
            panic("No display metrics");
        }
    }

    void graphicsLib::writeSolidColourScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour) {
        requireDisplayMetrics();

        uint16_t *p = (uint16_t *) scanline_buffer->data;

        *p++ = COMPOSABLE_COLOR_RUN;
        uint32_t color = colour;
        *p++ = color;
        *p++ = displayMetrics.getDisplayWidthPx() - 3;

        // 32 * 3, so we should be word aligned
//        assert(!(3u & (uintptr_t) p));

        // black pixel to end line
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = 0;
        // end of line with alignment padding
        *p++ = COMPOSABLE_EOL_SKIP_ALIGN;
        *p++ = 0;

        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);

        scanline_buffer->status = SCANLINE_OK;

    }

    void graphicsLib::clearFrame() {
        commandProcessor->clearFrame();
    }

    void graphicsLib::drawLine(scanVideo::graphics::command::PxCoord topLeftPx,
                               scanVideo::graphics::command::PxCoord bottomRightPx, uint32_t colour,
                               uint8_t lineWidth) {

        std::unique_ptr<scanVideo::graphics::command::LineCommand> ptr =
                std::make_unique<scanVideo::graphics::command::LineCommand>(topLeftPx, bottomRightPx, colour, lineWidth);

        addCommandToFrame(std::move(ptr));
    }

    void graphicsLib::drawEmptyRectangle(scanVideo::graphics::command::PxCoord topLeftPx,
                                         scanVideo::graphics::command::PxCoord bottomRightPx, uint32_t colour,
                                         uint8_t lineWidth) {


        std::unique_ptr<scanVideo::graphics::command::EmptyRectangleCommand> ptr =
                std::make_unique<scanVideo::graphics::command::EmptyRectangleCommand>(topLeftPx, bottomRightPx, colour, lineWidth);

        addCommandToFrame(std::move(ptr));
    }

    void graphicsLib::drawFilledRectangle(scanVideo::graphics::command::PxCoord topLeftPx,
                                          scanVideo::graphics::command::PxCoord bottomRightPx, uint32_t colour) {

        std::unique_ptr<scanVideo::graphics::command::FilledRectangleCommand> ptr =
                std::make_unique<scanVideo::graphics::command::FilledRectangleCommand>(topLeftPx, bottomRightPx, colour);

        addCommandToFrame(std::move(ptr));
    }

    void graphicsLib::drawText(std::string text, scanVideo::graphics::command::PxCoord topLeftPx, uint32_t colour,
                               uint8_t pixelSize) {

        std::unique_ptr<scanVideo::graphics::command::TextCommand> ptr =
                std::make_unique<scanVideo::graphics::command::TextCommand>(text, topLeftPx, colour, pixelSize);

        addCommandToFrame(std::move(ptr));
    }

    void graphicsLib::addCommandToFrame(std::unique_ptr<scanVideo::graphics::command::BaseCommand> command) {
        commandProcessor->addCommand(std::move(command));
    }

    uint8_t graphicsLib::getUserFrameState() {
        return commandProcessor->getUserFrameState();
    }

    void graphicsLib::setUserFrameState(uint8_t state) {
        commandProcessor->setUserFrameState(state);
    }

    void graphicsLib::render_commandProcessed(scanvideo_scanline_buffer_t *scanline_buffer) {
        commandProcessor->render_computed(scanline_buffer);
    }

    graphicsLib::graphicsLib(std::shared_ptr<scanVideo::graphics::command::CommandProcessor> commandProcessor) {
        this->commandProcessor = commandProcessor;
    }

    void graphicsLib::contributeRleRuns(
            std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs) {
        this->commandProcessor->addRleRuns(runs);
    }

    void graphicsLib::drawTextSpecialCharacter(uint8_t *bitmap, scanVideo::graphics::command::PxCoord topLeftPx,
                                               uint32_t colour, uint8_t pixelSize) {

        scanVideo::graphics::command::TextCommand textCommand = scanVideo::graphics::command::TextCommand(
                "",
                topLeftPx,
                colour,
                pixelSize
                );

        contributeRleRuns(textCommand.getRleRunsForSpecialCharacter(bitmap));
    }


} // scanProgram