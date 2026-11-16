//
// Bench-only hooks for testing video (built only with -DE39_BENCH_TX_TEST=ON).
//

#ifndef PICOTEMPLATE_BENCHVIDEOTESTOBSERVER_H
#define PICOTEMPLATE_BENCHVIDEOTESTOBSERVER_H

#include <memory>
#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <video/scanProgram/ScanProgramSwapper.h>

namespace pico::ibus::observers {

    /**
     * Requests are packets 14 -> 31 (an unused source, to the Pico). Nothing is ever transmitted.
     *   7D <program>  switch to scan program <program> (ScanProgram enum: 0 BOOT_SPLASH, 1 CLOCK, 2 DEMO, 3 MENU, 4 NOOP)
     *   7C <ms>       make core 1 busy-wait <ms> once (no scanlines delivered meanwhile; interrupts stay on)
     */
    class BenchVideoTestObserver : public BaseObserver {
    public:
        static constexpr uint8_t REQUEST_SRC = 0x14;
        static constexpr uint8_t SWAP = 0x7D;
        static constexpr uint8_t STALL_CORE1 = 0x7C;

        BenchVideoTestObserver(std::shared_ptr<logger::BaseLogger> logger,
                               std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper);
        std::string getTag() override { return "BenchVideoTestObserver"; }

    protected:
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
    };

} // observers

#endif //PICOTEMPLATE_BENCHVIDEOTESTOBSERVER_H
