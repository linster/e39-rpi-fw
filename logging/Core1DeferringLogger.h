//
// Hands log lines written on core 1 to core 0.
//

#ifndef PICOTEMPLATE_CORE1DEFERRINGLOGGER_H
#define PICOTEMPLATE_CORE1DEFERRINGLOGGER_H

#include <memory>
#include <string>
#include "pico/util/queue.h"
#include "BaseLogger.h"

namespace pico::logger {

    /**
     * Wraps the real logger. On core 0 it logs directly. On core 1 it only queues the line, and core 0
     * logs it from drainOnCore0().
     *
     * Core 1 drives scanvideo: its line-start interrupt has ~4.5 us to launch each line's DMA, so core 1
     * must never hold interrupts off for longer than that. The real loggers do: queueing a log packet
     * copies a 255-byte queue element with interrupts off (measured 15-29 us), and the USB stdio logger
     * runs TinyUSB's tud_task(). Each such log line on core 1 could make a scanline late, which set off
     * scanvideo's scanline "recovery" and sometimes wedged the scanline state machine (every scan
     * program switch logs from core 1). Here core 1 allocates the record with interrupts on and queues
     * only a pointer, on a spin lock nothing else uses.
     */
    class Core1DeferringLogger : public BaseLogger {
    public:
        explicit Core1DeferringLogger(std::shared_ptr<BaseLogger> inner);

        void d(std::string tag, std::string message) override { print(Level::DEBUG, tag, message); }
        void i(std::string tag, std::string message) override { print(Level::INFO, tag, message); }
        void w(std::string tag, std::string message) override { print(Level::WARN, tag, message); }
        void e(std::string tag, std::string message) override { print(Level::ERROR, tag, message); }
        void wtf(std::string tag, std::string message) override { print(Level::WTF, tag, message); }
        void print(Level level, std::string tag, std::string message) override;

        /** Log everything core 1 queued. Call regularly from core 0's loop. */
        static void drainOnCore0();

        /** Core 1 log lines dropped because the queue was full. */
        inline static volatile uint32_t core1LinesDropped = 0;

    private:
        struct Record {
            Level level;
            std::string tag;
            std::string message;
        };
        static constexpr uint QUEUE_DEPTH = 32;

        std::shared_ptr<BaseLogger> inner;
        queue_t core1Lines;
        inline static Core1DeferringLogger *instance = nullptr;

        void drain();
    };

} // pico::logger

#endif //PICOTEMPLATE_CORE1DEFERRINGLOGGER_H
