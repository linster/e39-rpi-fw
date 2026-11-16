//
// Bench-only hook for testing the car bus transmitter (built only with -DE39_BENCH_TX_TEST=ON).
//

#ifndef PICOTEMPLATE_BENCHTXTESTOBSERVER_H
#define PICOTEMPLATE_BENCHTXTESTOBSERVER_H

#include <memory>
#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <ibus/dma/IDmaManager.h>

namespace pico::ibus::observers {

    /**
     * On a request packet 14 -> 31 (an unused source, to the Pico) with data `7E <n>`, queues n test
     * packets 31 -> 12 (12 is unused by E39 modules) through the same car transmit path the Pi's
     * forwarded packets use. Each test packet's data is `7E <request#> <seq> <pad...>` so a listener
     * on the bus can count which arrived intact. Nothing else is ever sent.
     */
    class BenchTxTestObserver : public BaseObserver {
    public:
        static constexpr uint8_t REQUEST_SRC = 0x14;
        static constexpr uint8_t TEST_DST = 0x12;
        static constexpr uint8_t MAGIC = 0x7E;
        static constexpr uint8_t MAX_PACKETS_PER_REQUEST = 64;

        BenchTxTestObserver(std::shared_ptr<logger::BaseLogger> logger, std::shared_ptr<dma::IDmaManager> dmaManager);
        std::string getTag() override { return "BenchTxTestObserver"; }

    protected:
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<dma::IDmaManager> dmaManager;
        uint8_t requests = 0;
    };

} // observers

#endif //PICOTEMPLATE_BENCHTXTESTOBSERVER_H
