#pragma once

#include <type_traits>

#define DEFINE_CONNECTION(Signal, Name) \
    template<typename... CallArgs> \
    inline sigslot::connection connect##Name(CallArgs&&... args) { \
        return Signal.connect(std::forward<CallArgs>(args)...); \
    }

#define DEFINE_SCOPED_CONNECTION(Signal, Name) \
    template<typename... CallArgs> \
    inline sigslot::connection connect##Name(CallArgs&&... args) { \
        return Signal.connect_scoped(std::forward<CallArgs>(args)...); \
    }
