//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_COMMANDPROCESSOR_H
#define PICOTEMPLATE_COMMANDPROCESSOR_H

#include <memory>
#include <vector>
#include "../../../../../logging/BaseLogger.h"
#include "BaseCommand.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"
#include <cstdint>
namespace video::scanVideo::graphics::command {

    class CommandProcessor {

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;


        uint8_t userFrameState;


        std::vector<std::unique_ptr<BaseCommand>> commandsToProcess;

        //Key is scanline
        //Value is a vector of RleRuns
        std::map<uint16_t, std::vector<RleRun>> rleRunsForLine;

        bool isFrameComputed = false;

        /**
         *
         * @param runs
         * @return returns a vector of RleRun where no runs overlap, and the runs are sorted in
         *         ascending order of startX.
         */
        std::vector<RleRun> mergeRuns(std::vector<RleRun> runs);


        void drawScanline(scanvideo_scanline_buffer_t *scanline_buffer, std::vector<RleRun> merged);
        void skipScanline(scanvideo_scanline_buffer_t *scanline_buffer);
    public:

        CommandProcessor(
                std::shared_ptr<pico::logger::BaseLogger> logger
                );

        /**
         * Add a command to the frame.
         * @param baseCommand
         */
        void addCommand(std::unique_ptr<BaseCommand> baseCommand);

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

        /** The user can use this int to figure out whether they are happy
         *  with the frame state.
         * @return An arbitrary int the user can set for frame state.
         */
        uint8_t getUserFrameState();
        void setUserFrameState(uint8_t frameState);
    };

} // command

#endif //PICOTEMPLATE_COMMANDPROCESSOR_H
