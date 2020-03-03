/*****************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

********************************************************************/

/*!
 * @file predefines.h
 *
 * @brief This file contains common internal runtime pre-processor definitions
 * and the pre-processor definitions required by the SYCL 1.2 specification.
 */

#ifndef RUNTIME_INCLUDE_SYCL_PREDEFINES_H_
#define RUNTIME_INCLUDE_SYCL_PREDEFINES_H_

/// @cond COMPUTECPP_DEV

#define ABACUS_LIBRARY_STATIC 1

/** This file is generated from CMake and it contains some common version
    macros
*/
#include "version.h"
#include <computecpp_export.h>

/** Homogenize the operating system identification macro
 */
#if defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
#define COMPUTECPP_WINDOWS
#if defined(_WIN64)
#define COMPUTECPP_ENV_64
#define COMPUTECPP_PTR_SIZE 8
#else  // _WIN64
#define COMPUTECPP_ENV_32
#define COMPUTECPP_PTR_SIZE 4
#endif  // _WIN64
#endif  // defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
#ifdef __linux__
#if defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
#define COMPUTECPP_ENV_64
#define COMPUTECPP_PTR_SIZE 8
#else  // 64__
#define COMPUTECPP_ENV_32
#define COMPUTECPP_PTR_SIZE 4
#endif  // 64__
#define COMPUTECPP_LINUX
#endif  // __linux__

#ifdef __MINGW32__
#define COMPUTECPP_THREAD_LOCAL __thread
#else
#define COMPUTECPP_THREAD_LOCAL thread_local
#endif  // __MINGW32__

/**
 * When there is a mismatch between the host and the device bitness,
 * we need to ensure the alignment on both platforms matches.
 * To guarantee that, we force it to be aligned to 8, which works on
 * both combinations.
 */
#if defined(COMPUTECPP_ENV_32) && defined(__DEVICE_SPIR64__)
#define COMPUTECPP_PTR_SIZE 8
#elif defined(COMPUTECPP_ENV_64) && defined(__DEVICE_SPIR32__)
#define COMPUTECPP_PTR_SIZE 8
#endif  // defined(COMPUTECPP_ENV_32) && defined(__DEVICE_SPIR64__)

// Used to suppress "calling convention __stdcall ignored for this target"
#if defined(COMPUTECPP_WINDOWS) && defined(__SYCL_DEVICE_ONLY__)
#define COMPUTECPP_CL_API_CALL
#else
#define COMPUTECPP_CL_API_CALL CL_API_CALL
#endif

/// COMPUTECPP_DEV @endcond

#ifndef CL_SYCL_LANGUAGE_VERSION
/** Section 5.6 , CL_SYCL_LANGUAGE_VERSION
 *  @brief Integer reflecting the version number of the SYCL language
 *  being supported by the device compiler.
 */
#define CL_SYCL_LANGUAGE_VERSION 121
#endif  // CL_SYCL_LANGUAGE_VERSION

/// @cond COMPUTECPP_DEV

/* This macro is used for methods that are used in the unit tests and hence need
 * to be
 * specified virtual, but are not required to be virtual for the normal
 * operation of the runtime.
 */
#if COMPUTECPP_BUILD_UNIT_TEST
#define COMPUTECPP_TEST_VIRTUAL virtual
#define COMPUTECPP_TEST_OVERRIDE override
#else
#define COMPUTECPP_TEST_VIRTUAL
#define COMPUTECPP_TEST_OVERRIDE final
#endif  // COMPUTECPP_BUILD_UNIT_TEST

/**
 * Regular expressions are available in VS and in GCC 4.9 and higher
 */
#if defined(_MSC_VER)
#define COMPUTECPP_REGEX_AVAILABLE 1
#elif defined(__aarch64__)
#define COMPUTECPP_REGEX_AVAILABLE 0
#elif defined(__GNUC__)
#if (__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC__MINOR__ > 8)))
#define COMPUTECPP_REGEX_AVAILABLE 1
#else
#define COMPUTECPP_REGEX_AVAILABLE 0
#endif  // __GNUC__
#else
#define COMPUTECPP_REGEX_AVAILABLE 0
#endif  // _MSC_VER

/**
 * @brief Provides a way to specify pragmas to Clang or GCC
 */
#define COMPUTECPP_PRAGMA(message) _Pragma(#message)
#if defined(__clang__)
#define COMPUTECPP_HOST_CXX_DIAGNOSTIC(message) \
  COMPUTECPP_PRAGMA(clang diagnostic message)
#define COMPUTECPP_CXX clang
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPUTECPP_HOST_CXX_DIAGNOSTIC(message) \
  COMPUTECPP_PRAGMA(GCC diagnostic message)
#else
#define COMPUTECPP_HOST_CXX_DIAGNOSTIC(message)
#endif

/// COMPUTECPP_DEV @endcond

namespace cl {
namespace sycl {
namespace detail {

/**
 * @internal
 * ComputeCpp internal error codes. Use to report meaning full errors.
 */
enum class cpp_error_code : unsigned int {
  CPP_NO_ERROR = 0,
  // General Errors
  UNKNOWN_ERROR = 1,
  OUT_OF_HOST_MEMORY = 2,
  RETAIN_CL_OBJECT_ERROR = 3,
  RELEASE_CL_OBJECT_ERROR = 4,
  HOST_MEMORY_ALLOCATION_ERROR = 5,
  NOT_SUPPORTED_ERROR = 6,
  UNREACHABLE_ERROR = 7,
  SYCL_OBJECTS_STILL_ALIVE = 8,
  TARGET_ENV_ERROR = 9,
  TARGET_FORMAT_ERROR = 10,
  NULLPTR_ERROR = 11,
  INVALID_CONFIG_FILE = 12,
  INVALID_CONFIG_OPTION = 13,

  // Program/Kernel issues
  BUILD_PROGRAM_ERROR = 100,
  CREATE_KERNEL_ERROR = 101,
  KERNEL_NOT_FOUND_ERROR = 102,
  GET_PROGRAM_INFO_ERROR = 103,
  GET_KERNEL_INFO_ERROR = 104,
  TARGET_NOT_FOUND_ERROR = 105,
  DEVICE_NOT_FOUND_ERROR = 106,
  CREATE_PROGRAM_FROM_BINARY_ERROR = 107,
  LINK_PROGRAM_ERROR = 108,
  KERNEL_BUILD_ERROR = 109,
  CREATE_PROGRAM_FROM_SOURCE_ERROR = 110,
  CL_SET_KERNEL_ARGUMENT_ERROR = 111,
  RETAIN_KERNEL_ERROR = 112,
  RELEASE_KERNEL_ERROR = 113,
  INVALID_CL_PROGRAM_ERROR = 114,
  DEVICE_UNSUPPORTED_EXTENSIONS_ERROR = 115,
  INVALID_CL_KERNEL_ERROR = 116,
  COMPILE_PROGRAM_ERROR = 117,
  BINARY_NOT_FOUND_ERROR = 118,

  // Execution/Transaction/Command group errors
  CREATE_IMPLICIT_QUEUE_ERROR = 200,
  WAIT_FOR_EVENT_ERROR = 201,
  GET_EVENT_INFO_ERROR = 202,
  SET_USER_EVENT_STATUS_ERROR = 203,
  CREATE_CONTEXT_ERROR = 204,
  CREATE_USER_EVENT_ERROR = 205,
  QUEUE_FINISH_ERROR = 206,
  QUEUE_FLUSH_ERROR = 207,
  ENQUEUE_ERROR = 208,
  RETAIN_CONTEXT_ERROR = 209,
  RELEASE_CONTEXT_ERROR = 210,
  CONTEXT_WITH_NO_DEVICES_ERROR = 211,
  RETAIN_CL_EVENT_ERROR = 212,
  INVALID_CL_EVENT_ERROR = 213,
  RELEASE_CL_EVENT_ERROR = 214,
  SET_CALLBACK_ERROR = 215,
  COMMAND_GROUP_SUBMIT_ERROR = 216,
  KERNEL_EXECUTION_ERROR = 217,
  TRANSACTION_ADD_KERNEL_PARAM_ERROR = 218,
  COMMAND_GROUP_SYNTAX_ERROR = 219,
  MAXIMUM_DEVICES_PER_CONTEXT_ERROR = 220,
  NO_COMMAND_GROUP_AVAILABLE_ERROR = 221,
  ACCESSOR_OUTSIDE_COMMAND_GROUP_ERROR = 222,
  HOST_ACCESSOR_IN_COMMAND_GROUP_ERROR = 223,
  INSUFFICIENT_MEMORY_ON_SUBMIT_ERROR = 224,

  // Local/Workgroup problems
  GET_WORKGROUP_INFO_ERROR = 300,
  WORK_GROUP_SIZE_ERROR = 301,

  // Runtime classes errors
  GET_INFO_ERROR = 400,
  CREATE_SUBDEVICE_ERROR = 401,
  CREATE_DEVICE_ERROR = 402,
  RELEASE_DEVICE_ERROR = 403,
  GET_CL_MEM_OBJ_INFO_ERROR = 404,
  INVALID_CL_MEM_OBJ_ERROR = 405,
  INVALID_CONTEXT_ERROR = 406,
  CREATE_QUEUE_ERROR = 407,
  QUERY_NUMBER_OF_PLATFORMS_ERROR = 408,
  QUERY_PLATFORM_ERROR = 409,
  ACCESSOR_ARGUMENTS_ERROR = 410,
  INCORRECT_ACCESSOR_TYPE_ERROR = 411,
  PROPERTY_ERROR = 412,
  INVALID_CL_DEVICE_ERROR = 413,
  INVALID_CL_QUEUE_ERROR = 414,
  INVALID_CL_PLATFORM_ERROR = 415,
  NO_PROFILING_INFO_ERROR = 416,

  // Buffers/images/samplers errors
  CREATE_BUFFER_ERROR = 500,
  CREATE_SUBBUFFER_ERROR = 501,
  CREATE_IMAGE_ERROR = 502,
  CREATE_SAMPLER_ERROR = 503,
  RELEASE_MEM_OBJECT_ERROR = 504,
  RETAIN_MEM_OBJECT_ERROR = 505,
  GET_CL_MEM_ERROR = 506,
  CREATE_NDRANGE_ERROR = 507,
  MEMORY_OBJECT_UNAVAILABLE_ERROR = 508,
  INVALID_OBJECT_ERROR = 509,
  NULL_BUFFER_ERROR = 510,

  // Profiling API errors
  PROFILING_ENTRY_NOT_FOUND_ERROR = 600,
  CANNOT_WRITE_PROFILING_OUTPUT = 601,
  SET_EVENT_CALLBACK_ERROR = 602,
  JSON_PROFILING_ERROR = 603,

  // Extensions
  EXT_ONCHIP_MEMORY_ERROR = 900,
  EXT_SET_PLANE_ERROR = 901,
};

}  // namespace detail
}  // namespace sycl
}  // namespace cl

/// @cond COMPUTECPP_DEV

/* Macros for outputting a log to standard output or standard error. */
#ifndef __SYCL_DEVICE_ONLY__
/// @internal Internal log macro
#define COMPUTECPP_NOT_IMPLEMENTED(message)                                \
  cl::sycl::detail::trigger_sycl_log(                                      \
      cl::sycl::log_type::not_implemented, __FILE__, __LINE__, CL_SUCCESS, \
      cl::sycl::detail::cpp_error_code::NOT_SUPPORTED_ERROR, nullptr,      \
      message);

/// @internal Internal log macro
#define COMPUTECPP_LOG(logMessage)                     \
  cl::sycl::detail::trigger_sycl_log(                  \
      cl::sycl::log_type::info, __FILE__, __LINE__, 0, \
      cl::sycl::detail::cpp_error_code::CPP_NO_ERROR, nullptr, logMessage);

/// @internal Internal log macro
#define COMPUTECPP_CL_ERROR_CODE(openclErrorCode, cppErrorCode, contextPtr)   \
  cl::sycl::detail::trigger_sycl_log(cl::sycl::log_type::error, __FILE__,     \
                                     __LINE__, openclErrorCode, cppErrorCode, \
                                     contextPtr, nullptr);

/// @internal Internal log macro
#define COMPUTECPP_CL_ERROR_CODE_MSG(openclErrorCode, cppErrorCode,           \
                                     contextPtr, extraInformation)            \
  cl::sycl::detail::trigger_sycl_log(cl::sycl::log_type::error, __FILE__,     \
                                     __LINE__, openclErrorCode, cppErrorCode, \
                                     contextPtr, extraInformation);

#else  // __SYCL_DEVICE_ONLY__
#define COMPUTECPP_NOT_IMPLEMENTED(...)
#define COMPUTECPP_LOG(...)
#define COMPUTECPP_CL_ERROR_CODE(...)
#define COMPUTECPP_CL_ERROR_CODE_MSG(...)
#endif  // __SYCL_DEVICE_ONLY__

/// COMPUTECPP_DEV @endcond

#ifndef __SYCL_DEVICE_ONLY__

#ifdef _MSC_VER

#ifdef _MSC_FULL_VER

#if _MSC_FULL_VER < 170051025
#error "SYCL requires c++11 support"
#endif

#endif  // _MSC_FULL_VER

#else
#if __cplusplus <= 199711L
#error "SYCL requires c++11 support"
#endif
#endif  // _MSC_VER

#endif  //  __SYCL_DEVICE_ONLY__

/*
 * Suppress MSVC warnings
 */
#ifdef _MSC_VER
#pragma warning(push)

// Disable some warnings
#pragma warning(disable : 4251)  // because impl objects used in export classes
#pragma warning(disable : 4275)  // because impl objects used in export classes
#pragma warning(disable : 4996)  // unchecked iterators

// Disable some non-standard extensions
#pragma warning(error : 4204)  // non-constant aggregate initializer
#pragma warning(error : 4238)  // class rvalue used as lvalue
#pragma warning(error : 4239)  // binding temporaries to references
#endif

/// @cond COMPUTECPP_DEV

/*!
@internal
@def COMPUTECPP_VALID_KERNEL_ARG_IF(Condition, Message)
Macro which expands to the attribute computecpp::valid_kernel_arg_if
with the given condition and message. If a value of a type with this parameter
is offloaded to a kernel, if the condition is false an error will be raised,
including the given message in the form of a note.
This is used, for example, to prevent offloading of host accessors to device
code.
@param Condition True if this type can be offloaded, false otherwise.
@param Message Message to print after the error, to clarify what the issue might
be.
*/
#ifndef __SYCL_DEVICE_ONLY__
#define COMPUTECPP_VALID_KERNEL_ARG_IF(Condition, Message)
#else
#define COMPUTECPP_VALID_KERNEL_ARG_IF(Condition, Message) \
  [[computecpp::valid_kernel_arg_if((Condition), (Message))]]
#endif  // __SYCL_DEVICE_ONLY__

// Suppress security warnings
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif  // _CRT_SECURE_NO_WARNINGS
#endif  // _MSC_VER

/// COMPUTECPP_DEV @endcond

// COMPUTECPP_KAI
// If the COMPUTECPP_KAI Macro is defined, the runtime will work around
// some peculiarities of the OpenCL implementation in order to use
// their SPIR backend.
#if defined(COMPUTECPP_KAI)
// COMPUTECPP_KAI reports itself as OpenCL 1.1
#define COMPUTECPP_USE_OPENCL_1_1
#define COMPUTECPP_OPENCL_1_1_PATCH
#endif

// OpenCL implementation with support for setting struct of structs as kernels
#if (defined ANDROID || defined ARM) && !(defined COMPUTECPP_KAI)
#define COMPUTECPP_USE_SEPARATE_KERNEL_ARGUMENTS
#endif  // (defined ANDROID || defined ARM) && !(defined COMPUTECPP_KAI)

/// @cond COMPUTECPP_DEV

namespace cl {
namespace sycl {
namespace detail {
/// parameter_kind.
/// \brief Represents the parameter kind of a SYCL Kernel as understood
//         by the SYCL device compiler. See device compiler documentation
//         for details.
enum class parameter_kind : unsigned {
  /// \brief Represents an invalid parameter.
  invalid = 0,

  /// \brief Represents a normal parameter that should not get
  /// any special treatment (it's a standard layout struct,
  /// or a built-in type).
  normal = 1,

  /// \brief Represents any kind of pointer.
  pointer = 2,

  /// \brief Represents any OpenCL image built-in type (type
  /// BuiltinType::OCLImage1d, OCLImage1dArray, OCLImage1dBuffer,
  /// OCLImage2d, OCLImage2dArray, OCLImage3d).
  ocl_image = 3,

  /// \brief Represents an OpenCL sampler parameter (type
  /// BuiltinType::OCLSampler).
  ocl_sampler = 4,

  /// \brief Represents an OpenCL event parameter (type
  /// BuiltinType::OCLEvent).
  ocl_event = 5
};

/**
  The \ref{COMPUTECPP_CONVERT_ATTR} macro is used to define the opencl_convert
  attribute for the device side, this attribute is used to tell the device
  compiler that the struct should be flattened into it's containing fields when
  being passed to the kernel function.

  The \ref{COMPUTECPP_CONVERT_ATTR_PLACEHOLDER} macro performs the same role as
  COMPUTECPP_CONVERT_ATTR, but is also a way to distinguish a placeholder
  accessor from a regular one.
*/

/**
 * @brief Distinguishes the different types of accessors
 */
enum class parameter_class : int {
  user_defined = 0, /**< Represents an accessor to a used defined type */
  value = 1,        /**< Represents a value accessor */
  placeholder = 2,  /**< Represents a placeholder accessor */
  stream = 3,       /**< Represents a stream */
  sampler = 4,      /**< Represents a sampler */
};
}  // namespace detail
}  // namespace sycl
}  // namespace cl

#ifdef __SYCL_DEVICE_ONLY__
#define COMPUTECPP_CONVERT_ATTR \
  __attribute__((opencl_convert(::cl::sycl::detail::parameter_class::value)))
#define COMPUTECPP_CONVERT_ATTR_PLACEHOLDER \
  __attribute__(                            \
      (opencl_convert(::cl::sycl::detail::parameter_class::placeholder)))
#define COMPUTECPP_CONVERT_ATTR_STREAM \
  __attribute__((opencl_convert(::cl::sycl::detail::parameter_class::stream)))
#define COMPUTECPP_CONVERT_ATTR_SAMPLER \
  __attribute__((opencl_convert(::cl::sycl::detail::parameter_class::sampler)))
#define COMPUTECPP_CONVERT_ATTR_PLANE \
  __attribute__((opencl_convert(::cl::sycl::detail::parameter_class::plane)))

#else
#define COMPUTECPP_CONVERT_ATTR
#define COMPUTECPP_CONVERT_ATTR_PLACEHOLDER
#define COMPUTECPP_CONVERT_ATTR_STREAM
#define COMPUTECPP_CONVERT_ATTR_SAMPLER
#define COMPUTECPP_CONVERT_ATTR_PLANE
#endif

/*******************************************************************************
    COMPUTECPP_ENABLE_IF macro
*******************************************************************************/

/*!
@internal
@def COMPUTECPP_ENABLE_IF(typeName, condition)
Macro for specifying a condition to the method of a template class which
determines whether or not the method is enabled. The macro should be used in
place of a template parameter to a template class method. The condition must be
a compile time boolean expression and is used as the parameter to an
std::enable_if in order to remove the method from overload resolution if the
condition is not met according to the rules of SFINAE. A minor addition to this
is the first parameter to the macro, this parameter must be a typename from the
class template arguments, the reason for this is that SFINAE can only work if
the enable_if is dependant on template arguments of the method itself, rather
than from just the class. So this macro adds an additional template argument to
the method, uses the typename specified as it's default argument and performs a
logical AND with an is_same comparison between the method template argument
typename and the typename specified. This means that the method overload is then
dependant on a method template argument, however that argument is then resolved
to the default at the point of overload resolution, making the is_same
comparison always true and SFINAE can allow the overload to be dropped without
an error.
@param typeName Specify a typename from the class template arguments.
@param condition Specify a compile time boolean expression describing the
context in which the method overload should be enabled.
*/
#define COMPUTECPP_ENABLE_IF(typeName, condition)                              \
  typename overloadDependantT = typeName,                                      \
           typename = typename std::enable_if <                                \
                          std::is_same<typeName, overloadDependantT>::value && \
                      (condition) > ::type

/// COMPUTECPP_DEV @endcond

#ifdef COMPUTECPP_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif  // NOMINMAX
#endif  // COMPUTECPP_WINDOWS

/**
 * COMPUTECPP_DEPRECATED(reason)
 * @brief Macro which marks an interface as deprecated. Uses C++11 generalized
 * attributes for the device compiler and platform specific attributes for the
 * host compiler.
 */
#ifdef __SYCL_DEVICE_ONLY__
#define COMPUTECPP_DEPRECATED_API(reason) [[deprecated(reason)]]
#else
#ifdef COMPUTECPP_WINDOWS
#define COMPUTECPP_DEPRECATED_API(reason) __declspec(deprecated(reason))
#else
#define COMPUTECPP_DEPRECATED_API(reason) __attribute__((deprecated(reason)))
#endif  // COMPUTECPP_WINDOWS
#endif  // __SYCL_DEVICE_ONLY__

/// @cond COMPUTECPP_DEV

#ifdef __SYCL_DEVICE_ONLY__
/**
 * @brief Using this attribute on a class ensures that objects of that class are
 *        allocated in the private address space.
 */
#define COMPUTECPP_PRIVATE_MEMORY_ATTR __attribute__((__offload_private))
#else
#define COMPUTECPP_PRIVATE_MEMORY_ATTR
#endif  // __SYCL_DEVICE_ONLY__

/// COMPUTECPP_DEV @endcond

/**
 * @brief Adding manually cmath header.
 * This sanitizes the potential C Macros defines and ensures all
 * math dependencies are satisfied further down the line.
 */
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#endif  // RUNTIME_INCLUDE_SYCL_PREDEFINES_H_

////////////////////////////////////////////////////////////////////////////////
