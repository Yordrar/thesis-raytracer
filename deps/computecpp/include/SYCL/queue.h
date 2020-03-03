/**************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

****************************************************************/

/*!
  @file queue.h
  @brief This file contains the queue class as define in the SYCL 1.2
  specification.
 */

#ifndef RUNTIME_INCLUDE_SYCL_QUEUE_H_
#define RUNTIME_INCLUDE_SYCL_QUEUE_H_

#include "SYCL/command_group.h"
#include "SYCL/common.h"
#include "SYCL/context.h"
#include "SYCL/device.h"
#include "SYCL/error.h"
#include "SYCL/event.h"
#include "SYCL/info.h"
#include "SYCL/property.h"

namespace cl {
namespace sycl {

/// COMPUTECPP_DEV @endcond
class device_selector;
class handler;
/// @cond COMPUTECPP_DEV

namespace info {

/// @brief The queue_profiling flag type definition
using queue_profiling = bool;

/**
 * @brief Queue information descriptors
 */
enum class queue : int {
  reference_count, /*!< Query the reference count of the queue */
  device,          /*!< Query the device associate to the queue */
  context,         /*!< Query the context associate to the queue */
  queue_profiling  /*!< Query the if the queue profiling is enabled */
};

}  // namespace info

/// @cond COMPUTECPP_DEV

COMPUTECPP_DEFINE_SYCL_INFO_HANDLER(queue, cl_command_queue_info,
                                    cl_command_queue)

COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(queue, reference_count,
                                      CL_QUEUE_REFERENCE_COUNT, cl_uint,
                                      cl_uint)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(queue, context, CL_QUEUE_CONTEXT,
                                      cl::sycl::context, cl_context)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(queue, device, CL_QUEUE_DEVICE,
                                      cl::sycl::device, cl_device_id)
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(queue, queue_profiling,
                                      CL_QUEUE_PROPERTIES,
                                      info::queue_profiling,
                                      cl_command_queue_properties)

COMPUTECPP_DEFINE_SYCL_INFO_HOST(queue, reference_count, 0)
COMPUTECPP_DEFINE_SYCL_INFO_HOST(queue, context, cl::sycl::context())
COMPUTECPP_DEFINE_SYCL_INFO_HOST(queue, device, cl::sycl::device())
COMPUTECPP_DEFINE_SYCL_INFO_HOST(queue, queue_profiling, true)

template <>
struct info_convert<cl_command_queue_properties*, info::queue_profiling> {
  static info::queue_profiling cl_to_sycl(cl_command_queue_properties* clValue,
                                          size_t numElems) {
    for (size_t i = 0; i < numElems; i++) {
      if (clValue[i] & CL_QUEUE_PROFILING_ENABLE) {
        return true;
      }
    }
    return false;
  }
};

using command_group_functor_t = function_class<void(handler&)>;

/// COMPUTECPP_DEV @endcond

namespace property {
namespace queue {

/**
 * @brief The enable_profiling property adds the requirement that the SYCL
 *        runtime must capture profiling information for the command groups that
 *        are submitted from this SYCL queue and provide said information via
 *        the SYCL event class get_profiling_info member function, if the
 *        associated SYCL device supports queue profiling
 */
class COMPUTECPP_EXPORT enable_profiling : public detail::property_base {
 public:
  enable_profiling()
      : detail::property_base(detail::property_enum::enable_profiling) {}
};

}  // namespace queue
}  // namespace property

/**
The cl::sycl::queue object is the SYCL abstraction of the OpenCL object
cl_command_queue. It is
responsible for constructing the OpenCL cl_command_queue object and all OpenCL
API functions that
involve enqueuing. It can be constructed using either a
cl::sycl::device_selector, a cl::sycl::context
or using default behaviour. As the cl::sycl::queue object can be constructed
using different methods,
it maintains the ownership over objects that it can potentially be responsible
for constructing and
destructing.
 */
class COMPUTECPP_EXPORT queue {
 public:
  /**
   * @brief Constructs a queue using a default device selector.
   * @param propList List of queue properties
   */
  explicit queue(const property_list& propList = {})
      : queue(async_handler{}, propList) {}

  /**
   * @brief Constructs a queue using a default device selector.
   * @param asyncHandler User defined \ref async_handler
   * @param propList List of queue properties
   */
  queue(const async_handler& asyncHandler, const property_list& propList = {});

  /**
   * @brief Constructs a queue using a user defined device selector. The device
   *        selector \ref device_selector::select_device() is called by the
   *        constructor to get the device to construct the queue.
   * @param deviceSelector User defined \ref device_selector
   * @param propList List of queue properties
   */
  queue(const device_selector& deviceSelector,
        const property_list& propList = {})
      : queue(deviceSelector, async_handler{}, propList) {}

  /**
   * @brief Constructs a queue using a user defined device selector. The device
   *        selector \ref device_selector::select_device() is called by the
   *        constructor to get the device to construct the queue.
   * @param deviceSelector User defined \ref device_selector
   * @param asyncHandler User defined \ref async_handler
   * @param propList List of queue properties
   */
  queue(const device_selector& deviceSelector,
        const async_handler& asyncHandler, const property_list& propList = {});

  /**
   * @brief Construct a queue from a given device, creating an implicit context
   *        in the process.
   * @param dev The device to use to create the queue
   * @param propList List of queue properties
   */
  queue(const device& dev, const property_list& propList = {})
      : queue(dev, async_handler{}, propList) {}

  /**
   * @brief Construct a queue from a given device, creating an implicit context
   *        in the process.
   * @param dev The device to use to create the queue
   * @param asyncHandler User defined \ref async_handler
   * @param propList List of queue properties
   */
  queue(const device& dev, const async_handler& asyncHandler,
        const property_list& propList = {});

  /**
   * @brief Constructs a queue using a user defined device selector against a
   *        specific context. The device selector
   *        \ref device_selector::select_device() is called by the constructor
   *        to get the device to construct the queue.
   * @param syclContext Context in which the queue will be created.
   * @param selector Used to get a device from the context
   * @param propList List of queue properties
   */
  queue(const context& syclContext, const device_selector& selector,
        const property_list& propList = {})
      : queue(syclContext, selector, async_handler{}, propList) {}

  /**
   * @brief Constructs a queue using a user defined device selector against a
   *        specific context. The device selector
   *        \ref device_selector::select_device() is called by the constructor
   *        to get the device to construct the queue.
   * @param syclContext Context in which the queue will be created.
   * @param selector Used to get a device from the context
   * @param propList List of queue properties
   */
  queue(const context& syclContext, const device_selector& selector,
        const async_handler& asyncHandler, const property_list& propList = {});

  /**
   * @brief Construct a queue object from a given OpenCL object
   * @param clQueue a valid OpenCL object for a command queue
   * @param s_context a valid OpenCL context
   * @param asyncHandler User defined \ref async_handler
   */
  queue(cl_command_queue clqueue, const context& s_context,
        const async_handler& asyncHandler = {});

  /**
   * @brief Construct a queue object by copying the contents of a
   * given queue object.
   * @param rhs a queue to be copied over to the returned object
   */
  queue(const queue& rhs) : m_impl{rhs.m_impl} {}

  /**
   * @brief Completely assigns the contents of the queue to that of another
   * queue.
   */
  queue& operator=(const queue& rhs) {
    this->m_impl = rhs.m_impl;
    return *this;
  }

  /**
   * @brief Completely moves the contents of a queue to that of another queue.
   */
  queue(queue&& rhs) : m_impl{std::move(rhs.m_impl)} {}

  /**
   * @brief Completely moves the contents of a queue to that of another queue.
   */
  queue& operator=(queue&& rhs) {
    m_impl = std::move(rhs.m_impl);
    return *this;
  }

  /**
   * @brief Determines if lhs and rhs are equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if same underlying object
   */
  friend inline bool operator==(const queue& lhs, const queue& rhs) {
    return (lhs.get_impl() == rhs.get_impl());
  }

  /**
   * @brief Determines if lhs and rhs are not equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if different underlying objects
   */
  friend inline bool operator!=(const queue& lhs, const queue& rhs) {
    return !(lhs == rhs);
  }

  /**
   * @brief Returns whether this SYCL queue was constructed with the property
   *        specified by propertyT
   * @tparam propertyT Property to check for
   * @return True if queue constructed with the property
   */
  template <typename propertyT>
  bool has_property() const {
    return this->get_properties().template has_property<propertyT>();
  }

  /**
   * @brief Returns a copy of the property of type propertyT that this SYCL
   *        queue was constructed with. Throws an error if the queue was not
   *        constructed with the property.
   * @tparam propertyT Property to retrieve
   * @return Copy of the property
   */
  template <typename propertyT>
  propertyT get_property() const {
    return this->get_properties().template get_property<propertyT>();
  }

  /**
   * Determine if the queue is executing kernels on the host.
   * @return true if the queue is executing kernels on the host.
   */
  COMPUTECPP_TEST_VIRTUAL bool is_host() const;

  /**
   * Gets OpenCL information for the queue.
   * @tparam param A \ref info::queue descriptor specifying the information to
   * retrieve.
   * @return The retrieved information as the appropriate return type
   */
  template <info::queue param>
  COMPUTECPP_EXPORT typename info::param_traits<info::queue, param>::return_type
  get_info() const;

  /**
   * Returns the context associate to the queue.
   * @return The context object.
   */
  COMPUTECPP_TEST_VIRTUAL context get_context() const;

  /**
   *  Returns the underlying OpenCL cl_command_queue object.
   *  @return an OpenCL cl_command_queue object.
   */
  cl_command_queue get() const;

  /**
   * Performs a blocking wait for the completion of all
   * enqueued tasks in the queue. Synchronous errors
   * are reported through C++ exceptions.
   */
  void wait();

  /**
   * Returns the Device associated with the queue.
   * @return The device object used with the queue.
   */
  device get_device() const;

  /// @cond COMPUTECPP_DEV
  /**
   * Returns the implementation of the class.
   * @internal
   */
  COMPUTECPP_TEST_VIRTUAL dqueue_shptr get_impl() const;

  /**
   * Creates a queue using a specific implementation object.
   * @internal
   * @param detail::queue impl Implementation to use for the queue.
   */
  explicit queue(dqueue_shptr impl);

  /// COMPUTECPP_DEV @endcond

  /**
   * Performs a blocking wait for the completion of all
   * enqueued tasks in the queue. Synchronous errors
   * are reported through C++ exceptions. Asynchronous errors are reported to
   * the async_handler if provided (lost otherwise)
   */
  void wait_and_throw();

  /**
   * Report any unreported asynchronous errors via the
   * async_handler if provided (lost otherwise)
   */
  void throw_asynchronous();

  /**
   * Submits a command group functor to execution.
   * @tparam T The command group type
   * @param cgf The command group functor
   */
  template <typename T>
  inline event submit(T cgf) {
    auto cg = detail::command_group(this->get_impl());
    return cg.submit_handler(
        cgf, nullptr,
        detail::get_handler_tag(cgf, detail::select_handler_overload{}));
  }

  /**
   * Submits a command group functor to execution with
   * a fallback queue. If an error occur during the execution of the kernel on
   * the current queue, the runtime will try to run the kernel on the fallback
   * queue.
   *
   * @tparam T The command group type
   * @param cgf The command group functor
   * @param queue The fallback queue to use in case of error.
   */
  template <typename T>
  inline event submit(T cgf, const cl::sycl::queue& queue) {
    auto cg = detail::command_group(this->get_impl());
    return cg.submit_handler(
        cgf, queue.get_impl(),
        detail::get_handler_tag(cgf, detail::select_handler_overload{}));
  }

 protected:
  /// @cond COMPUTECPP_DEV

  /** queue.
   Assigns the cl::sycl::context object provided and assigns the
   cl::sycl::enum_execution_mode value
   from the cl::sycl::context object provided. Constructs the OpenCL
   cl_command_queue object using the
   cl::sycl::context object.
   @param syclContext a pointer to a valid context object.
   */
  explicit queue(dcontext_shptr syclContext);

  /** m_impl.
   * Pointer to the class implementation.
   */
  dqueue_shptr m_impl;

  /**
   * @brief Obtain the properties
   */
  property_list get_properties() const;

  /// COMPUTECPP_DEV @endcond
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
struct hash<cl::sycl::queue> {
 public:
  /*!
  @brief enables calling an std::hash object as a function with the object to be
  hashed as a parameter
  @param object the object to be hashed
  @tparam std the std namespace where this specialization resides
  */
  size_t operator()(const cl::sycl::queue& object) const {
    hash<cl::sycl::dqueue_shptr> hasher;
    return hasher(object.get_impl());
  }
};
}  // namespace std
////////////////////////////////////////////////////////////////////////////////

#endif  // RUNTIME_INCLUDE_SYCL_QUEUE_H_

////////////////////////////////////////////////////////////////////////////////
