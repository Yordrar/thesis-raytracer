/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

*******************************************************************************/

/*!
  @file abacus_types.h

  @brief This file contains the scalar and vector types used for SYCL
  host/abacus interop. The SYCL host is following the C++ standard definitions
  for
  primitive types and abacus is following the OpenCL C 1.2 standard
  definitions. These types reside in the abacus namespace.
*/
#ifndef RUNTIME_INCLUDE_SYCL_ABACUS_TYPES_H_
#define RUNTIME_INCLUDE_SYCL_ABACUS_TYPES_H_

#ifndef __SYCL_DEVICE_ONLY__

#include "SYCL/cl_types.h"
#include "SYCL/cl_vec_types.h"

namespace abacus {

#include "abacus/abacus_config"  // NOLINT due to this header specifically
                                 // is added here so that all the types
                                 // are defined within the abacus
                                 // namespace

typedef abacus_char cl_char;
typedef abacus_uchar cl_uchar;
typedef abacus_short cl_short;
typedef abacus_ushort cl_ushort;
typedef abacus_int cl_int;
typedef abacus_uint cl_uint;
typedef abacus_long cl_long;
typedef abacus_ulong cl_ulong;
typedef abacus_float cl_float;
typedef abacus_double cl_double;

typedef abacus_char2 cl_char2;
typedef abacus_char3 cl_char3;
typedef abacus_char4 cl_char4;
typedef abacus_char8 cl_char8;
typedef abacus_char16 cl_char16;

typedef abacus_uchar2 cl_uchar2;
typedef abacus_uchar3 cl_uchar3;
typedef abacus_uchar4 cl_uchar4;
typedef abacus_uchar8 cl_uchar8;
typedef abacus_uchar16 cl_uchar16;

typedef abacus_short2 cl_short2;
typedef abacus_short3 cl_short3;
typedef abacus_short4 cl_short4;
typedef abacus_short8 cl_short8;
typedef abacus_short16 cl_short16;

typedef abacus_ushort2 cl_ushort2;
typedef abacus_ushort3 cl_ushort3;
typedef abacus_ushort4 cl_ushort4;
typedef abacus_ushort8 cl_ushort8;
typedef abacus_ushort16 cl_ushort16;

typedef abacus_int2 cl_int2;
typedef abacus_int3 cl_int3;
typedef abacus_int4 cl_int4;
typedef abacus_int8 cl_int8;
typedef abacus_int16 cl_int16;

typedef abacus_uint2 cl_uint2;
typedef abacus_uint3 cl_uint3;
typedef abacus_uint4 cl_uint4;
typedef abacus_uint8 cl_uint8;
typedef abacus_uint16 cl_uint16;

using cl_long2 = abacus_vector<abacus_long, 2>;
using cl_long3 = abacus_vector<abacus_long, 3>;
using cl_long4 = abacus_vector<abacus_long, 4>;
using cl_long8 = abacus_vector<abacus_long, 8>;
using cl_long16 = abacus_vector<abacus_long, 16>;

using cl_ulong2 = abacus_vector<abacus_ulong, 2>;
using cl_ulong3 = abacus_vector<abacus_ulong, 3>;
using cl_ulong4 = abacus_vector<abacus_ulong, 4>;
using cl_ulong8 = abacus_vector<abacus_ulong, 8>;
using cl_ulong16 = abacus_vector<abacus_ulong, 16>;

typedef abacus_float2 cl_float2;
typedef abacus_float3 cl_float3;
typedef abacus_float4 cl_float4;
typedef abacus_float8 cl_float8;
typedef abacus_float16 cl_float16;

typedef abacus_double2 cl_double2;
typedef abacus_double3 cl_double3;
typedef abacus_double4 cl_double4;
typedef abacus_double8 cl_double8;
typedef abacus_double16 cl_double16;

template <typename T>
struct convert_abacus_sycl;

template <>
struct convert_abacus_sycl<char> {
  typedef char abacus_type;
  typedef unsigned char abacus_utype;
  typedef cl::sycl::cl_uchar sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<char, dims>> {
  typedef abacus_vector<char, dims> abacus_type;
  typedef abacus_vector<unsigned char, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_uchar, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_char> {
  typedef signed char abacus_type;
  typedef unsigned char abacus_utype;
  typedef cl::sycl::cl_uchar sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_char, dims>> {
  typedef abacus_vector<signed char, dims> abacus_type;
  typedef abacus_vector<unsigned char, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_uchar, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_uchar> {
  typedef unsigned char abacus_type;
  typedef unsigned char abacus_utype;
  typedef cl::sycl::cl_uchar sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_uchar, dims>> {
  typedef abacus_vector<abacus_uchar, dims> abacus_type;
  typedef abacus_vector<abacus_uchar, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_uchar, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_short> {
  typedef abacus_short abacus_type;
  typedef abacus_ushort abacus_utype;
  typedef cl::sycl::cl_ushort sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_short, dims>> {
  typedef abacus_vector<int16_t, dims> abacus_type;
  typedef abacus_vector<uint16_t, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ushort, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_ushort> {
  typedef abacus_ushort abacus_type;
  typedef abacus_ushort abacus_utype;
  typedef cl::sycl::cl_ushort sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_ushort, dims>> {
  typedef abacus_vector<abacus_ushort, dims> abacus_type;
  typedef abacus_vector<abacus_ushort, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ushort, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_uint> {
  typedef abacus_uint abacus_type;
  typedef abacus_uint abacus_utype;
  typedef cl::sycl::cl_uint sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_uint, dims>> {
  typedef abacus_vector<abacus_uint, dims> abacus_type;
  typedef abacus_vector<abacus_uint, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_uint, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_int> {
  typedef int abacus_type;
  typedef unsigned int abacus_utype;
  typedef cl::sycl::cl_uint sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_int, dims>> {
  typedef abacus_vector<int, dims> abacus_type;
  typedef abacus_vector<unsigned int, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_uint, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<long> {
  typedef cl_long abacus_type;
  typedef cl_ulong abacus_utype;
  typedef cl::sycl::cl_ulong sycl_utype;
};

template <>
struct convert_abacus_sycl<long long> {
  typedef cl_long abacus_type;
  typedef cl_ulong abacus_utype;
  typedef cl::sycl::cl_ulong sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<long, dims>> {
  typedef abacus_vector<abacus_long, dims> abacus_type;
  typedef abacus_vector<abacus_ulong, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ulong, dims> sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<long long, dims>> {
  typedef abacus_vector<abacus_long, dims> abacus_type;
  typedef abacus_vector<abacus_ulong, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ulong, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<unsigned long> {
  typedef abacus_ulong abacus_type;
  typedef abacus_ulong abacus_utype;
  typedef cl::sycl::cl_ulong sycl_utype;
};

template <>
struct convert_abacus_sycl<unsigned long long> {
  typedef abacus_ulong abacus_type;
  typedef abacus_ulong abacus_utype;
  typedef cl::sycl::cl_ulong sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<unsigned long, dims>> {
  typedef abacus_vector<abacus_ulong, dims> abacus_type;
  typedef abacus_vector<abacus_ulong, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ulong, dims> sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<unsigned long long, dims>> {
  typedef abacus_vector<abacus_ulong, dims> abacus_type;
  typedef abacus_vector<abacus_ulong, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_ulong, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_float> {
  typedef abacus_float abacus_type;
  typedef abacus_float abacus_utype;
  typedef cl::sycl::cl_float sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_float, dims>> {
  typedef abacus_vector<abacus_float, dims> abacus_type;
  typedef abacus_vector<abacus_float, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_float, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_double> {
  typedef abacus_double abacus_type;
  typedef abacus_double abacus_utype;
  typedef cl::sycl::cl_double sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::cl_double, dims>> {
  typedef abacus_vector<abacus_double, dims> abacus_type;
  typedef abacus_vector<abacus_double, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_double, dims> sycl_utype;
};

template <>
struct convert_abacus_sycl<cl::sycl::cl_half> {
  typedef cl::sycl::cl_float abacus_type;
  typedef cl::sycl::cl_float abacus_utype;
  typedef cl::sycl::cl_float sycl_utype;
};

template <int dims>
struct convert_abacus_sycl<cl::sycl::vec<cl::sycl::half, dims>> {
  typedef abacus_vector<cl::sycl::cl_float, dims> abacus_type;
  typedef abacus_vector<cl::sycl::cl_float, dims> abacus_utype;
  typedef cl::sycl::vec<cl::sycl::cl_float, dims> sycl_utype;
};

/**
 * @brief Converts a SYCL type to an Abacus type.
 * @tparam T Any SYCL type.
 */
template <typename T>
struct sycl_to_abacus;

/**
 * @brief Converts an Abacus type to a SYCL type.
 * @tparam T Any Abacus type.
 */
template <typename T>
struct abacus_to_sycl;

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_char> {
  using type = abacus_char;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct abacus_to_sycl<abacus_char> {
  using type = cl::sycl::cl_char;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_uchar> {
  using type = abacus_uchar;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_uchar> {
  using type = cl::sycl::cl_uchar;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_short> {
  using type = abacus_short;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_short> {
  using type = cl::sycl::cl_short;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_ushort> {
  using type = abacus_ushort;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_ushort> {
  using type = cl::sycl::cl_ushort;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_int> {
  using type = abacus_int;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_int> {
  using type = cl::sycl::cl_int;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_uint> {
  using type = abacus_uint;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_uint> {
  using type = cl::sycl::cl_uint;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_long> {
  using type = abacus_long;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_long> {
  using type = cl::sycl::cl_long;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_ulong> {
  using type = abacus_ulong;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_ulong> {
  using type = cl::sycl::cl_ulong;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_half> {
  using type = abacus_float;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_float> {
  using type = abacus_float;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_float> {
  using type = cl::sycl::cl_float;
};

/**
 * @ref sycl_to_abacus.
 */
template <>
struct sycl_to_abacus<cl::sycl::cl_double> {
  using type = abacus_double;
};

/**
 * @ref abacus_to_sycl.
 */
template <>
struct abacus_to_sycl<abacus_double> {
  using type = cl::sycl::cl_double;
};

/**
 * @ref sycl_to_abacus, specialized for vec
 */
template <typename T, int N>
struct sycl_to_abacus<cl::sycl::vec<T, N>> {
  using type = abacus_vector<typename sycl_to_abacus<T>::type, N>;
};

/**
 * @ref sycl_to_abacus, specialized for 1-elem vec
 */
template <typename T>
struct sycl_to_abacus<cl::sycl::vec<T, 1>> : sycl_to_abacus<T> {};

/**
 * @ref abacus_to_sycl, specialized for abacus_vector
 */
template <typename T, int N>
struct abacus_to_sycl<abacus_vector<T, N>> {
  using type = cl::sycl::vec<typename abacus_to_sycl<T>::type, N>;
};

/**
 * @ref sycl_to_abacus.
 */
template <typename T>
using sycl_to_abacus_t = typename sycl_to_abacus<T>::type;

/**
 * @ref abacus_to_sycl.
 */
template <typename T>
using abacus_to_sycl_t = typename sycl_to_abacus<T>::type;
}  // namespace abacus

#endif  // __SYCL_DEVICE_ONLY__
#endif  //  RUNTIME_INCLUDE_SYCL_ABACUS_TYPES_H_
