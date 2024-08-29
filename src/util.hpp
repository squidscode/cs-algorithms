#ifndef UTIL_HPP
#define UTIL_HPP

#define C_FUNCTION(ret_type, fname, ...) \
    extern "C" {ret_type (fname)(__VA_ARGS__);} \
    ret_type (fname)(__VA_ARGS__)

#define C_CONSTRUCTOR_NAME(class_name, constructor_version) \
    class_name ## __constructor__ ## constructor_version
#define C_DESTRUCTOR_NAME(class_name, destructor_version) \
    class_name ## __destructor__ ## destructor_version
#define C_METHOD_NAME(class_name, fname) \
    class_name ## __method__ ## fname

#define C_CONSTRUCTOR(constructor_binding, class_name, constructor_version, ...) \
    _C_METHOD((void*) constructor_binding, void*, C_CONSTRUCTOR_NAME(class_name, constructor_version), __VA_ARGS__)

#define C_DESTRUCTOR(class_cast, class_name, destructor_version, ...) \
    extern "C" { \
        void (C_DESTRUCTOR_NAME(class_name, destructor_version))(void *this_object, __VA_ARGS__){ \
            ((class_cast) this_object)->~class_name(__VA_ARGS__); \
        }; \
    }

#define C_METHOD(method_binding, ret_type, class_name, fname, ...) \
    _C_METHOD(method_binding, ret_type, C_METHOD_NAME(class_name, fname), __VA_ARGS__)

#define _C_METHOD(method_binding, ret_type, method_name, ...) \
    extern "C" {ret_type (method_name)(__VA_ARGS__) {return method_binding(__VA_ARGS__);}}

#endif
