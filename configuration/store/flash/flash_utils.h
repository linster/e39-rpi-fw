#include <cstdint>

//Adapted from
//https://community.element14.com/products/raspberry-pi/b/blog/posts/raspberry-pico-c-sdk-reserve-a-flash-memory-block-for-persistent-storage

inline uint8_t *getAddressPersistent() {
extern uint8_t ADDR_PERSISTENT[];
    return ADDR_PERSISTENT;
}