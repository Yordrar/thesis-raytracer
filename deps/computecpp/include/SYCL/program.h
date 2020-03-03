/*****************************************************************************

    Copyright (C) 2002-2018 Codeplay Software Limited
    All Rights Reserved.

    Codeplay's ComputeCpp

*******************************************************************************/

/*!
 * @file program.h
 *
 * @brief This file implements the \ref cl::sycl::program class as defined by
 * the SYCL 1.2 specification.
 */

#ifndef RUNTIME_INCLUDE_SYCL_PROGRAM_H_
#define RUNTIME_INCLUDE_SYCL_PROGRAM_H_

#include "SYCL/common.h"
#include "SYCL/context.h"
#include "SYCL/device.h"
#include "SYCL/info.h"
#include "SYCL/kernel.h"

namespace cl {
namespace sycl {

namespace info {

/**
 * @brief Program descriptor to query information about a program object
 */
enum class program : int {
  reference_count, /*!< Query the reference count of the program */
  context,         /*!< Query the cl_context associate with the program */
  devices          /*!< Query the set of devices the program is built against */
};

}  // namespace info

/// @cond COMPUTECPP_DEV

/// Program info definition
COMPUTECPP_DEFINE_SYCL_INFO_HANDLER(program, cl_program_info, cl_program)

/// Program info definition
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(program, reference_count,
                                      CL_PROGRAM_REFERENCE_COUNT, cl_uint,
                                      cl_uint)
/// Program info definition
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(program, context, CL_PROGRAM_CONTEXT,
                                      cl::sycl::context, cl_context)
/// Program info definition
COMPUTECPP_DEFINE_SYCL_INFO_PARAMETER(program, devices, CL_PROGRAM_DEVICES,
                                      vector_class<cl::sycl::device>,
                                      cl_device_id)

/// Host program info definition
COMPUTECPP_DEFINE_SYCL_INFO_HOST(program, reference_count, 0)
/// Host program info definition
COMPUTECPP_DEFINE_SYCL_INFO_HOST(program, context, cl::sycl::context())
/// Host program info definition
COMPUTECPP_DEFINE_SYCL_INFO_HOST(program, devices, vector_class<device>())

/// COMPUTECPP_DEV @endcond

/**
 * @brief Enum describing the build state of the program
 */
enum class program_state { none, compiled, linked };

/*!
@brief Public facing program class that provides an interface for abstracting
the construction and building of a cl_program object, See section 3.5.5 of the
SYCL 1.2 specification.
*/
class COMPUTECPP_EXPORT program {
  /*!
  @brief Friend class declaration of kernel as the program class requires access
  to the kernel classes protected constructor.
  */
  friend class kernel;

 public:
  /*!
  @brief Constructor that takes a context. Initialises the cl_program to
  nullptr.
  @param context A reference to the context that the cl_program will be
  associated with.
  */
  explicit program(const context& context);

  /*!
  @brief Constructor that takes a context and a list of devices. Initialises the
  cl_program to nullptr.
  @param deviceList A list of devices that the program will be associated with.
  */
  program(const context& context, vector_class<device> deviceList);

  /*!
  @brief Inter-op constructor that takes a context and a cl_program. Note that
  the clProgram param must have previously been created from the underlying
  cl_context of the context parameter and the underlying cl_devices from the
  list of devices parameter.
  @param context A reference to the context that the cl_program will be
  associated with.
  @param clProgram The cl_program that the program will be assigned to.
  */
  program(const context& context, cl_program clProgram);

  /*!
  @brief Linker constructor that takes a list of programs and a string
  specifying link options. Note that calling this constructor is invalid
  if the cl_program has already been successfully built or linked via either
  link(string_class), build_with_kernel_type(string_class) or
  program(vector_class<program>, string_class).
  @throw This constructor can throw a cl::sycl::exception
  @param programList The list programs that the cl_program will be constructed
  from.
  @param linkOptions The string specifying the link options the cl_program will
  be linked with.
  */
  program(vector_class<program> programList, string_class linkOptions = "");

  /*!
  @brief Copy constructor that initialises a copy of the program with the same
  underlying cl_program, associated context and list of associated devices.
  @param rhs The program being copied from.
  */
  program(const program& rhs) : m_impl{rhs.get_impl()} {}

  /*!
  @brief Assignment operator that initialises a copy of the program with the
  same underlying cl_program, associated context and list of associated devices.
  @param rhs The program being assigned from.
  */
  program& operator=(const program& rhs) {
    this->m_impl = rhs.get_impl();
    return *this;
  }

  /*!
  @brief Copy constructor that initialises a copy of the program with the same
  underlying cl_program, associated context and list of associated devices.
  @param rhs The program being copied from.
  */
  program(program&& rhs) : m_impl{std::move(rhs.m_impl)} {}

  /*!
  @brief Assignment operator that initialises a copy of the program with the
  same underlying cl_program, associated context and list of associated devices.
  @param rhs The program being assigned from.
  */
  program& operator=(program&& rhs) {
    this->m_impl = rhs.get_impl();
    return *this;
  }

  /**
   * @brief Determines if lhs and rhs are equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if same underlying object
   */
  friend inline bool operator==(const program& lhs, const program& rhs) {
    return (lhs.get_impl() == rhs.get_impl());
  }

  /**
   * @brief Determines if lhs and rhs are not equal
   * @param lhs Left-hand-side object in comparison
   * @param rhs Right-hand-side object in comparison
   * @return True if different underlying objects
   */
  friend inline bool operator!=(const program& lhs, const program& rhs) {
    return !(lhs == rhs);
  }

  /*!
  @brief Destructor that releases the cl_program.
  */
  COMPUTECPP_TEST_VIRTUAL ~program() = default;

  /*!
  @brief Compile a SYCL kernel using his name and optional custom compile
  options. This function produce a ready to link program.
  Note that calling this member function is invalid if the program has
  already been successfully compiled, built or linked via either
  link(string_class), compile_with_kernel_type(string_class),
  build_with_kernel_type(string_class) or program(vector_class<program>,
  string_class).
  @tparam kernelT Typename specifying the name of the kernel to be compiled.
  @param compileOptions The string specifying the compile options that will be
  provide to the underlying OpenCL API.
  */
  template <typename kernelT>
  void compile_with_kernel_type(string_class compileOptions = "") {
    if (detail::kernel_info<kernelT>::name == nullptr) {
      COMPUTECPP_CL_ERROR_CODE(
          0, detail::cpp_error_code::KERNEL_NOT_FOUND_ERROR, nullptr);
    }

    std::string kernelName = detail::kernel_info<kernelT>::name;
    const detail::kernel_binary_info& binInfo =
        *program::select_kernel_binary_info(
             detail::kernel_info<kernelT>::bin_info,
             detail::kernel_info<kernelT>::bin_count)
             .binary_info;
    detail::binary_address binaryData = binInfo.data;
    const size_t binarySize = binInfo.data_size;
    const char* const* const requiredExtensions = binInfo.used_extensions;
    if (binInfo.target == nullptr) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          0, detail::cpp_error_code::TARGET_NOT_FOUND_ERROR, nullptr,
          binInfo.target);
    }
    const std::string target{binInfo.target};

    this->compile_with_kernel_type_impl(binaryData, (cl_int)binarySize,
                                        requiredExtensions, compileOptions,
                                        target);
  }

  /*!
   * Compiles a program from the given OpenCL C kernel source.
   * Note that calling this member function is invalid if the program has
   * already been successfully compiled, built or linked via either
   * link(string_class), compile_with_kernel_type(string_class),
   * build_with_kernel_type(string_class) or program(vector_class<program>,
   * string_class).
   * @param kernelSource to compile
   * @param compilation options for the source
   */
  void compile_with_source(string_class kernelSource,
                           string_class compileOptions = "");

  /*!
   * Creates a valid cl_program from a pre-built kernel provided by the
   * underlying OpenCL implementation.
   * @param kernel name
   */
  void create_from_built_in_kernel(const string_class& kernelName);

  /*!
  @brief Build a SYCL kernel using its name and optional custom build options.
  This function produces a ready-to-run program.
  Note that calling this member function is invalid if the program has
  already been successfully compiled, built or linked via either
  link(string_class), compile_with_kernel_type(string_class),
  build_with_kernel_type(string_class) or program(vector_class<program>,
  string_class).
  @tparam kernelT Typename specifying the name of the kernel to be built.
  @param buildOptions The string specifying the build options to provide to the
  underlying OpenCL API.
  */
  template <typename kernelT>
  void build_with_kernel_type(string_class buildOptions = "") {
    if (detail::kernel_info<kernelT>::name == nullptr) {
      COMPUTECPP_CL_ERROR_CODE(
          0, detail::cpp_error_code::KERNEL_NOT_FOUND_ERROR, nullptr);
    }

    std::string kernelName = detail::kernel_info<kernelT>::name;
    const detail::kernel_binary_info& binInfo =
        *program::select_kernel_binary_info(
             detail::kernel_info<kernelT>::bin_info,
             detail::kernel_info<kernelT>::bin_count)
             .binary_info;
    detail::binary_address binaryData = binInfo.data;
    const size_t binarySize = binInfo.data_size;
    const char* const* const requiredExtensions = binInfo.used_extensions;
    if (binInfo.target == nullptr) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          0, detail::cpp_error_code::TARGET_NOT_FOUND_ERROR, nullptr,
          binInfo.target);
    }

    this->build_with_kernel_type_impl(binaryData, (cl_int)binarySize,
                                      requiredExtensions, buildOptions,
                                      binInfo.target);
  }

  /*!
  @brief Creates and builds a program from OpenCL C kernel source and optional
         build options. This function produces a ready-to-run program.
  @param kernelSource Source of the OpenCL kernel.
  @param buildOptions The string specifying the build options to provide to the
         underlying OpenCL API.
  */
  void build_with_source(string_class kernelSource,
                         string_class buildOptions = "");

  /*!
  @brief Link all compiled programs using the (optional) link options.
  This function produce a ready-to-run program using a compiled program. Note
  that calling this member function is invalid if the cl_program has already
  been successfully built or linked via either link(string_class),
  build_with_kernel_type(string_class) or program(vector_class<program>,
  string_class).
  @param linkOptions String specifying the link options to provide to the
  underlying OpenCL API.
  */
  void link(string_class linkOptions = "");

  /*!
  @brief Checks whether the program contains a kernel specified by the type.
  @tparam kernelT Typename specifying the name of the kernel to be returned.
  @return True if the SYCL kernel function defined by the type kernelT
          is an available kernel, either within the encapsulated cl_program
          (if this SYCL program is an OpenCL program), or on the host,
          otherwise false.
  */
  template <typename kernelT>
  bool has_kernel() const {
    const char* kernelName = detail::kernel_info<kernelT>::name;
    if (kernelName) {
      return has_kernel(kernelName);
    } else {
      /* Host mode or name not found */
      return has_kernel("");
    }
  }

  /*!
  @brief Checks whether the program contains a kernel specified by the name.
  @tparam kernelT Typename specifying the name of the kernel to be returned.
  @return True if the OpenCL C kernel function defined by kernelName is an
          available kernel within the encapsulated cl_program and this SYCL
          program is not a host program, otherwise false.
  */
  bool has_kernel(string_class kernelName) const;

  /*!
  @brief Retrieve a SYCL \ref kernel object described by the typename kernelT.
  @tparam kernelT Typename specifying the name of the kernel to be returned.
  @return The kernel that has been created from the kernel name parameter.
  */
  template <typename kernelT>
  kernel get_kernel() const {
    const char* kernelName = detail::kernel_info<kernelT>::name;
    if (kernelName) {
      return get_kernel(kernelName);
    } else {
      /* Host mode or name not found */
      return get_kernel("");
    }
  }

  /*!
  @brief Retrieve a SYCL \ref kernel object described by the kernel name.
  @param kernelName The string specifying the kernel name.
  @return The kernel that has been created form the kernel name parameter.
  */
  kernel get_kernel(string_class kernelName) const;

  /*!
  @brief Retrieves information about the program. The runtime query the OpenCL
  API and then converts the result into the SYCL representation before returning
  it.
  @tparam param Information to retrieve.
  @return The information in the SYCL format.
  */
  template <info::program param>
  COMPUTECPP_EXPORT
      typename info::param_traits<info::program, param>::return_type
      get_info() const;

  /*!
  @brief Return the list of binaries that were used to compile and link the
  program.
  @return The list of binaries that were used to compile and link the program.
  */
  vector_class<vector_class<char>> get_binaries() const;

  /*!
  @brief Retrieves the context associated with the program.
  @return Associated context.
  */
  context get_context() const;

  /*!
  @brief Return the list of devices associated with the program.
  @return The list of associated devices.
  */
  vector_class<cl::sycl::device> get_devices() const;

  /*!
  @brief Return the compile options used when compiling the program.
  @return A string specifying the compile options used when compiling the
          program.
  */
  std::string get_compile_options() const;

  /*!
  @brief Return the link options used when linking the program.
  @return A string specifying the link options used when linking the program.
  */
  std::string get_link_options() const;

  /*!
  @brief Return the build options used when building the program.
  @return A string specifying the build options used when building the program.
  */
  std::string get_build_options() const;

  /*!
  @brief Inter-op member function that returns the underlying cl_program.
  @return The underlying cl_program usable by the OpenCL API.
  */
  cl_program get() const;

  /*!
  @brief Return a bool specifying whether the program has been linked.
  @return True if the program has been linked.
  */
  bool is_linked() const;

  /*!
  @brief Returns whether the program was constructed from a host context
  @return True if program constructed from a host context
  */
  bool is_host() const;

  /*!
  @brief Retrieves the current build state of the program
  @return The build state of the program
  */
  program_state get_state() const;

  /// @cond COMPUTECPP_DEV
  /*!
  @brief Implementation-defined member function that returns the runtime's
  program implementation object.
  @return Opaque pointer to runtime's program implementation object.
  */
  dprogram_shptr get_impl() const;

  /*!
  @brief Returns a program for a kernel from a context.
  @tparam kernelT The kernel to build the program for.
  @param c The context that the kernel is to be created on.
  @return The created program.
  */
  template <typename kernelT>
  static program create_program_for_kernel(cl::sycl::context c) {
    if (detail::kernel_info<kernelT>::name == nullptr && !c.is_host()) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          CL_SUCCESS, detail::cpp_error_code::KERNEL_NOT_FOUND_ERROR,
          c.get_impl().get(),
          "Unable to retrieve kernel function, is integration header included?")
    } else if (c.is_host()) {
      return program(c);
    }
    // Otherwise, create a normal Program
    std::string kernelName = detail::kernel_info<kernelT>::name;
    const detail::kernel_binary_info& binInfo =
        *program::select_kernel_binary_info(
             detail::kernel_info<kernelT>::bin_info,
             detail::kernel_info<kernelT>::bin_count)
             .binary_info;
    const size_t dataSize = binInfo.data_size;
    detail::binary_address data = binInfo.data;
    const char* const* const requiredExtensions = binInfo.used_extensions;
    if (kernelName.empty()) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          0, detail::cpp_error_code::KERNEL_NOT_FOUND_ERROR, nullptr,
          kernelName.c_str());
    }
    if (binInfo.target == nullptr) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          0, detail::cpp_error_code::TARGET_NOT_FOUND_ERROR, nullptr,
          binInfo.target);
    }
    std::string target(binInfo.target);

    return program(create_program_for_kernel_impl(
        kernelName, data, (cl_int)dataSize, requiredExtensions, c.get_impl(),
        target));
  }
  /// COMPUTECPP_DEV @endcond

 protected:
  /// @cond COMPUTECPP_DEV
  /*!
  @brief Implementation-defined constructor that takes a detail program that
  initialises a copy of the program with the same underlying cl_program,
  associated context and list of associated devices.
  @param program The detail program being initialised from.
  */
  explicit program(dprogram_shptr program) : m_impl(std::move(program)) {}
  /// COMPUTECPP_DEV @endcond

  /*!
  @brief Inter-op member function that returns the underlying cl_program.
  @return The underlying cl_program usable by the OpenCL API.
  */
  cl_program get_no_retain() const;

 private:
  /*!
  @brief Implementation-defined member function that performs the implementation
  of the build_with_kernel_type() template member function. Assigns the
  cl_program to the result of the building; with the build options parameter, of
  the binary data and size parameters.
  @ref program::build_with_kernel_type
  @brief binaryData The data of the binary to be built.
  @brief binarySize The size of the binary to be built.
  @param requiredExtensions A list of extensions as strings.
  @param buildOptions The string specifying the build options the cl_program
  will be built with.
  @param target target string
  */
  void build_with_kernel_type_impl(detail::binary_address binaryData,
                                   cl_int binarySize,
                                   const char* const* const requiredExtensions,
                                   string_class buildOptions,
                                   string_class target);

  /*!
  @brief Implementation-defined member function that performs the implementation
  of the compile_with_kernel_type() template member function. Assigns the
  cl_program to the result of the compilation; with the compile options
  parameter, of
  the binary data and size parameters.
  @ref program::compile_with_kernel_type
  @brief binaryData The data of the binary to be compiled.
  @brief binarySize The size of the binary to be compiled.
  @param requiredExtensions A list of extensions as strings.
  @param buildOptions The string specifying the compile options the cl_program
  will be built with.
  @param target target string
  */
  void compile_with_kernel_type_impl(
      detail::binary_address binaryData, cl_int binarySize,
      const char* const* const requiredExtensions, string_class compileOptions,
      string_class target);

  /*!
  @brief Implementation-defined static member function that performs the
  implementation of create_program_for_kernel implementation-defined template
  member function. Returns a program for a kernel invoke API, either creating
  and caching the program or retrieving a cached program from a  previous call.
  @ref program::create_program_for_kernel
  @param kernelName The string specifying the name of the kernel to be built.
  @param binaryData The data of the binary to be built.
  @param dataSize The size of the binary to be built.
  @param requiredExtensions A list of extensions as strings.
  @param context The detail context that the program is to be associated with.
  @param target target string
  */
  static program create_program_for_kernel_impl(
      std::string kernelName, detail::binary_address binaryData,
      cl_int dataSize, const char* const* const requiredExtensions,
      dcontext_shptr context, string_class target);

 private:
  /*!
  @brief Friend class declaration of handler, as it needs to call this method
  to get the binary information to query the arguments.
  */
  friend class handler;

  /*!
  @brief Retrieve an appropriate binary to build a program.

  If a sycl stub file has more than one binary in it (e.g. spir64 and spirv64)
  the environment variable COMPUTECPP_TARGET_BITCODE can be used to select which
  binary this function will return. If the binary for the specified value is not
  found, the first one available will be returned.

  @param binList The list available binaries for this program.
  @param binListSize The length of `binList`.
  @return A reference to the selected binary.
  */
  template <typename BIType>
  static const BIType& select_kernel_binary_info(const BIType* binList,
                                                 size_t binListSize) {
    if ((binList == nullptr) || (binListSize == 0)) {
      COMPUTECPP_CL_ERROR_CODE_MSG(
          0, detail::cpp_error_code::BINARY_NOT_FOUND_ERROR, nullptr,
          "Unable to retrieve a binary, is integration header included?");
    }

    const auto endOfList = binList + binListSize;
    const auto targetKernel =
        std::find_if(binList, endOfList, [](const BIType& kernelBinInfo) {
          return should_use_binary_info_impl(*kernelBinInfo.binary_info);
        });

    if (targetKernel != endOfList) {
      return *targetKernel;
    }

    // The default case is to return the first kernel binary available.
    return binList[0];
  }

 private:
  /*!
  @brief Returns true iff the provided binary info should be used.

  This will return true if this is a PE ComputeCpp edition, the
  `COMPUTECPP_TARGET_BITCODE` is set, and the provided binary info has this
  bitcode type.

  Note that this function may return false for all binary infos, in that case
  no preference is indicated.

  @param binInfo The binary info to check.
  @return true iff the provided binary info should be used.
  */
  static bool should_use_binary_info_impl(
      const detail::kernel_binary_info& binInfo);

  /*! Shared pointer to the implementation-defined program object. */
  dprogram_shptr m_impl;
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
struct hash<cl::sycl::program> {
 public:
  /*!
  @brief enables calling an std::hash object as a function with the object to be
  hashed as a parameter
  @param object the object to be hashed
  @tparam std the std namespace where this specialization resides
  */
  size_t operator()(const cl::sycl::program& object) const {
    hash<cl::sycl::dprogram_shptr> hasher;
    return hasher(object.get_impl());
  }
};
}  // namespace std
#endif  // RUNTIME_INCLUDE_SYCL_PROGRAM_H_
