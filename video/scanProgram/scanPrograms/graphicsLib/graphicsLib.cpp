//
// Created by stefan on 10/7/23.
//

#include "graphicsLib.h"

using video::scanVideo::graphics::command::PxCoord;
namespace video::scanProgram {
    uint32_t *graphicsLib::getPalette() {
        bool is_2_bpp = true;
        if (is_2_bpp) {
            return (uint32_t *) &pallet2bpp;
        } else {
            return (uint32_t *) &pallet;
        }
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

    void graphicsLib::setBaseColour(uint32_t color) {
        this->commandProcessor->setBaseColour(color);
    }

    void graphicsLib::clearScanlines(uint16_t min, uint16_t max) {
        this->commandProcessor->clearScanlines(min, max);
    }

    bool graphicsLib::hasGraphicsToRender() {
        return this->commandProcessor->hasGraphicsToRender();
    }

    bool graphicsLib::getIsImmediateMode() {
        return this->commandProcessor->getIsImmediateMode();
    }

    void graphicsLib::setImmediateMode(bool immediateModeOn) {
        this->commandProcessor->setImmediateMode(immediateModeOn);
    }

    void graphicsLib::computeFrame() {
        this->commandProcessor->computeFrame();
    }

    void graphicsLib::drawNabla(
            uint16_t bottomY,
            scanVideo::graphics::command::PxCoord topLeft,
            scanVideo::graphics::command::PxCoord topRight,
            bool filled,
            uint8_t thickness,
            uint32_t colour
    ) {
        setImmediateMode(false);

        if (filled) {
            PxCoord bottom = PxCoord(
                    topLeft.getX() + ((topRight.getX() - topLeft.getX()) / 2),
                    bottomY
                    );

            for (int i = topLeft.getX(); i < topRight.getX(); i++) {
                drawLine(
                        scanVideo::graphics::command::PxCoord(i, topLeft.getY()),
                        bottom,
                        colour,
                        1
                );
            }
        } else {
            //TODO copy in the code from bootspash scan program here.

        }

        setImmediateMode(true);
        computeFrame();

    }

    void graphicsLib::drawDelta(
            uint16_t topY,
            scanVideo::graphics::command::PxCoord bottomLeft,
            scanVideo::graphics::command::PxCoord bottomRight,
            bool filled,
            uint8_t thickness,
            uint32_t colour
    ) {
        setImmediateMode(false);

        if (filled) {
            PxCoord top = PxCoord(
                bottomLeft.getX() + ((bottomRight.getX() - bottomLeft.getX()) / 2),
                topY
                );
            for (int i = bottomLeft.getX(); i < bottomRight.getX(); i++) {
                drawLine(
                        scanVideo::graphics::command::PxCoord(i, bottomRight.getY()),
                        top,
                        colour,
                        1
                );
            }
        }

        setImmediateMode(true);
        computeFrame();
    }


} // scanProgram