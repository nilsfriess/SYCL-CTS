/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  This test calls sycl::has_kernel_bundle<all bundle_states>(ctx, kernelId)
//  for kernels with this six kernels (2x kernel_cpu, 2x kernel_gpu,
//  2x kernel_accelerator).
//    1) Call sycl::has_kernel_bundle only with one kernel: with kernel_cpu,
//  kernel_gpu and kernel_accelerator.
//    2) Unite call results for two kernels, for example call result for
//  kernel_cpu and kernel_gpu (union using && operator). Save union result to
//  varaible with name "expected"
//    3) Call sycl::has_kernel_bundle with combination of these kernels:
//  kernel_cpu and kernel_gpu, kernel_cpu and kernel_gpu, kernel_gpu and
//  kernel_accelerator, kernel_cpu and second_kernel_cpu etc.
//
//  The test verifies that variable value "expected" is equal to
//  sycl::has_kernel_bundle<sycl::bundle_state>(context, kernelIds) calling
//  result.
//
*******************************************************************************/

#include "../common/common.h"
#include "../common/type_coverage.h"
#include "has_kernel_bundle_multiple_kernels.h"

#define TEST_NAME has_kernel_bundle_multiple_kernels_k_id

namespace TEST_NAMESPACE {
using namespace sycl_cts;
using namespace sycl_cts::tests::has_kernel_bundle;

template <sycl::bundle_state BundleState>
using verifier = check::multiple_kernels<BundleState, overload::id::ctx_kid>;

class TEST_NAME : public sycl_cts::util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
    sycl::device dev = util::get_cts_object::device();
    sycl::context ctx(dev.get_platform().get_devices());

    verifier<sycl::bundle_state::input>::run(log, dev);
    verifier<sycl::bundle_state::object>::run(log, dev);
    verifier<sycl::bundle_state::executable>::run(log, dev);
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;

}  // namespace TEST_NAMESPACE
