//
// Created by stefan on 1/29/24.
//

#ifndef PICOTEMPLATE_DMAMANAGERPROVIDER_H
#define PICOTEMPLATE_DMAMANAGERPROVIDER_H

#include <memory>
#include "ibus/dma/IDmaManager.h"

namespace pico::di::provider {

    class DmaManagerProvider {

    private:
        std::shared_ptr<ibus::dma::IDmaManager> m_dmaManager;
    public:

        explicit DmaManagerProvider() noexcept = default;

        void setPointer(std::shared_ptr<ibus::dma::IDmaManager> ptr);
        std::shared_ptr<ibus::dma::IDmaManager> getPointer();


    };

} // provider

#endif //PICOTEMPLATE_DMAMANAGERPROVIDER_H
