/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

*******************************************************************************/

/*!
  @file kernel.h

  @brief This file implements the kernel class as defined by the SYCL 1.2
  specification.
 */

#ifndef RUNTIME_INCLUDE_SYCL_KERNEL_H_
#define RUNTIME_INCLUDE_SYCL_KERNEL_H_

#include "SYCL/accessor_base.h"
#include "SYCL/common.h"
#include "SYCL/device.h"
#include "SYCL/info.h"
#include "SYCL/range.h"

namespace cl {
namespace sycl {

namespace detail {
class transaction;
}

namespace info {
/**
 * @brief Kernel descriptor to query information about a kernel object
 */
enum class kernel : int {
  reference_count, /*!< Get the reference count of the kernel object */
  num_args,        /*!< Get the number of arguments taken by the kernel */
  function_name,   /*!< Get the name of the kernel */
  attributes,      /*!< Get kernel attributes specified by in the source file */
  context,         /*!< Get the context associated with the kernel */
  program          /*!< Get the program associated with the kernel */
};

enum class kernel_work_group : int {
  global_work_size,
  work_group_size,
  compile_work_group_size,
  preferred_work_group_size_multiple,
  private_mem_size
};
}  // namespace info

/// @cond COMPUTECPP_DEV

COMPUTECPP_DEFINE_SYCL_INFO_HANDLER(kernel, cl_kernel_info, cl_kernel)

COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, reference_count,
                                      CL_KERNEL_REFERENCE_COUNT, cl_uint,
                                      cl_uint)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, num_args, CL_KERNEL_NUM_ARGS,
                                      cl_uint, cl_uint)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, function_name,
                                      CL_KERNEL_FUNCTION_NAME, string_class,
                                      char)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, attributes, CL_KERNEL_ATTRIBUTES,
                                      string_class, char)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, context, CL_KERNEL_CONTEXT,
                                      context, cl_context)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(kernel, program, CL_KERNEL_PROGRAM,
                                      program, cl_program)

COMPUTECPP_DEFINE_SYCL_INFO_HOST(kernel, reference_count, 0)
COMPUTECPP_DEFINE_SYCL_INFO_HOST(kernel, num_args, 0)
COMPUTECPP_DEFINE_SYCL_INFO_HOST(kernel, function_name, "")
COMPUTECPP_DEFINE_SYCL_INFO_HOST(kernel, attributes, "")

template <typename syclInfo>
struct opencl_kernel_work_group_info_base;

template <typename syclType, cl_uint clParam>
struct sycl_host_kernel_work_group_info {
  /*!
  @brief Static method that returns a host info parameter as a SYCL type.
  @return Host info parameter.
  */
  static syclType get(const cl::sycl::device& param) = delete;
};

template <>
struct opencl_kernel_work_group_info_base<info::kernel_work_group> {
  using cl_object = cl_kernel;
  using cl_param_object = cl_device_id;
  using cl_info = cl_kernel_work_group_info;
  using cl_function_type = cl_int(COMPUTECPP_CL_API_CALL*)(cl_object,
                                                           cl_param_object,
                                                           cl_info, size_t,
                                                           void*, size_t*);
  static cl_function_type cl_function;
};

template <typename syclInfo>
COMPUTECPP_EXPORT void get_opencl_kernel_work_group_info(
    const typename opencl_kernel_work_group_info_base<
        info::kernel_work_group>::cl_object& object,
    const typename opencl_kernel_work_group_info_base<
        info::kernel_work_group>::cl_param_object& param_object,
    const cl_kernel_work_group_info& param, void* buffer, const size_t size,
    size_t* returnSize);

template <info::kernel_work_group syclParam>
struct opencl_kernel_work_group_info;

namespace info {
template <info::kernel_work_group syclParam>
struct param_traits<info::kernel_work_group, syclParam> {
 public:
  using return_type =
      typename opencl_kernel_work_group_info<syclParam>::sycl_type;
};
}  // namespace info

#define COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(             \
    syclParam, clParam, syclType, clType, clTypeSize)                        \
  template <>                                                                \
  struct opencl_kernel_work_group_info<info::kernel_work_group::syclParam>   \
      : public opencl_kernel_work_group_info_base<info::kernel_work_group> { \
    static const cl_uint cl_param = clParam;                                 \
    using sycl_type = syclType;                                              \
    using cl_type = clType;                                                  \
    static const size_t cl_type_size = clTypeSize;                           \
  }

#define COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(syclParam,   \
                                                           returnValue) \
  template <>                                                           \
  struct sycl_host_kernel_work_group_info<                              \
      typename opencl_kernel_work_group_info<                           \
          info::kernel_work_group::syclParam>::sycl_type,               \
      opencl_kernel_work_group_info<                                    \
          info::kernel_work_group::syclParam>::cl_param> {              \
    static opencl_kernel_work_group_info<                               \
        info::kernel_work_group::syclParam>::sycl_type                  \
    get(cl::sycl::device& d) {                                          \
      return returnValue;                                               \
    }                                                                   \
  }

COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(
    global_work_size, CL_KERNEL_GLOBAL_WORK_SIZE, range<3>, size_t,
    (sizeof(size_t[3])));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(
    work_group_size, CL_KERNEL_WORK_GROUP_SIZE, size_t, size_t, sizeof(size_t));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(
    compile_work_group_size, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, range<3>,
    size_t, sizeof(size_t[3]));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(
    preferred_work_group_size_multiple,
    CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, size_t, size_t,
    sizeof(size_t));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER(
    private_mem_size, CL_KERNEL_PRIVATE_MEM_SIZE, cl_ulong, cl_ulong,
    sizeof(cl_ulong));

COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(global_work_size,
                                                   range<3>(1, 1, 1));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(work_group_size, 1);
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(compile_work_group_size,
                                                   range<3>(0, 0, 0));
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(
    preferred_work_group_size_multiple, 1);
COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST(private_mem_size, 8192);

template <typename syclType, typename clType, cl_uint clParam, size_t size>
syclType get_sycl_kernel_work_group_info(
    const typename opencl_kernel_work_group_info_base<
        info::kernel_work_group>::cl_object& clObject,
    const typename opencl_kernel_work_group_info_base<
        info::kernel_work_group>::cl_param_object& clParamObject,
    bool isHost) {
  /* Create an instance of the SYCL type to be returned. */
  auto syclValue = syclType();

  /* If the requesting object is in hose mode retrieve the info through the host
   * info definitions. */
  if (use_host_info_definitions(isHost)) {
    cl::sycl::device dev = cl::sycl::device();
    /* Retrieve the SYCL info host info specified by clParam. */
    syclValue = sycl_host_kernel_work_group_info<syclType, clParam>::get(dev);
  }
  /* If the requesting object is not in hose mode retrieve the info through the
  OpenCL info API and convert it to the corresponding SYCL type. */
  else {
    /* Allocate memory for the char* based on the size of the value that will be
     * returned. */
    vector_class<char> charVector(size);

    /* Retrieve the value for the OpenCL info parameter specified by clParam. */
    get_opencl_kernel_work_group_info<info::kernel_work_group>(
        clObject, clParamObject, clParam, charVector.data(), size, nullptr);

    /* Calculate the number of elements that were returned. */
    size_t numElems = size / sizeof(clType);

    /* Cast the char* to a pointer to the corresponding OpenCL type. */
    clType* clPtr = reinterpret_cast<clType*>(charVector.data());

    /* Convert the pointer to the OpenCL type to the corresponding SYCL type. */
    syclValue = info_convert<clType*, syclType>::cl_to_sycl(clPtr, numElems);
  }

  /* Return the SYCL value. */
  return syclValue;
}

/// COMPUTECPP_DEV @endcond

/** kernel
 * SYCL Kernel interface.
 *
 * See Section 3.5.4 of the Specification.
 */
class COMPUTECPP_EXPORT kernel {
  friend class handler;

  dkernel_shptr m_impl;

  // This method is available only inside command-group scope
  template <typename dataT, int dimensions, access::mode accessMode,
            access::target accessTarget>
  void set_arg(int argIndex,
               accessor<dataT, dimensions, accessMode, accessTarget> acc_obj,
               handler& cgh /* NOLINT */) {
#ifndef __SYCL_DEVICE_ONLY__
    this->set_arg_impl(argIndex, acc_obj, cgh);
#else
/* Empty for the device, different accessor implementation */
#endif
  }

  // This method is available only inside command-group scope
  template <typename T>
  void set_arg(int argIndex, T scalar_value, handler& cgh /* NOLINT */) {
    this->set_arg_impl(argIndex, &scalar_value, sizeof(T), cgh);
  }

  // This method is available only inside command-group scope
  void set_arg(int argIndex, const sampler& samplerObj,
               handler& cgh /* NOLINT */);

 protected:
  /// @cond COMPUTECPP_DEV
  /**
   * @internal
   * Internal empty kernel
   */
  kernel();

  /**
   * @internal
   * Internal kernel constructor
   */
  kernel(cl_kernel clKernel, dprogram_shptr program);

  /**
   * @internal
   * set_arg method implementation
   */
  void set_arg_impl(int argIndex, accessor_base acc, handler& cgh /* NOLINT */);
  /**
   * @internal
   * set_arg method implementation
   */
  void set_arg_impl(int argIndex, void* scalar_value, size_t size,
                    handler& cgh /* NOLINT */);

  /**
   * @internal
   */
  string_class get_kernel_attributes() const;

  /// COMPUTECPP_DEV @endcond

 public:
  /// @cond COMPUTECPP_DEV

  /**
   * @internal
   */
  dkernel_shptr get_impl() const;

  /**
   * @internal
   */
  explicit kernel(dkernel_shptr detail);

  /// COMPUTECPP_DEV @endcond

  /**
   * @brief Copy constructor. Create a copy of a kernel.
   */
  kernel(const kernel& rhs) : m_impl{rhs.get_impl()} {}

  /**
   * @brief Assignment operator. Assign a copy of a kernel.
   */
  kernel& operator=(const kernel& rhs) {
    m_impl = rhs.get_impl();
    return *this;
  }

  /**
   * @brief Move constructor. Create a copy of a kernel.
   */
  kernel(kernel&& rhs) : m_impl{rhs.get_impl()} {}

  /**
   * @brief Move assignment operator. Assign a copy of a kernel.
   */
  kernel& operator=(kernel&& rhs) {
    this->m_impl = std::move(rhs.m_impl);
    return *this;
  }

  /**
   * @brief Determines if lhs and rhs are equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if same underlying object
   */
  friend inline bool operator==(const kernel& lhs, const kernel& rhs) {
    return (lhs.get_impl() == rhs.get_impl());
  }

  /**
   * @brief Determines if lhs and rhs are not equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if different underlying objects
   */
  friend inline bool operator!=(const kernel& lhs, const kernel& rhs) {
    return !(lhs == rhs);
  }

  COMPUTECPP_TEST_VIRTUAL ~kernel() = default;

  /**
   * Create a kernel object from cl_kernel object created by an OpenCL runtime.
   * @param clKernel an OpenCL kernel created using the OpenCL API.
   * @deprecated Provide a context as the second argument
   */
  COMPUTECPP_DEPRECATED_API("kernel(cl_kernel) was deprecated in SYCL 1.2.1")
  kernel(cl_kernel clKernel);

  /**
   * @brief Constructs a kernel object from an OpenCL cl_kernel object
   * @param clKernel Kernel object created by an OpenCL runtime
   * @param syclContext Context associated with the OpenCL kernel object
   */
  kernel(cl_kernel clKernel, const context& syclContext);

  /**
   * Gets the SYCL program object this kernel is associated to.
   *
   * @return The SYCL program this kernel belongs to
   */
  COMPUTECPP_TEST_VIRTUAL program get_program() const;

  /**
   * Gets the SYCL Context this kernel has been constructed to.
   *
   * @return The SYCL context this kernel belongs to
   */
  COMPUTECPP_TEST_VIRTUAL context get_context() const;

  /**
   * @brief Get the underlying OpenCL kernel object.
   *
   * @return A cl_kernel object usable with the OpenCL API.
   */
  COMPUTECPP_TEST_VIRTUAL cl_kernel get() const;

  /**
   * Query information about the kernel.
   *
   * @tparam param The kernel information descriptor
   * @return The kernel information requested with \ref param.
   */
  template <info::kernel param>
  COMPUTECPP_EXPORT
      typename info::param_traits<info::kernel, param>::return_type
      get_info() const;

  template <info::kernel_work_group param>
  typename info::param_traits<info::kernel_work_group, param>::return_type
  get_work_group_info(const cl::sycl::device& object) const {
    // Call the get method of the info struct, with the
    // cl_kernel_work_group_info
    // template argument.
    cl_kernel me;
    cl_device_id my_device;
    if (object.is_host()) {
      me = nullptr;
      my_device = nullptr;
    } else {
      me = get();
      my_device = object.get();
    }
    return get_sycl_kernel_work_group_info<
        typename opencl_kernel_work_group_info<param>::sycl_type,
        typename opencl_kernel_work_group_info<param>::cl_type,
        opencl_kernel_work_group_info<param>::cl_param,
        opencl_kernel_work_group_info<param>::cl_type_size>(me, my_device,
                                                            object.is_host());
  }

  /**
   * @brief Returns whether the kernel was constructed from a host context
   * @return True if kernel constructed from a host context
   */
  bool is_host() const;

 protected:
  /**
   * @brief Get the underlying OpenCL kernel object without retaining it.
   * @return A cl_kernel object usable with the OpenCL API.
   */
  COMPUTECPP_TEST_VIRTUAL cl_kernel get_no_retain() const;
};

}  // namespace sycl
}  // namespace cl

namespace std {
/*!
@brief provides a specialization for std::hash for the buffer class. An
std::hash<std::shared_ptr<...>> object is created and its function call
operator is used to hash the contents of the shared_ptr. The returned hash is
actually the result of (size_t) object.get_impl().get()
*/
template <>
struct hash<cl::sycl::kernel> {
 public:
  /*!
  @brief enables calling an std::hash object as a function with the object to be
  hashed as a parameter
  @param object the object to be hashed
  @tparam std the std namespace where this specialization resides
  */
  size_t operator()(const cl::sycl::kernel& object) const {
    hash<cl::sycl::dkernel_shptr> hasher;
    return hasher(object.get_impl());
  }
};
}  // namespace std

#undef COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_PARAMETER
#undef COMPUTECPP_DEFINE_SYCL_KERNEL_WORK_GROUP_INFO_HOST

#endif  // RUNTIME_INCLUDE_SYCL_KERNEL_H_

////////////////////////////////////////////////////////////////////////////////
