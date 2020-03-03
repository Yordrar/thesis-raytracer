/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp
*******************************************************************************/

/*!
  @file accessor.h

  @brief This file contains the API for the public facing
  @ref cl::sycl::accessor class


  @detail This file contains:
  - @ref ACCESSOR_ALIGNMENT
  - @ref accessor

  The template class accessor is defined entirely in the header file and is
  defined for both the host side and the device side.

  The interface for the accessor class is vastly different depending on the
  specialization of the template arguments for access mode, access target,
  dimensions, and element type. This is implemented using enable if to enable
  only
  the constructors and operators that are relevant for a particular
  specialization
  to be available.

  On the host side the accessor class does not contain any fields and inherits
  its members from its non-templated base class accessor_base. On the device
  side the accessor class contains a single field, that being the device
  arguments, which varies depending on the specialization. On the device side it
  also inherits from the non-template base class accessor_base, however this is
  in
  order to make the declaration of the public interface functions available to
  device compiler when parsing host side only code.

  Common macros and structs are defined in accessor_args.h.

  The accessor_base class is defined in accessor_base.h.

  The subscript_op and sampler_op classes are defined in accessor_ops.h.
*/

#ifndef RUNTIME_INCLUDE_SYCL_ACCESSOR_H_
#define RUNTIME_INCLUDE_SYCL_ACCESSOR_H_

#include "SYCL/accessor_base.h"
#include "SYCL/accessor_ops.h"
#include "SYCL/atomic.h"
#include "SYCL/buffer.h"
#include "SYCL/common.h"
#include "SYCL/id.h"
#include "SYCL/image.h"
#include "SYCL/type_traits.h"
#include "SYCL/vec_swizzles_impl.h"
#include "computecpp/gsl/gsl"

namespace cl {
namespace sycl {

/*! @cond COMPUTECPP_DEV */

/*******************************************************************************
    diagrams
*******************************************************************************/
/*!

The host side accessor class structure is:

         ,--------------------------.
         |detail::host_arg_container|
         `--------------------------'
                     |
              ,------o------.
              |accessor_base|
              `-------------'
                     |
          ,-----------------------.
          |detail::accessor_common|
          `-----------------------'
                     |
                 ,--------.
                 |accessor|
                 `--------'

The device side accessor class structure is:

        ,------------------.  ,----------.
        |device_index_array|  |device_arg|
        `------------------'  `----------'
                |___________________|
                        |
             ,----------o---------.
             |device_arg_container|
             `--------------------'
                        |
                        |
              ,---------o----------.
              |accessor_device_base|
              `--------------------'
                        |
             ,-----------------------.
             |detail::accessor_common|
             `-----------------------'
                        |
                    ,--------.
                    |accessor|
                    `--------'

Important things to know:
 - "device_arg_container" and "host_arg_container" must be seen by the device
     compiler
   - "device_arg_container" has an attribute that refers to "host_arg_container"
 - "host_arg_container" is pulled by the stubfile
 - "device_arg_container" and "host_arg_container" content can differ
 - the accessor inheritance hierarchy must be equivalent (same tree layout)
*/

/*******************************************************************************
    ACCESSOR_ALIGNMENT macros
*******************************************************************************/

/*!
@def COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT()
The COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT macro is used for specifying the
alignment of the
accessor class for Windows, there reason for two separate macros is that on
Windows the attribute is placed at the start of declaration, whereas on all
other platforms the attribute is placed at the end of the declaration.
*/
/*!
@def COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT()
The COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT macro is used for specifying the
alignment of the
accessor class for non-Windows, there reason for two separate macros is that on
Windows the attribute is placed at the start of declaration, where as on all
other platforms the attribute is placed at the end of the declaration.
*/
#ifdef COMPUTECPP_WINDOWS
#define COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT \
  __declspec(align(COMPUTECPP_PTR_SIZE))
#define COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT
#else
#define COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT
#if defined(COMPUTECPP_KAI)
#define COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT __attribute__((aligned(8)))
#else
#define COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT \
  __attribute__((aligned(COMPUTECPP_PTR_SIZE)))
#endif
#endif

/*******************************************************************************
    accessor traits
*******************************************************************************/

namespace detail {

/**
 * @brief Calculates the index of the error code location within the buffer
 * @param size The size in bytes of the original buffer
 * @return Index of the error code location within the buffer
 */
constexpr size_t error_code_begin(const size_t size) noexcept {
  return (size / sizeof(int)) + 1;
}

/*!
 * @brief Template trait which provides a boolean value which specifies whether
 * a particular type is a value type for coordinates of a particular
 * dimensionality.
 * @tparam dimensions The required dimensionality for the type.
 * @tparam coordT The type which is checked for being a valid coordinates type.
 */
template <int dimensions, typename coordT>
struct is_coords {
  /*! The value result of the type trait @ref is_coords */
  static constexpr auto value =
      (detail::is_same_basic_type<typename coordT::element_type,
                                  cl::sycl::cl_int>::value ||
       detail::is_same_basic_type<typename coordT::element_type,
                                  cl::sycl::cl_float>::value) &&
      ((dimensions == coordT::width) ||
       (dimensions == 3 && coordT::width == 4));
};

/**
 * @brief Specialization of is_coords because for 1 dimension we have a scalar
 *        instead of a vec
 * @tparam coordT The type which is checked for being a valid coordinates type
 */
template <typename coordT>
struct is_coords<1, coordT> {
  /*! The value result of the type trait @ref is_coords */
  static constexpr auto value =
      (detail::is_same_basic_type<coordT, cl::sycl::cl_int>::value ||
       detail::is_same_basic_type<coordT, cl::sycl::cl_float>::value);
};

/*!
@brief Trait which provides the maximum of two integer values.
@tparam lhs First param.
@tparam rhs Second param.
*/
template <int lhs, int rhs>
struct max_dimensions {
  static constexpr int value = lhs < rhs ? rhs : lhs;
};

}  // namespace detail

/*******************************************************************************
    accessor utilities
*******************************************************************************/

namespace detail {

/*!
 * @brief Utility function which constructs coordinates for an image array slice
 * by combining the image coordinates and the image array index. Overload for
 * constructing 2-element coordinates.
 * @tparam elementT The element type of the coordinates.
 * @param coords The image coordinates.
 * @param arrayIndex The image array index.
 * @return The coordinates for the image array slice.
 */
template <typename elementT>
cl::sycl::vec<elementT, 2> make_image_array_slice_coords(elementT coords,
                                                         size_t arrayIndex) {
  return cl::sycl::vec<elementT, 2>(coords, static_cast<elementT>(arrayIndex));
}

/*!
 * @brief Utility function which constructs coordinates for an image array slice
 * by combining the image coordinates and the image array index. Overload for
 * constructing 4-element coordinates.
 * @tparam elementT The element type of the coordinates.
 * @param coords The image coordinates.
 * @param arrayIndex The image array index.
 * @return The coordinates for the image array slice.
 */
template <typename elementT>
cl::sycl::vec<elementT, 4> make_image_array_slice_coords(
    cl::sycl::vec<elementT, 2> coords, size_t arrayIndex) {
  return cl::sycl::vec<elementT, 4>(coords, static_cast<elementT>(arrayIndex),
                                    static_cast<elementT>(0));
}

/**
 * @brief Retrieves the access range based on the number of accessor dimensions
 *
 * For 0-dim accessors, the access range is a single point.
 * For other dimensions, use the provided range
 *
 * @tparam kDims Number of accessor dimensions
 * @tparam interface_dims Number of range dimensions
 * @param providedRange Range to use for non-zero-dim accessors
 * @return Access range for the accessor
 */
template <int kDims, int interface_dims>
detail::access_range get_access_range(
    const cl::sycl::range<interface_dims>& providedRange) {
  return {detail::index_array{0, 0, 0},
          (kDims == 0) ? range<interface_dims>{detail::index_array{1, 1, 1}}
                       : providedRange};
}

}  // namespace detail

/*******************************************************************************
    forward declarations
*******************************************************************************/

template <typename elemT, int kDims, access::mode kMode, access::target kTarget,
          access::placeholder isPlaceholder>
class accessor;

/*******************************************************************************
    image_array_slice
*******************************************************************************/

namespace detail {

/*!
 * @brief Intermediate class template which contains a reference to an image
 * array accessors and an image array index. Used to provide acc[i].read(..)
 * syntax.
 * @tparam elemT The element type of the image array accessor.
 * @tparam kDims The dimensionality of the image array accessor.
 * @tparam kMode The access mode of the image array accessor.
 */
template <typename elemT, int kDims, access::mode kMode>
class image_array_slice {
  /* Friend declaration to the associated specialisation of the accessor class.
   */
  friend class cl::sycl::accessor<elemT, kDims, kMode,
                                  access::target::image_array,
                                  access::placeholder::false_t>;

  static constexpr bool is_cl_float4 =
      (std::is_same<elemT, cl::sycl::cl_float4>::value);
  static constexpr bool is_cl_half4 =
      (std::is_same<elemT, cl::sycl::cl_half4>::value);
  static constexpr bool is_cl_int4 =
      (std::is_same<elemT, cl::sycl::cl_int4>::value);
  static constexpr bool is_cl_uint4 =
      (std::is_same<elemT, cl::sycl::cl_uint4>::value);

 public:
  /*!
  @brief Member function for reading an element of a read image array accessor
  with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  read from.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_float4 read(const coordT& coords) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readf(imageArraySliceCoords);
#else
    return detail::read_imagef(m_imageArrayAccRef.get_device_ptr(),
                               imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for reading an element of a read image array accessor
  with dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  read from.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_half4 read(const coordT& coords) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readh(imageArraySliceCoords);
#else
    return detail::read_imageh(m_imageArrayAccRef.get_device_ptr(),
                               imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for reading an element of a read image array accessor
  with dataT cl_int4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  read from.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(
                elemT, (is_cl_int4 && detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_int4 read(const coordT& coords) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readi(imageArraySliceCoords);
#else
    return detail::read_imagei(m_imageArrayAccRef.get_device_ptr(),
                               imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for reading an element of a read image array accessor
  with dataT cl_uint4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  read from.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_uint4 read(const coordT& coords) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readui(imageArraySliceCoords);
#else
    return detail::read_imageui(m_imageArrayAccRef.get_device_ptr(),
                                imageArraySliceCoords);

#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for sampling a point in a read image array accessor
  with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image array
  to sample from.
  @param smpl The sampler to use when sampling from the image array.
  @return An element calculated from sampling the image array using the
  coordinates specified and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_float4 read(const coordT& coords,
                           const cl::sycl::sampler& smpl) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readf(imageArraySliceCoords, smpl);
#else
    return detail::read_imagef(m_imageArrayAccRef.get_device_ptr(),
                               smpl.m_sampler, imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for sampling a point in a read image array accessor
  with dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image array
  to sample from.
  @param smpl The sampler to use when sampling from the image array.
  @return An element calculated from sampling the image array using the
  coordinates specified and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_half4 read(const coordT& coords,
                          const cl::sycl::sampler& smpl) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readh(imageArraySliceCoords, smpl);
#else
    return detail::read_imageh(m_imageArrayAccRef.get_device_ptr(),
                               smpl.m_sampler, imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for sampling a point in a read image array accessor
  with dataT cl_int4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image array
  to sample from.
  @param smpl The sampler to use when sampling from the image array.
  @return An element calculated from sampling the image array using the
  coordinates specified and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(
                elemT, (is_cl_int4 && detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_int4 read(const coordT& coords,
                         const cl::sycl::sampler& smpl) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readi(imageArraySliceCoords, smpl);
#else
    return detail::read_imagei(m_imageArrayAccRef.get_device_ptr(),
                               smpl.m_sampler, imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for sampling a point in a read image array accessor
  with dataT cl_uint4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image array
  to sample from.
  @param smpl The sampler to use when sampling from the image array.
  @return An element calculated from sampling the image array using the
  coordinates specified and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_uint4 read(const coordT& coords,
                          const cl::sycl::sampler& smpl) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    return m_imageArrayAccRef.readui(imageArraySliceCoords, smpl);
#else
    return detail::read_imageui(m_imageArrayAccRef.get_device_ptr(),
                                smpl.m_sampler, imageArraySliceCoords);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for writing to an element of a write image array
  accessor with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  write to.
  @param color The value that is to be assigned to the element of the image
  array.
  @return An element read from the image using the coordinates specified and the
  image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_float4& color) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    m_imageArrayAccRef.writef(imageArraySliceCoords, color);
#else
    detail::write_imagef(m_imageArrayAccRef.get_device_ptr(),
                         imageArraySliceCoords, color);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for writing to an element of a write image array
  accessor with dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  write to.
  @param color The value that is to be assigned to the element of the image
  array.
  @return An element read from the image array using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_half4& color) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    m_imageArrayAccRef.writeh(imageArraySliceCoords, color);
#else
    detail::write_imageh(m_imageArrayAccRef.get_device_ptr(),
                         imageArraySliceCoords, color);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for writing to an element of a write image array
  accessor with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  write to.
  @param color The value that is to be assigned to the element of the image
  array.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(
                elemT, (is_cl_int4 && detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_int4& color) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    m_imageArrayAccRef.writei(imageArraySliceCoords, color);
#else
    detail::write_imagei(m_imageArrayAccRef.get_device_ptr(),
                         imageArraySliceCoords, color);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
  @brief Member function for writing to an element of a write image array
  accessor with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image array to
  write to.
  @param color The value that is to be assigned to the element of the image
  array.
  @return An element read from the image array using the coordinates specified
  and the image array index.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_uint4& color) const {
    auto imageArraySliceCoords =
        make_image_array_slice_coords(coords, m_arrayIndex);
#ifndef __SYCL_DEVICE_ONLY__
    m_imageArrayAccRef.writeui(imageArraySliceCoords, color);
#else
    detail::write_imageui(m_imageArrayAccRef.get_device_ptr(),
                          imageArraySliceCoords, color);
#endif  // __SYCL_DEVICE_ONLY__
  }

  /*!
   * @brief Constructor which takes a reference to an image array accessor and
   * image array index and initialises the respective members.
   * @param imageArrayAccRef The reference to an image array accessor.
   * @param arrayIndex The image array index.
   */
  image_array_slice(
      const accessor_common<
          elemT, kDims, kMode, cl::sycl::access::target::image_array,
          cl::sycl::access::placeholder::false_t>& imageArrayAccRef,
      size_t arrayIndex)
      : m_imageArrayAccRef(imageArrayAccRef), m_arrayIndex(arrayIndex) {
    m_arrayIndex = arrayIndex;
  }

  const accessor_common<elemT, kDims, kMode, access::target::image_array,
                        access::placeholder::false_t>& m_imageArrayAccRef;
  size_t m_arrayIndex;
};

}  // namespace detail

/*******************************************************************************
    accessor_common
*******************************************************************************/

/*! COMPUTECPP_DEV @endcond*/

namespace detail {

// The inheritance changes slightly when on host or device.
// The device accessor_base is templated, but the host one is not.
// We declare this macro here so that the class declaration remains readable.
#ifndef __SYCL_DEVICE_ONLY__
#define COMPUTECPP_ACCESSOR_BASE_CLASS(elemT, kDims, kMode, kTarget, \
                                       isPlaceholder)                \
  accessor_base
#else  // !__SYCL_DEVICE_ONLY__
#define COMPUTECPP_ACCESSOR_BASE_CLASS(elemT, kDims, kMode, kTarget, \
                                       isPlaceholder)                \
  accessor_device_base<                                              \
      device_arg_container<elemT, kDims, kMode, kTarget, isPlaceholder>>
#endif  // __SYCL_DEVICE_ONLY__

/*!
@brief Class that implements common functions of the templated accessor class
@tparam elemT Specifies the element type of the pointer and typedef.
@tparam kDims Specifies the dimensions.
@tparam kMode Specifies the access mode.
@tparam kTarget Specifies the access target.
*/
template <typename elemT, int kDims, access::mode kMode, access::target kTarget,
          access::placeholder isPlaceholder>
class COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT accessor_common
    : public COMPUTECPP_ACCESSOR_BASE_CLASS(elemT, kDims, kMode, kTarget,
                                            isPlaceholder) {
  static constexpr int interface_dims = max_dimensions<kDims, 1>::value;
  static constexpr int range_dims =
      (kTarget == cl::sycl::access::target::image_array) ? (kDims + 1)
                                                         : interface_dims;

  using base_t = COMPUTECPP_ACCESSOR_BASE_CLASS(elemT, kDims, kMode, kTarget,
                                                isPlaceholder);
#undef COMPUTECPP_ACCESSOR_BASE_CLASS
 protected:
  /*!
  Predefined static const declarations of the const boolean expression used
  as enable_if conditions.
  */
  static constexpr bool is_image_ctr = (kTarget == access::target::image);
  static constexpr bool is_image_array_ctr =
      (kTarget == access::target::image_array);
  static constexpr bool is_host_image_ctr =
      (kTarget == access::target::host_image);
  static constexpr bool is_atomic_ctr = (kMode == access::mode::atomic);
  static constexpr bool is_buffer_syntax =
      ((kTarget == access::target::global_buffer ||
        kTarget == access::target::constant_buffer ||
        kTarget == access::target::host_buffer ||
        kTarget == access::target::local ||
        kTarget == access::target::subgroup_local));
  static constexpr bool is_read_only = (kMode == access::mode::read);
  static constexpr bool is_image_read_syntax =
      ((kTarget == access::target::image ||
        kTarget == access::target::host_image) &&
       (kMode == access::mode::read));
  static constexpr bool is_image_write_syntax =
      ((kTarget == access::target::image ||
        kTarget == access::target::host_image) &&
       (kMode == access::mode::write || kMode == access::mode::discard_write));
  static constexpr bool is_cl_float4 =
      (std::is_same<elemT, cl::sycl::cl_float4>::value);
  static constexpr bool is_cl_half4 =
      (std::is_same<elemT, cl::sycl::cl_half4>::value);
  static constexpr bool is_cl_int4 =
      (std::is_same<elemT, cl::sycl::cl_int4>::value);
  static constexpr bool is_cl_uint4 =
      (std::is_same<elemT, cl::sycl::cl_uint4>::value);
  static constexpr bool is_n_dim = kDims > 0 && kDims < 4;

  /**
   * @brief The atomic address space is only valid when combined with an atomic
   *        accessor, but it needs to be defined for all other targets as well.
   */
  static constexpr auto atomic_address_space =
      detail::get_atomic_address_space<kTarget>::value;

  using return_t = typename std::conditional<
      is_read_only, elemT,
      typename device_arg<elemT, kDims, kMode, kTarget,
                          isPlaceholder>::raw_ref_type>::type;

#ifdef __SYCL_DEVICE_ONLY__
  using base_t::m_deviceArgs;
#endif

 public:
  /*!
   @brief Alias for the value type of the accessor.
   */
  using value_type = elemT;
  /*!
   @brief Alias for the reference type of the accessor.
   */
  using reference = elemT&;
  /*!
   @brief Alias for the const reference type of the accessor.
   */
  using const_reference = const elemT&;
  /*!
  @brief Alias for the type returned by single subscript operators
  */
  using single_subscript_op_t =
      subscript_op<0, elemT, kDims, kMode, kTarget, isPlaceholder>;

  /**
    @brief Forwarding constructors to accessor_base
  */
  accessor_common(cl::sycl::storage_mem& store, handler& commandHandler)
      : base_t(store, kMode, kTarget, sizeof(elemT), commandHandler) {}
  accessor_common(cl::sycl::storage_mem& store, handler& commandHandler,
                  detail::access_range accessRange)
      : base_t(store, kMode, kTarget, sizeof(elemT), commandHandler,
               accessRange) {}
  accessor_common(dim_t numDims, const detail::index_array& numElements,
                  handler& commandHandler)
      : base_t(numDims, numElements, kMode, kTarget, sizeof(elemT),
               commandHandler) {}
  accessor_common(cl::sycl::storage_mem& store)
      : base_t(store, kMode, kTarget, sizeof(elemT)) {}
  accessor_common(cl::sycl::storage_mem& store,
                  detail::access_range accessRange)
      : base_t(store, kMode, kTarget, sizeof(elemT), accessRange) {}
  accessor_common() : base_t(kMode, kTarget) {}

#ifdef __SYCL_DEVICE_ONLY__
  /*Dummy declarations used by device compiler*/
  accessor_common(const accessor_common& rhs) = default;
  accessor_common(accessor_common&& rhs) = default;
  accessor_common& operator=(const accessor_common& rhs) = default;
  accessor_common& operator=(accessor_common&& rhs) = default;
#else   //__SYCL_DEVICE_ONLY__

  /*!
   @brief Copy constructor that delegates construction to accessor_base.
   @param rhs the accessor to be copied.
   */
  accessor_common(const accessor_common& rhs) : accessor_base{rhs} {}

  /*!
   @brief Move constructor that delegates construction to accessor_base.
   @param rhs the accessor to be moved.
   */
  accessor_common(accessor_common&& rhs) : accessor_base{std::move(rhs)} {}
  /*!
  @brief Copy Assignment operator that delegates the assignment to the
  accessor_base.
  @param rhs the accessor to be copied.
  */
  accessor_common& operator=(const accessor_common& rhs) {
    static_cast<accessor_base*>(this)->operator=(
        static_cast<const accessor_base&>(rhs));
    return *this;
  }
  /*!
  @brief Move Assignment operator that delegates the assignment to the
  accessor_base.
  @param rhs the accessor to be moved.
  */
  accessor_common& operator=(accessor_common&& rhs) {
    static_cast<accessor_base*>(this)->operator=(
        static_cast<accessor_base&&>(std::move(rhs)));
    return *this;
  }
#endif  // __SYCL_DEVICE_ONLY__

  /**
   * @brief Determines if lhs and rhs are equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if same underlying object
   */
  friend inline bool operator==(const accessor_common& lhs,
                                const accessor_common& rhs) {
    return ((lhs.get_impl() == rhs.get_impl()) &&
            (lhs.get_device_ptr() == rhs.get_device_ptr()));
  }

  /**
   * @brief Determines if lhs and rhs are not equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if different underlying objects
   */
  friend inline bool operator!=(const accessor_common& lhs,
                                const accessor_common& rhs) {
    return !(lhs == rhs);
  }

  /*!
  @brief Multiple-subscript syntax subscript operator for n-dimensional accessor
  with access target access::target::global_buffer,
  access::target::constant_buffer, access::target::host_buffer or
  access::target::local. Returns an access_ref object of dimensions 1 less than
  the dimensions of the accessor.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param index The first index to specify which element to access.
  @return An subscript_op object of dimensions 1 less than the accessor.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_buffer_syntax && !is_atomic_ctr &&
                                            kDims > 1)>
  subscript_op<(kDims - 1), elemT, kDims, kMode, kTarget, isPlaceholder>
  operator[](size_t index) const {
    detail::index_array idx(index, 0, 0);
    return subscript_op<(kDims - 1), elemT, kDims, kMode, kTarget,
                        isPlaceholder>(*this, idx);
  }

 private:
  /**
   * @brief Checks whether data is being accessed out-of-bounds
   * @param index Index being accessed
   * @return size_t Actual index.
   *         If the input index is valid, just return that,
   *         otherwise return 0.
   */
  inline size_t check_bounds(size_t index) const {
#ifdef COMPUTECPP_CHECK_OUT_OF_BOUNDS
    if (index >= this->get_count()) {
#ifdef COMPUTECPP_CHECK_VERBOSE
      printf("Out of bounds access with index %zu\n", index);
#endif  // COMPUTECPP_CHECK_VERBOSE
      auto ptr = reinterpret_cast<int*>(this->get_device_ptr());
      ptr[detail::error_code_begin(this->get_size())] = 0x5ca1ab1e;
      // Return a valid index instead
      return 0;
    }
#endif  // COMPUTECPP_CHECK_OUT_OF_BOUNDS
    return index;
  }

 public:
  /*!
  @brief Multiple-subscript syntax subscript operator for 1-dimensional accessor
  with access target access::target::global_buffer,
  access::target::constant_buffer, access::target::host_buffer or
  access::target::local. Returns a reference of type deduced by the device_arg
  struct to the element specified by the index parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param index The index to specify which element to access.
  @return A reference to the element specified by the index parameter.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, (is_buffer_syntax && kDims == 1 &&
                                         !is_atomic_ctr))>
  return_t operator[](size_t index) const {
    index = this->check_bounds(index);
    return this->get_device_ptr()[index];
  }

  /*!
  @brief Conversion operator for 0-dimensional accessor with access target
  access::target::global_buffer, or access::target::local. Returns a reference
  of type deduced by the device_arg struct to the element specified by the index
  parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @return A reference to the element specified by the index parameter.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, (is_buffer_syntax && kDims == 0 &&
                                         !is_atomic_ctr))>
  operator return_t() const {
    return *get_device_ptr();
  }

  /*!
  @brief Multiple-subscript syntax subscript operator for 1-dimensional accessor
  with access target access::target::global_buffer, or
  access::target::local. Returns a reference of type deduced by the device_arg
  struct to the element specified by the index parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param index The index to specify which element to accesss.
  @return A reference to the element specified by the index parameter.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, (is_buffer_syntax && kDims == 1 &&
                                         is_atomic_ctr))>
  atomic<elemT, atomic_address_space> operator[](size_t index) const {
    index = this->check_bounds(index);
    return cl::sycl::atomic<elemT, atomic_address_space>::make_from_device_ptr(
        this->get_device_ptr() + index);
  }

  /**
   * @brief Implicit conversion from a zero dimensional atomic accessor to
   *        an atomic type
   * @tparam COMPUTECPP_ENABLE_IF Only enabled for atomic 0-dim buffer accessors
   * @return Atomic object encapsulating the accessed data
   */
  template <COMPUTECPP_ENABLE_IF(elemT, (is_buffer_syntax && (kDims == 0) &&
                                         is_atomic_ctr))>
  operator atomic<elemT>() const {
    return atomic<elemT>::make_from_device_ptr(this->get_device_ptr());
  }

  /*!
  @brief Id subscript operator for accessor with access target
  access::target::global_buffer, access::target::constant_buffer,
  access::target::host_buffer or access::target::local. Returns a reference of
  type deduced by the device_arg struct to the element specified by the index
  parameter. This function also has a work around for the Android issue using
  the COMPUTECPP_USE_SEPARATE_KERNEL_ARGUMENTS macro, where individual fields
  are used for
  the range, rather than accessor range.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param index The index to specify which element to access.
  @return A reference to the element specified by the index parameter.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_buffer_syntax && !is_atomic_ctr &&
                                            is_n_dim)>
  return_t operator[](id<kDims> index) const {
#ifndef __SYCL_DEVICE_ONLY__
    range<interface_dims> rng = get_store_range();
    size_t idx = detail::construct_linear_row_major_index<kDims>(
        index[0], index[1], index[2], rng[0], rng[1], rng[2]);
#else
#if defined(COMPUTECPP_USE_SEPARATE_KERNEL_ARGUMENTS)
    size_t idx = detail::construct_linear_row_major_index<kDims>(
        index[0], index[1], index[2], m_deviceArgs.m_fullRange0,
        m_deviceArgs.m_fullRange1, m_deviceArgs.m_fullRange2);
#else
    size_t idx = detail::construct_linear_row_major_index<kDims>(
        index[0], index[1], index[2], m_deviceArgs.m_fullRange[0],
        m_deviceArgs.m_fullRange[1], m_deviceArgs.m_fullRange[2]);
#endif  // COMPUTECPP_USE_SEPARATE_KERNEL_ARGUMENTS
#endif  // __SYCL_DEVICE_ONLY__
    idx = this->check_bounds(idx);
    return this->get_device_ptr()[idx];
  }

  /*!
  @brief Id subscript operator for accessor with access target
  access::target::global_buffer or access::target::local. Returns a reference of
  type deduced by the
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param index The index to specify which element to access.
  @return A reference to the element specified by the index parameter.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_atomic_ctr&& is_n_dim)>
  atomic<elemT, atomic_address_space> operator[](id<kDims> index) const {
#ifndef __SYCL_DEVICE_ONLY__
    range<interface_dims> rng = get_store_range();
    size_t idx = detail::construct_linear_row_major_index<kDims>(
        index[0], index[1], index[2], rng[0], rng[1], rng[2]);
#else
    size_t idx = detail::construct_linear_row_major_index<kDims>(
        index[0], index[1], index[2], m_deviceArgs.m_fullRange[0],
        m_deviceArgs.m_fullRange[1], m_deviceArgs.m_fullRange[2]);
#endif  // __SYCL_DEVICE_ONLY__
    idx = this->check_bounds(idx);
    return atomic<elemT, atomic_address_space>::make_from_device_ptr(
        (this->get_pointer() + idx));
  }

  /*!
   * @brief Subscript operator for image array accessors which takes an image
   * array index and returns an intermediate obejct which represents the image
   * array slice. Used to provide acc[i].read(..) syntax.
   * @tparam COMPUTECPP_ENABLE_IF condition.
   * @param index The image array index.
   * @return The image array slice intermediate obejct.
   */
  template <COMPUTECPP_ENABLE_IF(elemT, is_image_array_ctr&& kDims < 3)>
  cl::sycl::detail::image_array_slice<elemT, kDims, kMode> operator[](
      size_t index) const {
    cl::sycl::detail::image_array_slice<elemT, kDims, kMode> imageArraySlice(
        *this, index);
    return imageArraySlice;
  }

  /*!
  @brief Member function for reading an element of a read image accessor with
  dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to read
  from.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_float4 read(const coordT& coords) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readf(coords);
#else
    return cl::sycl::cl_float4(
        detail::read_imagef(this->get_device_ptr(), coords));
#endif
  }

  /*!
  @brief Member function for reading an element of a read image accessor with
  dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to read
  from.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_half4 read(const coordT& coords) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readh(coords);
#else
    return cl::sycl::cl_half4(
        detail::read_imageh(this->get_device_ptr(), coords));
#endif
  }

  /*!
  @brief Member function for reading an element of a read image accessor with
  dataT cl_int4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to read
  from.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_int4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_int4 read(const coordT& coords) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readi(coords);
#else
    return cl::sycl::cl_int4(
               detail::read_imagei(this->get_device_ptr(), coords))
        .convert<cl::sycl::cl_int, rounding_mode::automatic>();
#endif
  }

  /*!
  @brief Member function for reading an element of a read image accessor with
  dataT cl_uint4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to read
  from.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_uint4 read(const coordT& coords) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readui(coords);
#else
    return cl::sycl::cl_uint4(
        detail::read_imageui(this->get_device_ptr(), coords));
#endif
  }

  /*!
  @brief Member function for sampling a point in a read image accessor with
  dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image to
  sample from.
  @param smpl The sampler to use when sampling from the image.
  @return An element calculated from sampling the image using the coordinates
  specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_float4 read(const coordT& coords,
                           const cl::sycl::sampler& smpl) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readf(coords, smpl);
#else
    return cl::sycl::cl_float4(
        detail::read_imagef(this->get_device_ptr(), smpl.m_sampler, coords));
#endif
  }

  /*!
  @brief Member function for sampling a point in a read image accessor with
  dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image to
  sample from.
  @param smpl The sampler to use when sampling from the image.
  @return An element calculated from sampling the image using the coordinates
  specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_half4 read(const coordT& coords,
                          const cl::sycl::sampler& smpl) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readh(coords, smpl);
#else
    return cl::sycl::cl_half4(
        detail::read_imageh(this->get_device_ptr(), smpl.m_sampler, coords));
#endif
  }

  /*!
  @brief Member function for sampling a point in a read image accessor with
  dataT cl_int4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image to
  sample from.
  @param smpl The sampler to use when sampling from the image.
  @return An element calculated from sampling the image using the coordinates
  specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_int4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_int4 read(const coordT& coords,
                         const cl::sycl::sampler& smpl) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readi(coords, smpl);
#else
    return cl::sycl::cl_int4(detail::read_imagei(this->get_device_ptr(),
                                                 smpl.m_sampler, coords))
        .convert<cl::sycl::cl_int, rounding_mode::automatic>();
#endif
  }

  /*!
  @brief Member function for sampling a point in a read image accessor with
  dataT cl_uint4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates which specify the point within the image to
  sample from.
  @param smpl The sampler to use when sampling from the image.
  @return An element calculated from sampling the image using the coordinates
  specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_read_syntax && is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  cl::sycl::cl_uint4 read(const coordT& coords,
                          const cl::sycl::sampler& smpl) const {
#ifndef __SYCL_DEVICE_ONLY__
    return readui(coords, smpl);
#else
    return cl::sycl::cl_uint4(
        detail::read_imageui(this->get_device_ptr(), smpl.m_sampler, coords));
#endif
  }

  /*!
  @brief Member function for writing to an element of a write image accessor
  with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to write
  to.
  @param color The value that is to be assigned to the element of the image.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_write_syntax && is_cl_float4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_float4& color) const {
#ifndef __SYCL_DEVICE_ONLY__
    writef(coords, color);
#else
    detail::write_imagef(this->get_device_ptr(), coords, color);
#endif
  }

  /*!
  @brief Member function for writing to an element of a write image accessor
  with dataT cl_half4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to write
  to.
  @param color The value that is to be assigned to the element of the image.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_write_syntax && is_cl_half4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_half4& color) const {
#ifndef __SYCL_DEVICE_ONLY__
    writeh(coords, color);
#else
    detail::write_imageh(this->get_device_ptr(), coords, color);
#endif
  }

  /*!
  @brief Member function for writing to an element of a write image accessor
  with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to write
  to.
  @param color The value that is to be assigned to the element of the image.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_write_syntax && is_cl_int4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_int4& color) const {
#ifndef __SYCL_DEVICE_ONLY__
    writei(coords, color);
#else
    detail::write_imagei(this->get_device_ptr(), coords, color);
#endif
  }

  /*!
  @brief Member function for writing to an element of a write image accessor
  with dataT cl_float4.
  @tparam coordT The type of the coordinates parameter.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param coords The coordinates to specify which element of the image to write
  to.
  @param color The value that is to be assigned to the element of the image.
  @return An element read from the image using the coordinates specified.
  */
  template <typename coordT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (is_image_write_syntax && is_cl_uint4 &&
                                  detail::is_coords<kDims, coordT>::value))>
  void write(const coordT& coords, const cl::sycl::cl_uint4& color) const {
#ifndef __SYCL_DEVICE_ONLY__
    writeui(coords, color);
#else
    detail::write_imageui(this->get_device_ptr(), coords, color);
#endif
  }

  /**
    @brief Indicates whether the accessor is a placeholder accessor
    @return True if accessor is a placeholder
  */
  inline bool is_placeholder() const {
    return (isPlaceholder == access::placeholder::true_t);
  }

  /*!
  @brief Method that returns the device argument, which can be either a pointer
  with an address space or an OpenCL image type, this is deduced by the
  device_arg struct. This method is not enabled for access targets
  access::target::image or access::target::host_image.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @return The device argument deduced by the device_arg struct.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, (!is_image_ctr))>
  typename device_arg<elemT, kDims, kMode, kTarget,
                      isPlaceholder>::ptr_class_type
  get_pointer() const {
    return typename device_arg<elemT, kDims, kMode, kTarget, isPlaceholder>::
        ptr_class_type(this->get_device_ptr());
  }

  /*!
  @brief Method that returns the device argument, which can be either a pointer
  with an address space or an OpenCL image type, this is deduced by the
  device_arg struct. For the host side this returns the raw host pointer that is
  in accessor_base, for the device side this returns the device pointer that is
  in the device_arg_container.
  @return The device argument deduced by the device_arg struct.
  */
  typename device_arg<elemT, kDims, kMode, kTarget, isPlaceholder>::raw_ptr_type
  get_device_ptr() const {
#if defined(__SYCL_DEVICE_ONLY__)
    return m_deviceArgs.m_deviceArg.get_ptr();
#else
    using ptrType = typename device_arg<elemT, kDims, kMode, kTarget,
                                        isPlaceholder>::raw_ptr_type;
    return static_cast<ptrType>(base_t::get_host_data());
#endif
  }

  /*!
   *@brief Returns the plane id.
   *@return 0 for the host, the corresponding
   *planeId for the device
   */
#if defined(__SYCL_DEVICE_ONLY__)
  std::int8_t get_device_plane_id() const { return m_deviceArgs.m_planeId; }
#else
  std::int8_t get_device_plane_id() const { return 0; }
#endif

  /*!
  @brief Method for converting an id<N> to a the appropriate coordinates type
  for reading and writing images. This overload is for id<1> and returns an int.
  @param index The id<1> to be converted to an int.
  @return the converted int value.
  */
  inline int convert_coords(id<1> index) const { return int(index[0]); }

  /*!
  @brief Method for converting an id<N> to a the appropriate coordinates type
  for reading and writing images. This overload is for id<2> and returns an
  int2.
  @param index The id<2> to be converted to an int2.
  @return the converted int2 value.
  */
  inline int2 convert_coords(id<2> index) const {
    return int2(index[0], index[1]);
  }

  /*!
  @brief Method for converting an id<N> to a the appropriate coordinates type
  for reading and writing images. This overload is for id<3> and returns an
  int4. As OpenCL uses int4 coordinates for accessing a 3 dimensional image, we
  convert to an int4 here.
  @param index The id<3> to be converted to an int4.
  @return the converted int4 value.
  */
  inline int4 convert_coords(id<3> index) const {
    return int4(index[0], index[1], index[2], 0);
  }

  /*!
  @brief Gets the number of elements the accessor can access.
  @return The number of elements the accessor can access.
  */
  size_t get_count() const {
#ifndef __SYCL_DEVICE_ONLY__
    return accessor_base::get_count();
#else
    return (m_deviceArgs.m_range[0] * m_deviceArgs.m_range[1] *
            m_deviceArgs.m_range[2]);
#endif
  }

  /*!
  @brief Gets the number of elements the accessor can access.
  @return The number of elements the accessor can access.
  */
  size_t get_size() const {
#ifndef __SYCL_DEVICE_ONLY__
    return accessor_base::get_size();
#else
    return (sizeof(elemT) * (m_deviceArgs.m_range[0] * m_deviceArgs.m_range[1] *
                             m_deviceArgs.m_range[2]));
#endif
  }

  /*!
  @brief Gets the range of the memory the accessor can access.
  @return The the range of the memory the accessor can access.
  */
  range<range_dims> get_range() const {
#ifndef __SYCL_DEVICE_ONLY__
    return accessor_base::get_range();
#else
    range<range_dims> range;
    for (int i = 0; i < range_dims; ++i) {
      range[i] = m_deviceArgs.m_range[i];
    }
    return range;
#endif
  }

  /*!
  @brief Gets the offset of the memory the accessor can access.
  @return The the offset of the memory the accessor can access.
  */
  id<range_dims> get_offset() const {
#ifndef __SYCL_DEVICE_ONLY__
    return accessor_base::get_offset();
#else
    id<range_dims> offset;
    for (int i = 0; i < range_dims; ++i) {
      offset[i] = m_deviceArgs.m_offset[i];
    }
    return offset;
#endif
  }
} COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT;
}  // namespace detail

/*******************************************************************************
    accessor
*******************************************************************************/

/*!
@brief Public facing interface class for allowing users access to buffer
objects, image objects and local memory from within kernel functions and the
host. The accessor class has many different constructors and operators available
depending on the class template arguments including access target, access mode
and dimensions. These constructors and operators are implemented using an
enable_if technique in order to avoid a large amount of inheritance and code
duplication. In order to reduce the complexity the enable_if conditions are
predefined using static const const booleans and the enable_if definitions
themselves are defined using the COMPUTECPP_ENABLE_IF macro. The accessor class
also has
the COMPUTECPP_CONVERT_ATTR macro attached to the end of the struct
declaration,
this is used during the compilers parameter flattening mechanism. The accessor
class also has the COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT and
COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT
macros which align the accessor class to the pointer size, the reason for
this is that for environments where the host and device pointer sizes don't
match the kernel argument offsets can sometimes misalign, aligning the accessor
resolves this.
@tparam elemT Specifies the element type of the pointer and typedef.
@tparam kDims Specifies the dimensions.
@tparam kMode Specifies the access mode.
@tparam kTarget Specifies the access target.
@tparam isPlaceholder Specifies whether this is a placeholder accessor.
*/
template <typename elemT, int kDims, access::mode kMode, access::target kTarget,
          access::placeholder isPlaceholder>
class COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT COMPUTECPP_VALID_KERNEL_ARG_IF(
    kTarget != access::target::host_buffer,
    "Cannot pass host accessor to SYCL kernel") accessor
    : public detail::accessor_common<elemT, kDims, kMode, kTarget,
                                     access::placeholder::false_t> {
  using accessor_common = detail::accessor_common<elemT, kDims, kMode, kTarget,
                                                  access::placeholder::false_t>;

  static constexpr int interface_dims = detail::max_dimensions<kDims, 1>::value;

  static_assert((kTarget != access::target::local) ||
                    (kMode == access::mode::read_write ||
                     kMode == access::mode::atomic),
                "access::target::local is only compatible with "
                "access::mode::read_write.");
  static_assert((kTarget != access::target::subgroup_local) ||
                    (kMode == access::mode::read_write),
                "access::target::subgroup_local is only compatible with "
                "access::mode::read_write.");

 public:
  /*!
  Predefined static const declarations of the const boolean expression used
  as enable_if conditions.
  */
  static constexpr bool is_global_buffer_ctr =
      (kTarget == access::target::global_buffer);
  static constexpr bool is_const_buffer_ctr =
      (kTarget == access::target::constant_buffer);
  static constexpr bool is_host_buffer_ctr =
      (kTarget == access::target::host_buffer);
  static constexpr bool is_local_ctr =
      (kTarget == access::target::local) ||
      (kTarget == access::target::subgroup_local);
  static constexpr bool is_image_ctr = accessor_common::is_image_ctr;
  static constexpr bool is_image_array_ctr =
      accessor_common::is_image_array_ctr;
  static constexpr bool is_host_image_ctr = accessor_common::is_host_image_ctr;
  static constexpr bool is_atomic_ctr = accessor_common::is_atomic_ctr;
  static constexpr bool is_global_or_const_atom_ctr =
      (is_global_buffer_ctr || (is_const_buffer_ctr && !is_atomic_ctr));
  static constexpr bool is_non_local_ctr =
      (is_global_or_const_atom_ctr || is_image_ctr);
  static constexpr bool is_n_dim = kDims > 0 && kDims < 4;

  /*!
  @brief Constructs an accessor of access target access::target::global_buffer
  or access::target::constant_buffer by taking a buffer object and a handler and
  initialises the base_accessor with the buffer, the handler, the access target,
  the access mode and the element size.
  @tparam AllocatorT Specifies the type of the buffer objects allocator.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
  global, or
  is constant and not atomic.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_global_or_const_atom_ctr)>
  accessor(buffer<elemT, interface_dims, AllocatorT>& bufferRef,
           handler& commandHandler)
      : accessor_common(
            bufferRef, commandHandler,
            detail::get_access_range<kDims>(bufferRef.get_range())) {}

  /*!
  @brief Constructs an accessor of access target access::target::global_buffer
  or access::target::constant_buffer by taking a buffer object, a handler, an
  offset and a range and initialises the base_accessor with the buffer, the
  handler, the access target, the access mode and the element size. This
  constructor is for constructing a sub accessor.
  @tparam AllocatorT Specifies the type of the buffer objects allocator.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
  global, or
  is constant and not atomic.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  @param accessOffset The offset that the sub accessor should have access from.
  @param accessRange The range that the sub accessor should have access to.
  scope that the accessor is being constructed within.
  @deprecated Need to reverse the order of the access offset and range,
              see 4.7.6.6 Buffer accessor interface in SYCL 1.2.1
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_global_or_const_atom_ctr&& is_n_dim)>
  COMPUTECPP_DEPRECATED_API("Deprecated constructor since SYCL 1.2.1")
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, handler& commandHandler,
           id<kDims> accessOffset, range<kDims> accessRange)
      : accessor_common(bufferRef, commandHandler,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs an accessor of access target global_buffer or
   *        constant_buffer from a buffer object, a handler, a range
   *        and an offset.
   * @tparam AllocatorT Type of the buffer objects allocator
   * @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
   *         global, or is constant and not atomic, and is not zero dimensional
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from
   * @param commandHandler Reference to the handler object for the command group
   * @param accessRange The range that the accessor should have access to
   * @param accessOffset The offset that the accessor should have access from
   */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_global_or_const_atom_ctr&& is_n_dim)>
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, handler& commandHandler,
           range<kDims> accessRange, id<kDims> accessOffset = {})
      : accessor_common(bufferRef, commandHandler,
                        detail::access_range(accessOffset, accessRange)) {}

  /*!
  @brief Constructs an accessor of access target access::target::host_buffer by
  taking a buffer object and initialises the base_accessor with the buffer, the
  access target, the access mode and the element size.
  @tparam AllocatorT Specifies the type of the buffer objects allocator.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the condition
  is_host_buffer_ctr is met.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_host_buffer_ctr)>
  accessor(buffer<elemT, interface_dims, AllocatorT>& bufferRef)
      : accessor_common(
            bufferRef, detail::get_access_range<kDims>(bufferRef.get_range())) {
  }

  /*!
  @brief Constructs an accessor of access target access::target::host_buffer by
  taking a buffer object, an offset and a range and initialises the
  base_accessor with the buffer, the access target, the access mode and the
  element size. This constructor is for constructing a sub accessor.
  @tparam AllocatorT Specifies the type of the buffer objects allocator.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the condition
  is_host_buffer_ctr is met.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param accessOffset The offset that the sub accessor should have access from.
  @param accessRange The range that the sub accessor should have access to.
  @deprecated Need to reverse the order of the access offset and range,
              see 4.7.6.6 Buffer accessor interface in SYCL 1.2.1
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_host_buffer_ctr&& is_n_dim)>
  COMPUTECPP_DEPRECATED_API("Deprecated constructor since SYCL 1.2.1")
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, id<kDims> accessOffset,
           range<kDims> accessRange)
      : accessor_common(bufferRef,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs an accessor of access target host_buffer
   *        from a buffer object, an offset and a range
   * @tparam AllocatorT Type of the buffer object allocator
   * @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the condition
   *         is_host_buffer_ctr is met and this is not a zero dimensional
   *         accessor
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from
   * @param accessRange The range that the accessor should have access to
   * @param accessOffset The offset that the accessor should have access from
   */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_host_buffer_ctr&& is_n_dim)>
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef,
           range<kDims> accessRange, id<kDims> accessOffset = {})
      : accessor_common(bufferRef,
                        detail::access_range(accessOffset, accessRange)) {}

  /*!
  @brief Constructs an n-dimensional accessor of access target
  access::target::local by taking a range and a handler and initialises the
  base_accessor with the range, the handler, the access target, the access mode
  and the element size.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the conditions
  is_local_ctr and kDims > 0 is met.
  @param numElements The range that local memory should be allocated for.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_local_ctr&& is_n_dim)>
  accessor(range<interface_dims> numElements, handler& commandHandler)
      : accessor_common(kDims, numElements, commandHandler) {}

  /*!
  @brief Constructs a 0-dimensional accessor of access target
  access::target::local by taking a handler and initialises the base_accessor
  with a range of 1, the handler, the access target, the access mode and the
  element size.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the conditions
  is_local_ctr and kDims == 0 are met.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_local_ctr&& kDims == 0)>
  accessor(handler& commandHandler)
      : accessor_common(kDims, range<1>(1), commandHandler) {}

  /*!
  @brief Constructs an accessor of access target access::target::image by taking
  an image object and a handler and initialises the base_accessor with the
  image, the handler, the access target, the access mode and the element size.
  @tparam AllocatorT Specifies the type of the image objects allocator.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param imageRef Reference to the image object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_image_ctr&& is_n_dim)>
  accessor(image<kDims, AllocatorT>& imageRef, handler& commandHandler)
      : accessor_common(imageRef, commandHandler) {}

  /*!
  @brief Constructs an accessor of access target access::target::host_image by
  taking an image object and initialises the base_accessor with the image, the
  access target, the access mode and the element size.
  @tparam AllocatorT Specifies the type of the image objects allocator.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param imageRef Reference to the image object the accessor is to access
  data from.
  */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_host_image_ctr&& is_n_dim)>
  explicit accessor(image<kDims, AllocatorT>& imageRef)
      : accessor_common(imageRef) {}

  /*!
   * @brief Constructs an accessor of access target access::target::image_array
   * by taking an image object of dimensionality one greater than this accessor
   * and initialises the accessor_common with the image.
   * @tparam AllocatorT Specifies the allocator type.
   * @tparam COMPUTECPP_ENABLE_IF condition.
   * @param imageRef Reference to the image object being accessed.
   * @param commandHandler Reference to the handler of the command group the
   * accessor is being constructed within.
   */
  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT, is_image_array_ctr&& kDims < 3)>
  accessor(image<(kDims + 1), AllocatorT>& imageRef, handler& commandHandler)
      : accessor_common(imageRef, commandHandler) {}

 protected:
  friend class accessor<elemT, kDims, kMode, kTarget,
                        access::placeholder::true_t>;

  /*!
  @internal
  @brief Constructs an accessor from a \ref{storage_mem} object.
         Used to create normal accessors from placeholder ones.
  @tparam COMPUTECPP_ENABLE_IF condition.
  @param store Storage object representing the buffer/image
  @param commandHandler Handler object for the command group scope that the
         accessor is being constructed within.
  */
  template <COMPUTECPP_ENABLE_IF(elemT, is_non_local_ctr&& is_n_dim)>
  accessor(storage_mem&& store, handler& commandHandler,
           detail::access_range accessRange)
      : accessor_common(store, commandHandler, accessRange) {}

} COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT COMPUTECPP_CONVERT_ATTR;

/*******************************************************************************
    placeholder accessor
*******************************************************************************/
/*!
  @brief A public facing accessor that can be constructed outside of a command
         group.

         Even though it can be constructed, it cannot be accessed outside of a
         command group. Before it can be accessed, it has to be registered in a
         command group handler.

         This is an extension of the SYCL specification.

         The reason this is a specialization instead of using enable_if in one
         class is that COMPUTECPP_CONVERT_ATTR has to be placed on the regular
         accessor class and COMPUTECPP_CONVERT_ATTR_PLACEHOLDER on the
         placeholder one.
  @tparam elemT Specifies the element type of the pointer and typedef.
  @tparam kDims Specifies the dimensions.
  @tparam kMode Specifies the access mode.
  @tparam kTarget Specifies the access target.
*/
template <typename elemT, int kDims, access::mode kMode, access::target kTarget>
class COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT COMPUTECPP_VALID_KERNEL_ARG_IF(
    kTarget != access::target::host_buffer,
    "Cannot pass host accessor to SYCL kernel")
    accessor<elemT, kDims, kMode, kTarget, access::placeholder::true_t>
    : public detail::accessor_common<elemT, kDims, kMode, kTarget,
                                     access::placeholder::true_t> {
  using accessor_common = detail::accessor_common<elemT, kDims, kMode, kTarget,
                                                  access::placeholder::true_t>;

  static constexpr int interface_dims = detail::max_dimensions<kDims, 1>::value;

  static_assert((kTarget != access::target::local) ||
                    (kMode == access::mode::read_write),
                "access::target::local is only compatible with "
                "access::mode::read_write.");
  static_assert((kTarget != access::target::subgroup_local) ||
                    (kMode == access::mode::read_write),
                "access::target::subgroup_local is only compatible with "
                "access::mode::read_write.");

 protected:
  /**
   * @brief Constructs a ranged placeholder accessor
   * @tparam AllocatorT Specifies the type of the buffer objects allocator.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   * @param accessRange the offset and range that this accessor can access.
   */
  accessor(storage_mem& store, detail::access_range accessRange)
      : accessor_common(store, accessRange) {}
  /**
  @brief returns an accessor similar to *this, with the offset changed to
         this->get_offset()[0] + addedOffset. A call to this function will fail
         with ACCESSOR_ARGUMENTS_ERROR if the created accessor has an offset
         less than 0 or if the new offset plus the current access range exceed
         this->get_range()[0].
  */
  accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t>
  get_accessor_with_added_offset(int addedOffset) const {
    detail::access_range range = this->get_access_range();
    // ensure that negative values move the offset backwards
    intptr_t result = static_cast<intptr_t>(addedOffset) +
                      static_cast<intptr_t>(range.offset[0]);
    // check underflow
    if (result < 0) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          CL_SUCCESS, detail::cpp_error_code::ACCESSOR_ARGUMENTS_ERROR, nullptr,
          "Attempted arithmetic operation out of accessor bounds. Underflow");
    }
    // check overflow
    size_t resultSizeT = static_cast<size_t>(result);
    if (resultSizeT + range.range[0] > this->get_store_range()[0]) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          CL_SUCCESS, detail::cpp_error_code::ACCESSOR_ARGUMENTS_ERROR, nullptr,
          "Attempted arithmetic operation out of accessor bounds. Overflow");
    }
    // set the result
    range.offset[0] = resultSizeT;

    cl::sycl::storage_mem storage(this->get_store());
    return accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t>(
        storage, range);
  }

 public:
  /**
   * @brief Constructs a default placeholder accessor without associated
   * storage.
   */
  accessor() : accessor_common() {}

  /**
   * @brief Constructs a placeholder accessor
   * @tparam AllocatorT Specifies the type of the buffer objects allocator.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   */
  template <typename AllocatorT = default_allocator>
  explicit accessor(buffer<elemT, interface_dims, AllocatorT>& bufferRef)
      : accessor_common(
            bufferRef, detail::get_access_range<kDims>(bufferRef.get_range())) {
  }

  /**
   * @brief Constructs a ranged placeholder accessor
   * @tparam AllocatorT Specifies the type of the buffer objects allocator.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   * @param accessRange the offset and range that this accessor can access.
   * @deprecated Need to reverse the order of the access offset and range,
   *             see 4.7.6.6 Buffer accessor interface in SYCL 1.2.1
   */
  template <typename AllocatorT>
  COMPUTECPP_DEPRECATED_API("Deprecated constructor since SYCL 1.2.1")
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, id<kDims> accessOffset,
           range<kDims> accessRange)
      : accessor_common(bufferRef,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs a ranged placeholder accessor
   * @tparam AllocatorT Specifies the type of the buffer objects allocator.
   * @param accessRange the offset and range that this accessor can access.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   */
  template <typename AllocatorT>
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef,
           range<kDims> accessRange, id<kDims> accessOffset = {})
      : accessor_common(bufferRef,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs a ranged placeholder accessor
   * @param accessRange the offset and range that this accessor can access.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   */
  accessor(buffer<elemT, kDims>& bufferRef, range<kDims> accessRange,
           id<kDims> accessOffset = {})
      : accessor_common(bufferRef,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs a ranged placeholder accessor
   * @tparam AllocatorT Specifies the type of the buffer objects allocator.
   * @param commandHandler Reference to the handler object for the command group
   * scope that the accessor is being constructed within.
   * @param accessRange the offset and range that this accessor can access.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   */
  template <typename AllocatorT>
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, handler& commandHandler,
           range<kDims> accessRange, id<kDims> accessOffset = {})
      : accessor_common(bufferRef, commandHandler,
                        detail::access_range(accessOffset, accessRange)) {}

  /**
   * @brief Constructs a ranged placeholder accessor
   * @param commandHandler Reference to the handler object for the command group
   * scope that the accessor is being constructed within.
   * @param accessRange the offset and range that this accessor can access.
   * @param bufferRef Reference to the buffer object the accessor is to access
   *        data from.
   */
  accessor(buffer<elemT, kDims>& bufferRef, handler& commandHandler,
           range<kDims> accessRange, id<kDims> accessOffset = {})
      : accessor_common(bufferRef, commandHandler,
                        detail::access_range(accessOffset, accessRange)) {}

  /*!
  @brief Constructs a placeholder accessor of access target
  access::target::global_buffer or access::target::constant_buffer by taking a
  buffer object and a handler and initialises the base_accessor with the buffer,
  the handler, the access target, the access mode and the element size.
  @tparam AllocatorT Specifies the type of the buffer objects allocator.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
  global, or
  is constant and not atomic.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  template <typename AllocatorT>
  accessor(buffer<elemT, kDims, AllocatorT>& bufferRef, handler& commandHandler)
      : accessor_common(bufferRef, commandHandler) {}

  /*!
  @brief Constructs a placeholder accessor of access target
  access::target::global_buffer or access::target::constant_buffer by taking a
  buffer object and a handler and initialises the base_accessor with the buffer,
  the handler, the access target, the access mode and the element size.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
  global, or
  is constant and not atomic.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */
  accessor(buffer<elemT, kDims>& bufferRef, handler& commandHandler)
      : accessor_common(bufferRef, commandHandler) {}

  /*!
  @brief Constructs a placeholder 0 dimentional accessor of access target
  access::target::global_buffer or access::target::constant_buffer by taking a
  buffer object and a handler and initialises the base_accessor with the buffer,
  the handler, the access target, the access mode and the element size.
  @tparam COMPUTECPP_ENABLE_IF Only allows the constructor if the accessor is
  global, or
  is constant and not atomic.
  @param bufferRef Reference to the buffer object the accessor is to access
  data from.
  @param commandHandler Reference to the handler object for the command group
  scope that the accessor is being constructed within.
  */

  template <typename AllocatorT,
            COMPUTECPP_ENABLE_IF(elemT,
                                 (kTarget == access::target::global_buffer ||
                                  kTarget == access::target::constant_buffer) &&
                                     kDims == 0)>
  accessor(buffer<elemT, 1, AllocatorT>& bufferRef, handler& commandHandler)
      : accessor_common(bufferRef, commandHandler) {}

  /**
   * @brief Obtains a normal accessor from the placeholder accessor
   * @param commandHandler Command group handler where the accessor will be used
   * @return Normal accessor that does not need to be registered
   */
  accessor<elemT, kDims, kMode, kTarget, access::placeholder::false_t>
  get_access(handler& commandHandler) const {
    return accessor<elemT, kDims, kMode, kTarget, access::placeholder::false_t>(
        cl::sycl::storage_mem(this->get_store()), commandHandler,
        this->get_access_range());
  }

  /**
   * @brief Creates and returns a new accessor with its offset changed by rhs
   * @param rhs the offset to add to the current accessor's offset in the new
   * accessor
   * @return A new accessor with its get_offset()[0] equal to
   * this->get_offset()[0] + rhs
   */
  template <COMPUTECPP_ENABLE_IF(elemT, (kDims == 1))>
  accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t> operator+(
      int rhs) const {
    return get_accessor_with_added_offset(rhs);
  }

  /**
   * @brief Creates and returns a new accessor with its offset changed by rhs
   * @param rhs the offset to subtract to the current accessor's offset in the
   * new accessor
   * @return A new accessor with its get_offset()[0] equal to
   * this->get_offset()[0] - rhs
   */
  template <COMPUTECPP_ENABLE_IF(elemT, (kDims == 1))>
  accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t> operator-(
      int rhs) const {
    return get_accessor_with_added_offset(-rhs);
  }

  /**
   * @brief Chnages this->offset by rhs as this->get_offset()[0] + rhs
   * @param rhs the offset to add to the current accessor's offset
   * @return This accesor
   */
  template <COMPUTECPP_ENABLE_IF(elemT, (kDims == 1))>
  accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t>& operator+=(
      int rhs) {
    *this = get_accessor_with_added_offset(rhs);
    return *this;
  }

  /**
   * @brief Chnages this->offset by rhs as this->get_offset()[0] - rhs
   * @param rhs the offset to subtract to the current accessor's offset
   * @return This accesor
   */
  template <COMPUTECPP_ENABLE_IF(elemT, (kDims == 1))>
  accessor<elemT, 1, kMode, kTarget, access::placeholder::true_t>& operator-=(
      int rhs) {
    *this = get_accessor_with_added_offset(-rhs);
    return *this;
  }

} COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT COMPUTECPP_CONVERT_ATTR_PLACEHOLDER;

/*******************************************************************************
    undefine macros
*******************************************************************************/

#undef COMPUTECPP_ACCESSOR_WINDOWS_ALIGNMENT
#undef COMPUTECPP_ACCESSOR_LINUX_ALIGNMENT

/******************************************************************************/

}  // namespace sycl
}  // namespace cl

namespace std {
/*!
@brief provides a specialization for std::hash for the buffer class. An
std::hash<std::shared_ptr<...>> object is created and its function call
operator is used to hash the contents of the shared_ptr. The returned hash is
actually the result of (size_t) object.get_impl().get()
*/
template <typename elemT, int kDims, cl::sycl::access::mode kMode,
          cl::sycl::access::target kTarget,
          cl::sycl::access::placeholder isPlaceholder>
struct hash<cl::sycl::accessor<elemT, kDims, kMode, kTarget, isPlaceholder>> {
 public:
  /*!
  @brief enables calling an std::hash object as a function with the object to be
  hashed as a parameter
  @param object the object to be hashed
  @tparam std the std namespace where this specialization resides
  */
  size_t operator()(const cl::sycl::accessor<elemT, kDims, kMode, kTarget,
                                             isPlaceholder>& rhs) const {
    hash<cl::sycl::daccessor_shptr> hasher;
    return hasher(rhs.get_impl());
  }
};
}  // namespace std

/******************************************************************************/

#endif  // RUNTIME_INCLUDE_SYCL_ACCESSOR_H_

/******************************************************************************/
