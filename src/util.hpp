#ifndef UTIL_HPP
#define UTIL_HPP

#define C_FUNCTION(ret_type, fname, ...) \
    extern "C" {ret_type (fname)(__VA_ARGS__);} \
    ret_type (fname)(__VA_ARGS__)

#endif
