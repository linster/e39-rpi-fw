//
// Reading nanopb enum fields safely.
//

#ifndef PICOTEMPLATE_PROTOENUM_H
#define PICOTEMPLATE_PROTOENUM_H

#include <cstring>
#include <type_traits>

namespace pico::messages {

    /**
     * The integer stored in a nanopb (C) enum field, read without loading it as the enum type.
     *
     * nanopb writes whatever varint arrived on the wire into the field, so it can hold a value
     * that isn't one of the enumerators (e.g. a newer Pi build's message type). In C++, loading
     * an unscoped enum without a fixed underlying type outside its value range is undefined
     * behaviour (UBSan: "load of value 99, which is not a valid value for type ..."), so
     * converters switch on this instead of on the enum itself.
     */
    template<class E>
    inline std::underlying_type_t<E> protoEnumRawValue(const E &field) {
        static_assert(std::is_enum_v<E>, "protoEnumRawValue takes a nanopb enum field");
        std::underlying_type_t<E> raw;
        std::memcpy(&raw, &field, sizeof raw);
        return raw;
    }

} // pico::messages

#endif //PICOTEMPLATE_PROTOENUM_H
