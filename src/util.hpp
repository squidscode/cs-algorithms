#include <magic.hpp>

#ifndef UTIL_HPP
#define UTIL_HPP

#define C_FUNCTION(ret_type, fname, ...) \
    extern "C" {ret_type (C_FUNCTION_NAME(fname))(__VA_ARGS__);} \
    ret_type (fname)(__VA_ARGS__)

#define C_FUNCTION_NAME(fname) \
    fname
#define C_CONSTRUCTOR_NAME(class_name, constructor_version) \
    class_name ## __constructor__ ## constructor_version
#define C_DESTRUCTOR_NAME(class_name, destructor_version) \
    class_name ## __destructor__ ## destructor_version
#define C_METHOD_NAME(class_name, fname) \
    class_name ## __method__ ## fname

#define C_CONSTRUCTOR(constructor_binding, class_name, constructor_version, ...) \
    extern "C" {void *(C_CONSTRUCTOR_NAME(class_name, constructor_version))(FUNCTION_ARGS(__VA_ARGS__)) { \
        return (void*) new constructor_binding(CALL_ARGS(__VA_ARGS__)); \
    }}

#define C_DESTRUCTOR(class_cast, class_name, destructor_version, ...) \
    __C_DESTRUCTOR(class_cast, class_name, destructor_version, void *this_object, ## __VA_ARGS__)

#define __C_DESTRUCTOR(class_cast, class_name, destructor_version, ...) \
    extern "C" { \
        void (C_DESTRUCTOR_NAME(class_name, destructor_version))(__VA_ARGS__){ \
            delete (class_cast) this_object; \
        }; \
    }

#define C_METHOD(class_cast, ret_type, class_name, fname, ...) \
    _C_METHOD(class_cast, ret_type, C_METHOD_NAME(class_name, fname), fname, void*, __VA_ARGS__)
#define C_VOID_METHOD(class_cast, class_name, fname, ...) \
    extern "C" {void (C_METHOD_NAME(class_name, fname))(FUNCTION_ARGS(void*, __VA_ARGS__)) { \
        ((class_cast) I)->fname(EVAL1(DEFER1(REST) (CALL_ARGS(void*, __VA_ARGS__)))); \
    }}


#define COMBINE_TYPE_AND_ID(type, id) \
    type id
#define GET_IDS(type, id) \
    id
#define FUNCTION_ARGS(...) \
    IF_ELSE(HAS_ARGS(__VA_ARGS__))( \
        MAP_WITH_ID(COMBINE_TYPE_AND_ID, COMMA, __VA_ARGS__), \
        EMPTY() \
    )
#define CALL_ARGS(...) \
    IF_ELSE(HAS_ARGS(__VA_ARGS__))( \
        MAP_WITH_ID(GET_IDS, COMMA, __VA_ARGS__), \
        EMPTY() \
    )

#define _C_METHOD(class_cast, ret_type, c_func_name, method_name, ...) \
    extern "C" {ret_type (c_func_name)(FUNCTION_ARGS(__VA_ARGS__)) { \
        return ((class_cast) I)->method_name(CALL_ARGS(REST(__VA_ARGS__))); \
    }}

#endif
