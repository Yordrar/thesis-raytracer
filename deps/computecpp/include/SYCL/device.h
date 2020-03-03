/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp.

*******************************************************************************/

/*!
  @file device.h

  @brief This file contains the API for the cl::sycl::device class
*/
#ifndef RUNTIME_INCLUDE_SYCL_DEVICE_H_
#define RUNTIME_INCLUDE_SYCL_DEVICE_H_

#include "SYCL/common.h"
#include "SYCL/device_info.h"

namespace cl {
namespace sycl {
namespace detail {
// Implementation class declaration.
class device;

/**
 * Enum class.
 * @brief A backend (SPIR, SPIR-V, etc.) that can be supported by a given
 * device.
 */
enum class device_backend { SPIR, SPIRV, PTX, GCN };

}  // namespace detail

// Forward class declaration
class platform;
class device_selector;
class device;

/**
 * @brief Interface for abstracting and interacting with an underlying
 * cl_device_id object.
 */
class COMPUTECPP_EXPORT device {
 public:
  /**
   * @brief Default Constructor.
   * Constructs a device object in host mode.
   */
  device();

  /**
   * @brief Constructs a device object using a cl_device_id object and retains
   * the cl_device_id object if the device is not in host mode.
   * @param deviceID A cl_device_id object.
   */
  explicit device(cl_device_id deviceID);

  /**
   * @brief Constructs a device using the requested device selector
   * @param deviceSelector the device selector that will provide a device to
   * copy
   */
  explicit device(const device_selector& deviceSelector);

  /**
   * @brief Constructs a device object from another device object and retains
   * the cl_device_id object if the device is not in host mode.
   */
  device(const device& rhs) : m_impl{rhs.m_impl} {}

  /**
   * @brief Constructs a device object by moving another device object
   */
  device(device&& rhs) : m_impl{std::move(rhs.m_impl)} {}

  /**
   * @brief Completely assigns the contents of the device to that of another
   * and retains the cl_device_id object if the device is not in host
   * mode.
   */
  device& operator=(const device& rhs) {
    m_impl = rhs.m_impl;
    return *this;
  }

  /**
   * @brief Completely moves the contents of the device to that of another.
   */
  device& operator=(device&& rhs) {
    this->m_impl = std::move(rhs.m_impl);
    return *this;
  }

  /**
   * @brief Determines if lhs and rhs are equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if same underlying object
   */
  friend inline bool operator==(const device& lhs, const device& rhs) {
    return (lhs.get_impl() == rhs.get_impl());
  }

  /**
   * @brief Determines if lhs and rhs are not equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if different underlying objects
   */
  friend inline bool operator!=(const device& lhs, const device& rhs) {
    return !(lhs == rhs);
  }

  /**
   * @brief Destroys the implementation object.
   */
  COMPUTECPP_TEST_VIRTUAL ~device() = default;

  /** get.
   * @brief Returns the underlying cl_device_id object and retain it. The caller
   * is responsible for releasing the returned cl_device_id.
   * @return The cl_device_id object.
   */
  COMPUTECPP_TEST_VIRTUAL cl_device_id get() const;

  /**
   * @brief Specifies whether the device is in host mode.
   * @return A boolean specifying whether the device is in host mode.
   */
  COMPUTECPP_TEST_VIRTUAL bool is_host() const;

  /**
   * @brief Specifies whether the device is in CPU mode.
   * @return A boolean specifying whether the device is an OpenCL CPU device.
   */
  COMPUTECPP_TEST_VIRTUAL bool is_cpu() const;

  /**
   * @brief @return A boolean specifying whether the device is an OpenCL GPU
   * device.
   */
  COMPUTECPP_TEST_VIRTUAL bool is_gpu() const;

  /**
   * @brief @return A boolean specifying whether the device is an OpenCL
   * Accelerator device.
   */
  COMPUTECPP_TEST_VIRTUAL bool is_accelerator() const;

  /**
   * @brief Gets OpenCL information for the underlying cl_device_id.
   * @tparam param A cl_int specifying the info parameter.
   * @return The retrieved information as the appropriate return type,
   * derived via the get_sycl_info function, defined in info.h.
   */
  template <info::device param>
  typename info::param_traits<info::device, param>::return_type get_info()
      const {
    // Call the get method of the info struct, with the cl_device_info
    // template argument.
    cl_device_id device_id = nullptr;
    if (!this->is_host()) {
      device_id = get();
    }
    return get_sycl_info<info::device,
                         typename opencl_device_info<param>::sycl_type,
                         typename opencl_device_info<param>::cl_type,
                         opencl_device_info<param>::cl_param>(device_id,
                                                              this->is_host());
  }

  /**
   * @brief Specifies whether a specific extension is supported on the device.
   * @param extension A string specifying the extension to check for.
   * @return A boolean specifying whether the extension is supported.
   */
  COMPUTECPP_TEST_VIRTUAL bool has_extension(string_class extension) const;

  /**
     @brief Checks whether the device supports a given backend
     @param backend the backend to check for
     @return true if supported, false otherwise
  */
  COMPUTECPP_TEST_VIRTUAL bool supports_backend(
      const detail::device_backend backend) const;

  /**
   * @brief Determine whether the device features a given device type flag.
   * @param type_flag The device type flag to check for this device.
   * @return true if the device features the flag, false otherwise.
   */
  bool has_type_flag(info::device_type type_flag) const {
    return (get_info<info::device::device_type>() == type_flag);
  }

  /**
   * @brief Gets the platform that the device is associated with.
   * @return A platform object.
   */
  COMPUTECPP_TEST_VIRTUAL platform get_platform() const;

  /**
   * @brief Gets a list of all available devices.
   * @return A vector of device objects.
   */
  static vector_class<device> get_devices(
      info::device_type deviceType = info::device_type::all);

  /**
   * @brief Returns an opaque pointer to the implementation object.
   * @return A pointer to the implementation object.
   **/
  COMPUTECPP_TEST_VIRTUAL ddevice_shptr get_impl() const;

  /**
   * @brief Constructs a device using an existing implementation object.
   */
  explicit device(const ddevice_shptr& impl);

  /**
      @brief Partition device into sub devices evenly.
      @tparam prop Must be info::partition_property::partition_equally.
      @param nbSubDev Desired number of sub devices.
      @return Vector of sub devices.
  */
  template <info::partition_property prop>
  vector_class<device> create_sub_devices(size_t nbSubDev) const;

  /**
      @brief Partition device into sub devices by explicitly stating the number
      of compute units used by each device.
      @tparam prop Must be info::partition_property::partition_by_counts.
      @param counts A vector of sizes for the resulting sub devices.
      @return Vector of sub devices. The number of sub devices created is the
      same as the number of sizes passed.
  */
  template <info::partition_property prop>
  vector_class<device> create_sub_devices(
      const vector_class<size_t>& counts) const;

  /**
      @brief Partition device into sub devices using the provided affinity
      domain.
      @tparam prop Must be
     info::partition_property::partition_by_affinity_domain.
      @param affinityDomain Affinity domain used for the partitioning.
      @return Vector of sub devices.
  */
  template <info::partition_property prop>
  vector_class<device> create_sub_devices(
      info::partition_affinity_domain affinityDomain) const;

 private:
  ddevice_shptr m_impl;
};

/**
  @brief This function converts a cl device object to a sycl device object
  it is used to allow get_info<cl::sycl::device::parent_device> to return a
  sycl device object
*/
template <>
struct info_convert<cl_device_id*, device> {
  /**
  @brief Convert the pointer to the OpenCL type to the corresponding SYCL type.
  if the dereferenced value is nullptr, a host device is returned
  */
  static device cl_to_sycl(cl_device_id* clValue, size_t numElems) {
    // TODO(ComputeCpp): Avoid creation of non host devices of id 0 to follow
    // linter conventions
    if (*clValue != nullptr) {
      return device(*clValue);
    }
    return device();  // ensure a correct host device is returned
  }
};

COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, name);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, vendor);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, version);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, max_work_group_size);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, max_work_item_sizes);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, half_fp_config);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, double_fp_config);
COMPUTECPP_GET_INFO_SPECIALIZATION_DECL(device, codeplay_onchip_memory_size);

/** @brief Template specialisation of create_sub_devices() for
    info::partition_property::partition_equally
*/
template <>
COMPUTECPP_EXPORT vector_class<device>
device::create_sub_devices<info::partition_property::partition_equally>(
    size_t nbSubDev) const;

/** @brief Template specialisation of create_sub_devices() for
    info::partition_property::partition_by_counts
*/
template <>
COMPUTECPP_EXPORT vector_class<device>
device::create_sub_devices<info::partition_property::partition_by_counts>(
    const vector_class<size_t>& counts) const;

/** @brief Template specialisation of create_sub_devices() for
    info::partition_property::partition_by_affinity_domain
*/
template <>
COMPUTECPP_EXPORT vector_class<device> device::create_sub_devices<
    info::partition_property::partition_by_affinity_domain>(
    info::partition_affinity_domain affinityDomain) const;

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
struct hash<cl::sycl::device> {
 public:
  /*!
  @brief enables calling an std::hash object as a function with the object to be
  hashed as a parameter
  @param object the object to be hashed
  @tparam std the std namespace where this specialization resides
  */
  size_t operator()(const cl::sycl::device& object) const {
    hash<cl::sycl::ddevice_shptr> hasher;
    return hasher(object.get_impl());
  }
};
}  // namespace std
#endif  // RUNTIME_INCLUDE_SYCL_DEVICE_H_
