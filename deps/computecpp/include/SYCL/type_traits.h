/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

*******************************************************************************/

/*!
  @file type_traits.h

  @brief Provides type traits from future C++ versions in C++11, as well as some
         in-house type traits
  @note This header is part of the implementation of the SYCL library and cannot
  be used independently.
*/
#ifndef RUNTIME_INCLUDE_SYCL_TYPE_TRAITS_H_
#define RUNTIME_INCLUDE_SYCL_TYPE_TRAITS_H_

#include "SYCL/vec.h"
#include <type_traits>

namespace cl {
namespace sycl {
namespace detail {
/**
 * @brief See http://en.cppreference.com/w/cpp/types/enable_if.
 */
template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/conditional.
 */
template <bool B, typename T1, typename T2>
using conditional_t = typename std::conditional<B, T1, T2>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/make_signed.
 */
template <typename T>
using make_signed_t = typename std::make_signed<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/make_unsigned.
 */
template <typename T>
using make_unsigned_t = typename std::make_unsigned<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/negation.
 */
template <typename T>
using negation = std::integral_constant<bool, !T::value>;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_cv.
 */
template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_cv.
 */
template <typename T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_cv.
 */
template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_reference.
 */
template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_cvref.
 */
template <typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/remove_pointer.
 */
template <typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/underlying_type.
 */
template <typename T>
using underlying_type_t = typename std::underlying_type<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/decay.
 */
template <typename T>
using decay_t = typename std::decay<T>::type;

/**
 * @brief See http://en.cppreference.com/w/cpp/types/void_t.
 */
template <typename...>
using void_t = void;

/////////// Codeplay-exclusive type_traits

/**
 * @brief Extracts an element_type typename from an arbitrary type.
 * @tparam T The type to extract `element_type` from.
 * @tparam void Used to detect if typename `T::element_type` is a valid type.
 * @note If `typename T::element_type` is not a valid type, then `element_type`
 *       does not have a member.
 */
template <typename T, typename = void>
struct get_element_type {};

/**
 * @brief Extracts an element_type typename from an arbitrary type.
 *        Specialization for when `typename T::element_type` is a valid type.
 * @tparam T The type to extract `element_type` from.
 */
template <typename T>
struct get_element_type<T, void_t<typename T::element_type>> {
  using type = typename T::element_type;
};

template <typename T>
using get_element_type_t = typename get_element_type<T>::type;

/**
 * @brief Unary type function with a return type determined by whether or not
 *        the input is a signed type or not.
 *
 * @tparam T1 input type
 * @tparam T2 returned type if T1 is a signed type
 * @tparam T3 returned type if T1 is not a signed type
 */
template <typename T1, typename T2, typename T3>
using deduce_signedness_t =
    typename std::conditional<std::is_signed<T1>::value, T2, T3>::type;

/**
 * @brief A library-based emulation of C++17 fold expressions
 */
template <bool...>
struct bool_pack {};

/**
 * @brief Checks to see if T1 is present in the parameter pack.
 * @return false if T1 is present in the paramter pack, true otherwise.
 * @tparam T1 The subject type.
 * @tparam Ts... A list of types to be compared against.
 */
// TODO(Chris): Removed to avoid compiler bug in MSVC.
template <typename T1, typename... Ts>
using is_none_of =
    std::is_same<bool_pack<false, std::is_same<T1, Ts>::value...>,
                 bool_pack<std::is_same<T1, Ts>::value..., false>>;

/**
 * @brief Checks to see if T1 is present in the parameter pack.
 * @tparam T1 The subject type.
 * @tparam Ts... A list of types to be compared against.
 */
template <typename T1, typename... Ts>
using is_one_of =
    negation<std::is_same<bool_pack<false, std::is_same<T1, Ts>::value...>,
                          bool_pack<std::is_same<T1, Ts>::value..., false>>>;

/**
 * @brief Enables T1 if it is the same type as T2.
 * @tparam T1 The subject type.
 * @tparam T2 The objective type.
 */
template <typename T1, typename T2>
using requires_is_same_t = enable_if_t<std::is_same<T1, T2>::value, T1>;

// TODO(Chris): Removed to avoid compiler bug in MSVC.
/**
 * @brief Enables T1 if it is not any of the other types.
 * @tparam T1 The subject type.
 * @tparam Ts... The prohibited types.
 */
// template <typename T1, typename... Ts>
// using requires_is_none_of_t = enable_if_t<!is_one_of<T1, Ts...>::value, T1>;

/**
 * @brief Extracts the value type for a vec, matches the type for a scalar.
 * @tparam T Type of the scalar
 */
template <typename T>
struct scalar_type {
  using type = T;
};

/**
 * @brief Specialization for vec class
 * @tparam T Underlying type of the vec
 * @tparam N Number of vector elements
 */
template <typename T, int N>
struct scalar_type<::cl::sycl::vec<T, N>> {
  using type = T;
};

/**
 * @brief Specialization for swizzled_vec class
 * @tparam T Underlying type of the swizzled_vec
 * @tparam kElems Number of original vec elements
 * @tparam Indexes The indexes used when accessing the original vec
 */
template <typename T, int kElems, int... Indexes>
struct scalar_type<::cl::sycl::swizzled_vec<T, kElems, Indexes...>> {
  using type = T;
};

/**
 * @brief Retrieves the underlying type of a vec
 * @tparam T Type to collapse to a scalar
 */
template <typename T>
using scalar_type_t = typename scalar_type<T>::type;

/**
 * @brief Retrieves the underlying vec from a swizzled_vec
 * @param V Type to collapse to a vec
 */
template <typename V>
struct collapse_swizzled_vec {
  /**
   * @brief In the general case, just return the same type
   */
  using type = V;
};

/**
 * @brief Retrieves the underlying vec from a swizzled_vec,
 *        specialization for swizzled_vec
 * @tparam T Underlying type of the swizzled_vec
 * @tparam kElems Number of original vec elements
 * @tparam Indexes The indexes used when accessing the original vec
 */
template <typename T, int kElems, int... Indexes>
struct collapse_swizzled_vec<swizzled_vec<T, kElems, Indexes...>> {
  /**
   * @brief swizzled_vec collapses to a vec
   */
  using type = vec<T, sizeof...(Indexes)>;
};

/**
 * @brief Retrieves the underlying vec from a swizzled_vec,
 *        specialization for 1-elem swizzled_vec
 * @tparam T Underlying type of the swizzled_vec
 * @tparam kElems Number of original vec elements
 * @tparam s0 The index used when accessing the original vec
 */
template <typename T, int kElems, int s0>
struct collapse_swizzled_vec<swizzled_vec<T, kElems, s0>> {
  /**
   * @brief 1-elem swizzled_vec collapses to a scalar
   */
  using type = T;
};

/**
 * @brief Transforms a 1-elem vec into a scalar
 * @tparam T Underlying type of the vec
 */
template <typename T>
struct collapse_swizzled_vec<vec<T, 1>> {
  /**
   * @brief 1-elem vec collapses to a scalar
   */
  using type = T;
};

/**
 * @brief Retrieves the underlying vec from a swizzled_vec
 * @tparam V Type to collapse to a vec
 */
template <typename V>
using collapse_swizzled_vec_t = typename collapse_swizzled_vec<V>::type;

/**
 * @brief Checks whether the type is convertible to and from a half
 * @tparam T Type to check
 */
template <typename T>
using is_half_convertible =
    std::integral_constant<bool, (std::is_convertible<T, half>::value &&
                                  std::is_convertible<half, T>::value)>;

/**
 * @brief Checks whether T is a custom half type
 *        - either half or convertible to it
 * @tparam T Type to check
 */
template <typename T>
using is_custom_half_type =
    std::integral_constant<bool, (std::is_same<half, decay_t<T>>::value ||
                                  (!std::is_floating_point<T>::value &&
                                   !std::is_integral<T>::value &&
                                   is_half_convertible<T>::value))>;

/**
 * @brief Returns half if T is convertible to half
 * @tparam T Type to convert
 */
template <typename T>
struct common_half_type {
  using type =
      typename std::enable_if<cl::sycl::detail::is_half_convertible<T>::value,
                              half>::type;
};

/**
 * @brief Returns half if T is convertible to half
 * @tparam T Type to convert
 */
template <typename T>
using common_half_type_t = typename common_half_type<T>::type;

/**
 * @brief Helper struct to retrieve the common type of multiple types
 * @tparam Ts Types to find the common type for
 */
template <typename... Ts>
struct common_type_helper {
  using type = typename std::common_type<Ts...>::type;
};

/**
 * @brief Helper struct to retrieve the common type of multiple types,
 *        specialization for half as first parameter
 * @tparam T Type used to find find common_type<half, T>
 */
template <typename T>
struct common_type_helper<half, T> : common_half_type<T> {};

/**
 * @brief Helper struct to retrieve the common type of multiple types,
 *        specialization for half as second parameter
 * @tparam T Type used to find find common_type<T, half>
 */
template <typename T>
struct common_type_helper<T, half> : common_type_helper<half, T> {};

/**
 * @brief Helper struct to retrieve the common type of multiple types,
 *        specialization for half as both parameters
 */
template <>
struct common_type_helper<half, half> {
  using type = half;
};

/**
 * @brief Finds the common type of multiple types
 * @tparam Ts Types to find the common type for
 */
template <typename... Ts>
using common_type_t = typename common_type_helper<Ts...>::type;

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        When mixing vectors and scalars, a vector will be returned.
 *        Otherwise, common_type will be used.
 *
 * @tparam Ts Types to find the common return type for
 */
template <typename... Ts>
struct common_return_helper {
  using type = common_type_t<collapse_swizzled_vec_t<Ts>...>;
};

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        Specialization for vec as first parameter, underlying type of the vec
 *        as second parameter.
 *
 * @tparam T Underlying type of the vec
 * @tparam N Number of vector elements
 */
template <class T, int N>
struct common_return_helper<vec<T, N>, T> : common_return_helper<vec<T, N>> {};

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        Specialization for underlying type of the vec as first parameter,
 *        vec as second parameter.
 *
 * @tparam T Underlying type of the vec
 * @tparam N Number of vector elements
 */
template <class T, int N>
struct common_return_helper<T, vec<T, N>> : common_return_helper<vec<T, N>> {};

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        Specialization for getting the common return type
 *        of two 1-elem swizzled_vec parameters.
 *
 * @tparam T Underlying type of the swizzled_vec
 * @tparam kElemsX Number of original vec elements of the first swizzled_vec
 * @tparam s0x The index used when accessing the original vec
 *         of the first swizzled_vec
 * @tparam kElemsY Number of original vec elements of the second swizzled_vec
 * @tparam s0y The index used when accessing the original vec
 *         of the second swizzled_vec
 * @note Common return type is a scalar
 */
template <class T, int kElemsX, int kElemsY, int s0x, int s0y>
struct common_return_helper<swizzled_vec<T, kElemsX, s0x>,
                            swizzled_vec<T, kElemsY, s0y>>
    : common_return_helper<T> {};

#ifndef __SYCL_DEVICE_ONLY__

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        Host specialization for getting the common return type
 *        of a 1-elem vec parameter and a 1-elem swizzled_vec.
 *
 * @tparam T Underlying type of the vec and swizzled_vec
 * @tparam kElems Number of original vec elements of the swizzled_vec
 * @tparam s0 The index used when accessing the original vec
 *         of the swizzled_vec
 * @note Common return type is a scalar
 */
template <class T, int kElems, int s0>
struct common_return_helper<vec<T, 1>, swizzled_vec<T, kElems, s0>>
    : common_return_helper<T> {};

/**
 * @brief Helper struct to retrieve the common return type of multiple types.
 *
 *        Host specialization for getting the common return type
 *        of a 1-elem swizzled_vec and a 1-elem vec parameter.
 *
 * @tparam T Underlying type of the vec and swizzled_vec
 * @tparam kElems Number of original vec elements of the swizzled_vec
 * @tparam s0 The index used when accessing the original vec
 *         of the swizzled_vec
 * @note Common return type is a scalar
 */
template <class T, int kElems, int s0>
struct common_return_helper<swizzled_vec<T, kElems, s0>, vec<T, 1>>
    : common_return_helper<T> {};

#endif  // __SYCL_DEVICE_ONLY__

/**
 * @brief Retrieves the common return type of multiple types.
 *
 *        When mixing vectors and scalars, a vector will be returned.
 *        Otherwise, common_type will be used.
 *
 * @tparam Ts Types to find the common return type for
 */
template <typename... Ts>
using common_return = common_return_helper<decay_t<Ts>...>;

/**
 * @brief Retrieves the common return type of multiple types.
 *
 *        When mixing vectors and scalars, a vector will be returned.
 *        Otherwise, common_type will be used.
 *
 * @tparam Ts Types to find the common return type for
 */
template <typename... Ts>
using common_return_t = typename common_return<Ts...>::type;

/**
 * @brief Checks whether the types are the same, ignoring references and
 *        cv-qualifiers
 * @tparam First The first type
 * @tparam Second The second type
 */
template <typename First, typename Second>
struct is_same_basic_type : std::is_same<decay_t<First>, decay_t<Second>> {};

/**
 * @brief Deduces the cv-qualifiers necessary for a decaying a pointer to void*.
 *
 * Given that `void* foo = bar` is not possible when `bar` is a
 * pointer-to-cv-qualified type, `void_ptr` deduces the correct type.
 * @note The primary template handles T* as well.
 */
template <typename T>
struct void_ptr {
  using type = void*;
};

/**
 * @brief Deduces decay to void* for const types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<const T> {
  using type = const void*;
};

/**
 * @brief Deduces decay to void* for pointer-to-const types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<const T*> {
  using type = const void*;
};

/**
 * @brief Deduces decay to void* for volatile types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<volatile T> {
  using type = volatile void*;
};

/**
 * @brief Deduces decay to void* for pointer-to-volatile types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<volatile T*> {
  using type = volatile void*;
};

/**
 * @brief Deduces decay to void* for const volatile types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<const volatile T> {
  using type = const volatile void*;
};

/**
 * @brief Deduces decay to void* for pointer-to-const-volatile types.
 * @seealso void_ptr<T>.
 */
template <typename T>
struct void_ptr<const volatile T*> {
  using type = const volatile void*;
};

template <typename T>
using void_ptr_t = typename void_ptr<T>::type;

}  // namespace detail
}  // namespace sycl
}  // namespace cl

#endif  // RUNTIME_INCLUDE_SYCL_TYPE_TRAITS_H_
