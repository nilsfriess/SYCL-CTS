/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2023 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
*******************************************************************************/

//  Provides tests for the exception that are thrown by [[sycl::device_has]]
//  attribute.

#include "../common/disabled_for_test_case.h"
#include "catch2/catch_template_test_macros.hpp"
#include "kernel_features_common.h"

namespace kernel_features_device_has_exceptions {
using namespace sycl_cts;
using namespace kernel_features_common;

using AtomicRefT =
    sycl::atomic_ref<unsigned long long, sycl::memory_order::relaxed,
                     sycl::memory_scope::device>;

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_feature;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel that uses the tested feature but does not have any attribute "
 "[[sycl::device_has()]]",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname = kernel_use_feature<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() { USE_FEATURE(FeatureTypeT); };
    const auto lambda_item_arg = [](sycl::item<1>) {
      USE_FEATURE(FeatureTypeT);
    };
    const auto lambda_group_arg = [](sycl::group<1>) {
      USE_FEATURE(FeatureTypeT);
    };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT = non_decorated_call_use_feature<FeatureTypeT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(is_exception_expected, expected_errc, queue,
                        NO_ATTRIBUTE, kname, USE_FEATURE(FeatureTypeT));
  }
});

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_feature_function_non_decorated;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel that calls a function that uses the tested feature. Neither the "
 "kernel nor the function have an attribute ",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname =
      kernel_use_feature_function_non_decorated<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() {
      use_feature_function_non_decorated<FeatureTypeT>();
    };
    const auto lambda_item_arg = [](sycl::item<1>) {
      use_feature_function_non_decorated<FeatureTypeT>();
    };
    const auto lambda_group_arg = [](sycl::group<1>) {
      use_feature_function_non_decorated<FeatureTypeT>();
    };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT = non_decorated_call_non_decorated_function<FeatureTypeT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(is_exception_expected, expected_errc, queue,
                        NO_ATTRIBUTE, kname,
                        use_feature_function_non_decorated<FeatureTypeT>());
  }
});

#ifdef SYCL_EXTERNAL

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_feature_function_external_decorated;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel does not have the attribute [[sycl::device_has()]] but it "
 "calls a SYCL_EXTERNAL function which uses the tested feature. The "
 "SYCL_EXTERNAL function is defined in another translation unit. The "
 "SYCL_EXTERNAL function is declared with the corresponding attribute "
 "[[sycl::device_has()]].",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname = kernel_use_feature_function_external_decorated<FeatureTypeT,
                                                               FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_item_arg = [](sycl::item<1>) {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_group_arg = [](sycl::group<1>) {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT =
        non_decorated_call_decorated_external_function<FeatureTypeT,
                                                       FeatureAspectT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(
        is_exception_expected, expected_errc, queue, NO_ATTRIBUTE, kname,
        use_feature_function_external_decorated<FeatureTypeT,
                                                FeatureAspectT>());
  }
});
#endif

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_dummy_function_non_decorated;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel does not use the tested feature but is decorated with the "
 "corresponding attribute [[sycl::device_has()]].",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname =
      kernel_dummy_function_non_decorated<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() [[sycl::device_has(FeatureAspectT)]] {
      dummy_function_non_decorated();
    };
    const auto lambda_item_arg = [](sycl::item<1>)
                                     [[sycl::device_has(FeatureAspectT)]] {
                                       dummy_function_non_decorated();
                                     };
    const auto lambda_group_arg = [](sycl::group<1>)
                                      [[sycl::device_has(FeatureAspectT)]] {
                                        dummy_function_non_decorated();
                                      };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT = decorated_call_non_decorated_dummy<FeatureAspectT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(is_exception_expected, expected_errc, queue,
                        [[sycl::device_has(FeatureAspectT)]], kname,
                        dummy_function_non_decorated());
  }
});

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_dummy_function_decorated;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel that calls a function which is decorated with the feature's "
 "corresponding attribute [[sycl::device_has()]]. Neither the kernel "
 "nor the function use the feature.",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname = kernel_dummy_function_decorated<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() {
      dummy_function_decorated<FeatureAspectT>();
    };
    const auto lambda_item_arg = [](sycl::item<1>) {
      dummy_function_decorated<FeatureAspectT>();
    };
    const auto lambda_group_arg = [](sycl::group<1>) {
      dummy_function_decorated<FeatureAspectT>();
    };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT = non_decorated_call_decorated_dummy<FeatureAspectT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(is_exception_expected, expected_errc, queue,
                        NO_ATTRIBUTE, kname,
                        dummy_function_decorated<FeatureAspectT>());
  }
});

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_feature_function_decorated;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel that calls a function which is decorated with the feature's "
 "corresponding attribute [[sycl::device_has()]]. Function uses the "
 "feature and kernel doesn't.",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname =
      kernel_use_feature_function_decorated<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Check if the device supports testing feature
  bool is_exception_expected = true;
  if (queue.get_device().has(FeatureAspectT)) {
    is_exception_expected = false;
  }

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  {
    const auto lambda_no_arg = []() {
      use_feature_function_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_item_arg = [](sycl::item<1>) {
      use_feature_function_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_group_arg = [](sycl::group<1>) {
      use_feature_function_decorated<FeatureTypeT, FeatureAspectT>();
    };

    run_separate_lambda<kname>(is_exception_expected, expected_errc, queue,
                               lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT =
        non_decorated_call_decorated_function<FeatureTypeT, FeatureAspectT>;

    run_functor<FunctorT>(is_exception_expected, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(
        is_exception_expected, expected_errc, queue, NO_ATTRIBUTE, kname,
        use_feature_function_decorated<FeatureTypeT, FeatureAspectT>());
  }
});

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_another_feature;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel with tested feature but with attribute [[sycl::device_has()]] "
 "for another feature.",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname = kernel_use_another_feature<FeatureTypeT, FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  static constexpr sycl::aspect AnotherFeatureAspect =
      get_another_aspect<FeatureAspectT>();
  bool other_feature_exception_expect = true;
  if (queue.get_device().has(AnotherFeatureAspect) &&
      queue.get_device().has(FeatureAspectT)) {
    other_feature_exception_expect = false;
  }

  {
    const auto lambda_no_arg = []() [[sycl::device_has(AnotherFeatureAspect)]] {
      USE_FEATURE(FeatureTypeT);
    };
    const auto lambda_item_arg =
        [](sycl::item<1>) [[sycl::device_has(AnotherFeatureAspect)]] {
          USE_FEATURE(FeatureTypeT);
        };
    const auto lambda_group_arg =
        [](sycl::group<1>) [[sycl::device_has(AnotherFeatureAspect)]] {
          USE_FEATURE(FeatureTypeT);
        };

    run_separate_lambda<kname>(other_feature_exception_expect, expected_errc,
                               queue, lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT =
        decorated_call_use_feature<FeatureTypeT, AnotherFeatureAspect>;

    run_functor<FunctorT>(other_feature_exception_expect, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(other_feature_exception_expect, expected_errc, queue,
                        [[sycl::device_has(AnotherFeatureAspect)]], kname,
                        USE_FEATURE(FeatureTypeT));
  }

});

#ifdef SYCL_EXTERNAL

template <typename FeatureTypeT, sycl::aspect FeatureAspectT>
class kernel_use_feature_function_external_decorated_with_attr;

DISABLED_FOR_TEMPLATE_TEST_CASE_SIG(ComputeCpp, hipSYCL)
("Kernel with attribute [[sycl::device_has()]] for not currently tested "
 "feature but with SYCL_EXTERNAL function with tested feature and "
 "attribute [[sycl::device_has()]] with tested feature that is defined in "
 "another translation unit.",
 "[kernel_features]",
 ((typename FeatureTypeT, sycl::aspect FeatureAspectT), FeatureTypeT,
  FeatureAspectT),
 (sycl::half, sycl::aspect::fp16), (double, sycl::aspect::fp64),
 (AtomicRefT, sycl::aspect::atomic64))({
  using kname =
      kernel_use_feature_function_external_decorated_with_attr<FeatureTypeT,
                                                               FeatureAspectT>;
  auto queue = util::get_cts_object::queue();

  // Set expected error code
  constexpr sycl::errc expected_errc = sycl::errc::kernel_not_supported;

  static constexpr sycl::aspect AnotherFeatureAspect =
      get_another_aspect<FeatureAspectT>();
  bool other_feature_exception_expect = true;
  if (queue.get_device().has(AnotherFeatureAspect) &&
      queue.get_device().has(FeatureAspectT)) {
    other_feature_exception_expect = false;
  }

  {
    const auto lambda_no_arg = []() [[sycl::device_has(AnotherFeatureAspect)]] {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_item_arg = [](sycl::item<1>) [[sycl::device_has(
                                     AnotherFeatureAspect)]] {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };
    const auto lambda_group_arg = [](sycl::group<1>) [[sycl::device_has(
                                      AnotherFeatureAspect)]] {
      use_feature_function_external_decorated<FeatureTypeT, FeatureAspectT>();
    };

    run_separate_lambda<kname>(other_feature_exception_expect, expected_errc,
                               queue, lambda_no_arg, lambda_item_arg,
                               lambda_group_arg);
  }

  {
    using FunctorT = decorated_call_decorated_external_function<
        FeatureTypeT, AnotherFeatureAspect, FeatureAspectT>;

    run_functor<FunctorT>(other_feature_exception_expect, expected_errc, queue);
  }

  {
    RUN_SUBMISSION_CALL(
        other_feature_exception_expect, expected_errc, queue,
        [[sycl::device_has(AnotherFeatureAspect)]], kname,
        use_feature_function_external_decorated<FeatureTypeT,
                                                FeatureAspectT>());
  }
});
#endif
}  // namespace kernel_features_device_has_exceptions
