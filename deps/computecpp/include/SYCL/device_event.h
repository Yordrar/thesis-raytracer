/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

*******************************************************************************/

/*!
 * @file device_event.h
 *
 * @brief Declaration of the device event
 */

#ifndef RUNTIME_INCLUDE_SYCL_DEVICE_EVENT_H_
#define RUNTIME_INCLUDE_SYCL_DEVICE_EVENT_H_
namespace cl {
namespace sycl {
namespace detail {
class nd_item_base;
}

/**
 * @brief Event for asynchronous operations on the device side.
 * Currently implemented as a no-op.
 *
 * On device it has to be located in private memory - by default,
 * objects are placed in local memory when created within
 * a parallel_for_work_group invocation.
 */
class device_event {
  friend class nd_item<1>;
  friend class nd_item<2>;
  friend class nd_item<3>;
  friend class group<1>;
  friend class group<2>;
  friend class group<3>;

  enum class kind { sycl, dma_event };
  device_event(const kind eventType) : m_eventType{eventType} {}

 public:
#ifdef __SYCL_DEVICE_ONLY__
  device_event(__sycl_event_t internalEvent) : m_kernelEvent(internalEvent) {}
#else   // __SYCL_DEVICE_ONLY__
  device_event() = default;
#endif  // __SYCL_DEVICE_ONLY__

  device_event(const device_event&) = default;
  device_event& operator=(const device_event&) = default;

  device_event(device_event&&) = default;
  device_event& operator=(device_event&&) = default;

  /**
   * @brief Wait for the event to finalize
   */
  void wait() const {
#ifdef __SYCL_DEVICE_ONLY__
    __sycl_event_t internalEvent = m_kernelEvent;
    if (m_eventType == kind::sycl) {
      ::cl::sycl::detail::wait_group_events(1, &internalEvent);
    } else {
      ::cl::sycl::detail::end_dma_transfer();
    }
#else
// Barrier is not needed on the host because the operation is
// synchronous
#endif  // __SYCL_DEVICE_ONLY__
  }

  /**
   * @brief Destroys the device event. Performs a wait.
   */
  ~device_event() { wait(); }

 private:
  kind m_eventType = kind::sycl;
#ifdef __SYCL_DEVICE_ONLY__
  __sycl_event_t m_kernelEvent;
#endif  // __SYCL_DEVICE_ONLY__

} COMPUTECPP_PRIVATE_MEMORY_ATTR;

}  // namespace sycl
}  // namespace cl

#endif  // RUNTIME_INCLUDE_SYCL_DEVICE_EVENT_H_
