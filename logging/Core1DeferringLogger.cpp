#include "Core1DeferringLogger.h"
#include "pico/platform.h"
#include "hardware/sync.h"

namespace pico::logger {

    Core1DeferringLogger::Core1DeferringLogger(std::shared_ptr<BaseLogger> inner) {
        this->inner = inner;
        queue_init_with_spinlock(&core1Lines, sizeof(Record *), QUEUE_DEPTH, spin_lock_claim_unused(true));
        instance = this;
    }

    void Core1DeferringLogger::print(Level level, std::string tag, std::string message) {
        if (get_core_num() == 0) {
            drain(); //Keep core 1's lines roughly in order with core 0's.
            inner->print(level, tag, message);
            return;
        }
        auto *record = new Record{level, std::move(tag), std::move(message)};
        if (!queue_try_add(&core1Lines, &record)) {
            delete record;
            core1LinesDropped = core1LinesDropped + 1;
        }
    }

    void Core1DeferringLogger::drainOnCore0() {
        if (instance != nullptr && get_core_num() == 0) {
            instance->drain();
        }
    }

    void Core1DeferringLogger::drain() {
        Record *record;
        while (queue_try_remove(&core1Lines, &record)) {
            inner->print(record->level, record->tag, record->message);
            delete record;
        }
    }

} // pico::logger
