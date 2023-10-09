//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_GRAPHICSLIB_H
#define PICOTEMPLATE_GRAPHICSLIB_H

#include <cstdint>
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/scanvideo/composable_scanline.h"

#include "DisplayMetrics.h"
#include "command/CommandProcessor.h"
#include "command/BaseCommand.h"
#include "command/PxCoord.h"
#include "command/LineCommand.h"
#include "command/EmptyRectangleCommand.h"
#include "command/FilledRectangleCommand.h"
#include "command/TextCommand.h"
#include "command/RleRun.h"

namespace video::scanProgram {

        class graphicsLib {


        private:

            std::shared_ptr<scanVideo::graphics::command::CommandProcessor> commandProcessor;

            bool haveDisplayMetrics = false;
            DisplayMetrics displayMetrics = DisplayMetrics(0,0);
            void requireDisplayMetrics();

            //https://github.com/dquadros/pico-vgatext40/blob/master/textvideo.c
            // 16 color pallet
        #define PICO_COLOR_FROM_RGB5(r, g, b)  ((r & 0x1F) | ((g & 0x1F) << 6) | ((b & 0x1F) << 11))
        static constexpr uint32_t pallet[] = {
                PICO_COLOR_FROM_RGB5(0, 0, 0),
                PICO_COLOR_FROM_RGB5(0, 0, 15),
                PICO_COLOR_FROM_RGB5(0, 15, 0),
                PICO_COLOR_FROM_RGB5(0, 15, 15),
                PICO_COLOR_FROM_RGB5(15, 0, 0),
                PICO_COLOR_FROM_RGB5(15, 0, 15),
                PICO_COLOR_FROM_RGB5(15, 15, 0),
                PICO_COLOR_FROM_RGB5(15, 15, 15),   // 7, Lighter Grey
                PICO_COLOR_FROM_RGB5(7, 7, 7),      // 8, Light Grey
                PICO_COLOR_FROM_RGB5(0, 0, 31),     // 9, Bright Blue
                PICO_COLOR_FROM_RGB5(0, 31, 0),    //10, Bright Green
                PICO_COLOR_FROM_RGB5(0, 31, 31),   //11, Bright Light Blue
                PICO_COLOR_FROM_RGB5(31, 0, 0),    //12, Bright red
                PICO_COLOR_FROM_RGB5(31, 0, 31),   //13, Bright purple
                PICO_COLOR_FROM_RGB5(31, 31, 0),   //14, Bright Yellow
                PICO_COLOR_FROM_RGB5(31, 31, 31)   //15, Bright White
        };

        public:

            constexpr static uint8_t* SPECIAL_CHARACTER_COPYRIGHT = const_cast<uint8_t *>(fonts::FontProvider::font8x8_ext_latin[9]);

            graphicsLib(
                    std::shared_ptr<scanVideo::graphics::command::CommandProcessor> commandProcessor
            );


        void setDisplayMetrics(DisplayMetrics displayMetrics);

            /**
             * Gets an EGA 16-colour palette. Indices into the returned array
             * are [0, 15].
             *
             * For example:
             * @code
             * uint32_t color = graphicsLib->getPalette()[15];
             * *p++ = color;
             * @endcode
             * @return Returns a pointer to an EGA colour palette.
             * @
             */
            uint32_t* getPalette();

            /**
             * Writes a solid colour scanline. No additional drawing on top of the written
             * scanline is allowed.
             * @param scanline_buffer the scanline buffer to write into.
             * @param colour the uint32_t colour to write to the scanline. Could be from a palette.
             */
            void writeSolidColourScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour);

            //We need a method that draws a solid scanline

            //We can't mutate a scanline by drawing over it again,
            //since the existing scanline is run-length encoded (RLE)
            // However, what we can do is do a font routine that RLEs
            // part of the font onto a solid colour.


            void clearFrame();
            void drawLine(
                    scanVideo::graphics::command::PxCoord topLeftPx,
                    scanVideo::graphics::command::PxCoord bottomRightPx,
                    uint32_t colour,
                    uint8_t lineWidth
                    );
            void drawEmptyRectangle(
                    scanVideo::graphics::command::PxCoord topLeftPx,
                    scanVideo::graphics::command::PxCoord bottomRightPx,
                    uint32_t colour,
                    uint8_t lineWidth
                    );

            void drawFilledRectangle(
                    scanVideo::graphics::command::PxCoord topLeftPx,
                    scanVideo::graphics::command::PxCoord bottomRightPx,
                    uint32_t colour
                    );

            void drawText(
                    std::string text,
                    scanVideo::graphics::command::PxCoord topLeftPx,
                    uint32_t colour,
                    uint8_t pixelSize
                    );

            void drawTextSpecialCharacter(
                    uint8_t* bitmap,
                    scanVideo::graphics::command::PxCoord topLeftPx,
                    uint32_t colour,
                    uint8_t pixelSize
            );

            void addCommandToFrame(std::unique_ptr<scanVideo::graphics::command::BaseCommand> command);

            //So we can add arbitrary RLE runs (like a special character)
            void contributeRleRuns(
                    std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs
            );

            uint8_t getUserFrameState();
            void setUserFrameState(uint8_t state);
            void render_commandProcessed(scanvideo_scanline_buffer_t *scanline_buffer);

        };

    } // scanProgram

#endif //PICOTEMPLATE_GRAPHICSLIB_H