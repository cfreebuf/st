// Copyright (c) 2010,2011 .com, Inc. All Rights Reserved
//
// Macros of logging, return code checking, value checking and debugging
// utilities
// Author: gejun
// Date: 2010/07/30 16:00 
#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>      // printf
#include <assert.h>     // assert
#include <iostream>     // std::cout
#include <new>          // std::nothrow
#include "st_errno.h"   // error numbers
#include "st_timer.h"   // timing

namespace st {

// Hint compile that x is LIKELY or UNLIKELY to be true
// Note: DO NOT pass in non-boolean values
#define LIKELY(x)       __builtin_expect((x), 1)
#define UNLIKELY(x)     __builtin_expect((x), 0)

// Delete a pointer and set to NULL
#define ST_DELETE(_ptr_)                        \
    do {                                        \
        decltype(_ptr_) __p;                    \
        __p = _ptr_;                            \
        if (NULL != __p) {                      \
            delete __p;                         \
            __p = NULL;                         \
        }                                       \
    } while(0)

// Delete an array created by new and set to NULL
#define ST_DELETE_ARRAY(_parray_)               \
    do {                                        \
        decltype(_parray_) __a;                 \
        __a = _parray_;                         \
        if (NULL != __a) {                      \
            delete [] __a;                      \
            __a = NULL;                         \
        }                                       \
    } while(0)

// New a _type_
// Returns: NULL for out-of-memory rather than throwing exception
#define ST_NEW(_type_, ...) (new (std::nothrow) _type_(__VA_ARGS__))

// Call constructor of _type_ on _mem_ (as this pointer)
// Note: this is generally the way calling constructor in C++,
//       correspondingly a destructor ~_type_() could be called directly
// FIXME(lidm):compile error occurs here if no __VA_ARGS__!
#define ST_NEW_ON(_mem_, _type_, ...) (new (_mem_) _type_(__VA_ARGS__))
#define ST_NEW_ON_EMPTY(_mem_, _type_) (new (_mem_) _type_)

// New an _type_ array with _len_ elements
#define ST_NEW_ARRAY(_type_, _len_) (new (std::nothrow) _type_[_len_])

// Logging utilities
// #define LOG_TYPE 1
#define LOG_TYPE 0

#define ST_SAY(_fmt_, args...)                                  \
    printf("[%s:%d][%s] " _fmt_ "\n"            \
                , __FILE__, __LINE__, __FUNCTION__, ##args)

#define ST_FATAL(_fmt_, args...)                                \
    printf("[%s:%d][%s] " _fmt_ "\n"              \
                , __FILE__, __LINE__, __FUNCTION__, ##args)

#define ST_WARN(_fmt_, args...)                                 \
    printf("[%s:%d][%s] " _fmt_ "\n"            \
                , __FILE__, __LINE__, __FUNCTION__, ##args)

#define ST_TRACE(_fmt_, args...)                               \
    printf("[%s:%d][%s] " _fmt_  "\n"            \
                , __FILE__, __LINE__, __FUNCTION__, ##args)

#define ST_DEBUG(_fmt_, args...)                                \
    printf("[%s:%d][%s] " _fmt_ "\n"              \
                , __FILE__, __LINE__, __FUNCTION__, ##args)

// Check return values
#ifndef NDEBUG
#define RETURN(_code_)                          \
    do {                                        \
        int __c = _code_;                       \
        if (0 != __c) {                         \
            ST_WARN("returning %d", __c);       \
        }                                       \
        return __c;                             \
    } while(0)

#define RETURN_EX(_code_,_goodcode_)            \
    do {                                        \
        int __c = _code_;                       \
        if (!(__c & (_goodcode_))) {            \
            ST_WARN("returning %d", __c);       \
        }                                       \
        return __c;                             \
    } while(0)
#else  // NDEBUG
#define RETURN(_code_) return (_code_)
#define RETURN_EX(_code_,_goodcode_) return (_code_)
#endif

#ifndef NDEBUG
// following macros attach side effects of reporting invalid values
#define CHECK_BOP(_bop_,_obj_,_value_)                          \
    (((_obj_) _bop_ (_value_))                                  \
     ? (ST_WARN("%s %s %s", #_obj_, #_bop_, #_value_), (_obj_)) \
     : (_obj_))

#define CHECK_NE(_obj_,_value_)  CHECK_BOP(==,_obj_,_value_)
#define CHECK_EQ(_obj_,_value_)  CHECK_BOP(!=,_obj_,_value_)
#define CHECK_GT(_obj_,_value_)  CHECK_BOP(<=,_obj_,_value_)
#define CHECK_GE(_obj_,_value_)  CHECK_BOP(<,_obj_,_value_)
#define CHECK_LT(_obj_,_value_)  CHECK_BOP(>=,_obj_,_value_)
#define CHECK_LE(_obj_,_value_)  CHECK_BOP(>,_obj_,_value_)
#else
#define CHECK_NE(_obj_,_value_)  (_obj_)
#define CHECK_EQ(_obj_,_value_)  (_obj_)
#define CHECK_GT(_obj_,_value_)  (_obj_)
#define CHECK_GE(_obj_,_value_)  (_obj_)
#define CHECK_LT(_obj_,_value_)  (_obj_)
#define CHECK_LE(_obj_,_value_)  (_obj_)
#endif

}

#endif  // _DEBUG_H_
