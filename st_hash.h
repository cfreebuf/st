// Copyright (c) 2010,2011 .com, Inc. All Rights Reserved
//
// Compute hash values
// Author: gejun@.com
// Date: Dec 4 17:01:02 CST 2010
#pragma once
#ifndef _FUN_HASH_HPP_
#define _FUN_HASH_HPP_

#include <string>
#include "base_function_object.hpp"

namespace st {

// This is for compatibility with existing code, don't use it
typedef size_t hash_value_t; 
#define compute_hash(...) hash(__VA_ARGS__)

// Overloads of primitive types
inline size_t hash(char);
inline size_t hash(unsigned char);
inline size_t hash(short);
inline size_t hash(unsigned short);
inline size_t hash(int);
inline size_t hash(unsigned int);
inline size_t hash(long);
inline size_t hash(unsigned long);
inline size_t hash(long long);
inline size_t hash(unsigned long long);
inline size_t hash(const char*);
inline size_t hash(char*);
inline size_t hash(const std::string&);
template <typename _T> size_t hash(const _T* p) { return (size_t)p; }
template <typename _T> size_t hash(_T* p)       { return (size_t)p; }

inline size_t hash(const void* __restrict p_data, size_t size);
inline unsigned int murmur_hash2(const void* key, int len, unsigned int seed);

inline size_t _normalize(int v);
inline size_t _normalize(unsigned int v);

// The function object corresponding to hash(1)
template <typename _T>
struct Hash : public BaseFunctionObject<size_t(_T)> {
    size_t operator()(const _T& v) const
    {
        return hash(v);
    } 
};

}  // namespace st

#include "detail/st_hash_inl.h"

#endif  // _FUN_HASH_HPP_

