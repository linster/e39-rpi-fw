//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_COMMANDPROCESSOR_H
#define PICOTEMPLATE_COMMANDPROCESSOR_H

#include <memory>
#include <vector>
#include <logging/BaseLogger.h>
#include "BaseCommand.h"
#include "RleRunContainerCommand.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/mutex.h"
#include <cstdint>
namespace video::scanVideo::graphics::command {

    class CommandProcessor {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;

        std::vector<std::unique_ptr<BaseCommand>> commandsToProcess;
        
        /**
         * Key is scanline number.
         * Value is a vector<RleRun> where no runs overlap, all runs sorted in ascending order
         * of startX. Can be looped over in drawScanline
         */
        std::map<uint16_t, std::vector<RleRun>> rleRunsForLine;


        //Guards: isFrameComputedMutex, rleRunsForLine
        mutex_t isFrameComputedMutex;
        //End Guard: isFrameComputedMutex, rleRunsForLine

        bool isFrameComputed = false;

        //We only need a 400*234 display max, so we'll just use a line buffer.
        //Someday I'll clean this function up, but is should consumer only ~2kb of ram,
        //So it's already better than a frame buffer.
        static constexpr const uint16_t maxDisplayWidth = 400;

        uint32_t baseColour = 0;
        std::array<uint32_t, maxDisplayWidth> lineBuffer;


        /**
         *
         * @param runs
         * @return returns a vector of RleRun where no runs overlap, and the runs are sorted in
         *         ascending order of startX. The returned vector can be looped over in drawScanline.
         */
        std::vector<RleRun> mergeRuns(std::unique_ptr<std::vector<RleRun>> runs);


        void drawScanline(scanvideo_scanline_buffer_t *scanline_buffer, std::vector<RleRun> merged);
        void skipScanline(scanvideo_scanline_buffer_t *scanline_buffer);

        //Finish this (without depending on display metrics) to fill the screen with baseColour.
        //void drawSolidColorScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour);
    public:

        explicit CommandProcessor(
                std::shared_ptr<pico::logger::BaseLogger> logger
                );

        /**
         * Add a command to the frame.
         * @param baseCommand
         */
        void addCommand(std::unique_ptr<BaseCommand> baseCommand);


        void addRleRuns(std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs);

        /**
         * Remove all the commands from the frame.
         */
        void clearFrame();

        /**
         * Compute the run lengths & tokens for the frame.
         * Must be called so that render can loop over the stored run lengths
         */
        void computeFrame();

        /**
         * Call this from a scan program to draw the computed frame.
         * @param scanline_buffer
         */
        void render_computed(scanvideo_scanline_buffer_t *scanline_buffer);

        //TODO render_computed needs to skip scanlines with no runs on them.


        void setBaseColour(uint32_t color);

        void clearScanlines(uint16_t min, uint16_t max);
    };

} // command

#endif //PICOTEMPLATE_COMMANDPROCESSOR_H
