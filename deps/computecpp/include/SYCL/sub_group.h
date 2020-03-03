/*****************************************************************************
  Copyright (C) 2002-2019 Codeplay Software Limited

  All Rights Reserved.
  Codeplay's ComputeCpp
*****************************************************************************/

/*!
  @file sub_group.h
  @brief This file contains the API for cl::sycl::sub_group
*/

#ifndef RUNTIME_INCLUDE_SYCL_SUB_GROUP_H_
#define RUNTIME_INCLUDE_SYCL_SUB_GROUP_H_

#include "SYCL/common.h"
#include "SYCL/group_base.h"
#include "SYCL/id.h"
#include "SYCL/range.h"

namespace cl {
namespace sycl {
namespace codeplay {
/*
 * @brief The sub_group class is an interface for sub groups
 */
class sub_group {
 public:
  template <int dimensions>
  friend class ::cl::sycl::nd_item;

  /*
   * @brief Getter function for the sub group range
   * @return The subgroup range
   */
  inline range<1> get_group_range() const noexcept {
    return range<1>(m_subGroupRange);
  }

  /*
   * @brief Getter function for the sub group range
   * @return The sub group id
   */
  inline id<1> get_group_id() const noexcept { return id<1>(m_subGroupId); }

  /*
   * @brief Getter function for the sub group range
   * @return The local range
   */
  inline range<1> get_local_range() const noexcept {
    return range<1>(m_localRange);
  }

  /*
   * @brief Getter function for the local id
   * @return The local id
   */
  inline id<1> get_local_id() const noexcept { return id<1>(m_localId); }

 private:
  /*
   * @brief Private consturctor to enforce construction by
   * nd_item::get_sub_group()
   */
  sub_group(size_t subGroupId, size_t subGroupRange, size_t localId,
            size_t localRange)
      : m_subGroupId(subGroupId),
        m_subGroupRange(subGroupRange),
        m_localId(localId),
        m_localRange(localRange) {}

  size_t m_subGroupId;     // sub group id struct
  size_t m_subGroupRange;  // sub group range
  size_t m_localId;        // local id struct
  size_t m_localRange;     // local range struct
};

}  // namespace codeplay
}  // namespace sycl
}  // namespace cl
#endif  // RUNTIME_INCLUDE_SYCL_SUB_GROUP_H_
