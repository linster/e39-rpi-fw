//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_COMMANDPROCESSOR_H
#define PICOTEMPLATE_COMMANDPROCESSOR_H

#include <array>
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

        /*
         * Locking (mutators run on BOTH cores: scan program start/stop on core 1,
         * IBus-driven menu redraws on core 0; render_computed runs on core 1):
         *
         *  pendingMutex - "mutator lock". Held for the whole of any public method that
         *                 changes drawing state. Guards commandsToProcess, rleRunsToProcess,
         *                 lineBuffer, isImmediateMode. Never taken by render_computed.
         *  renderMutex  - short lock shared with render_computed. Guards rleRunsForLine,
         *                 isFrameComputed, baseColour. Writers only swap prepared data in
         *                 under it, one scanline at a time, and free old data after release.
         *
         * Lock order: pendingMutex, then renderMutex. render_computed takes only renderMutex
         * and must not allocate while holding it.
         */
        mutex_t pendingMutex;
        mutex_t renderMutex;

        std::vector<std::unique_ptr<BaseCommand>> commandsToProcess;

        std::map<uint16_t, std::vector<RleRun>> rleRunsToProcess;

        /**
         * Key is scanline number.
         * Value is a vector<RleRun> where no runs overlap, all runs sorted in ascending order
         * of startX. Can be looped over in drawScanline
         */
        std::map<uint16_t, std::vector<RleRun>> rleRunsForLine;

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


        void drawScanline(scanvideo_scanline_buffer_t *scanline_buffer, const std::vector<RleRun> &merged);
        void skipScanline(scanvideo_scanline_buffer_t *scanline_buffer);

        //Finish this (without depending on display metrics) to fill the screen with baseColour.
        void drawSolidColorScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour);

        bool isImmediateMode = true;

        /** Used to reduce memory footprint in real-life. Set to False for debugging. *
         *
         */
        bool clearCommandsOnFrameComputed = true;

        void clearCommandsToProcess();

        //Bodies of the public mutators; caller must hold pendingMutex.
        void clearFrameLocked();
        void computeFrameLocked();
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
        bool hasGraphicsToRender();

        /** If true, we are in immediate mode, which means that every addCommand()
         *  causes a computeFrame().
         * @return
         */
        bool getIsImmediateMode();
        /**
         * Enables and disables immediate mode. If enabled, every addCommand() causes
         * a computeFrame(), which can cause flickering when there's lots of text on screen.
         * @param immediateModeOn
         */
        void setImmediateMode(bool immediateModeOn);
    };

} // command

#endif //PICOTEMPLATE_COMMANDPROCESSOR_H
