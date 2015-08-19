#include <vector>

#include <aslam/common/entrypoint.h>
#include <aslam/common/stl-helpers.h>
#include <gtest/gtest.h>

using namespace aslam;

TEST(StlHelpers, erase_indices) {
  std::vector<int> test_vector = {0, 1, 2, 3, 4, 5};
  std::vector<size_t> indices_to_remove = {2, 4};

  std::vector<int> result_vector =
      aslam::common::eraseIndicesFromVector(test_vector, indices_to_remove);
  ASSERT_EQ(result_vector.size(), test_vector.size() - indices_to_remove.size());

  std::vector<int> expected_result = {0, 1, 3, 5};
  ASSERT_EQ(result_vector.size(), expected_result.size());
  for (size_t idx = 0u; idx < result_vector.size(); ++idx) {
    EXPECT_EQ(expected_result[idx], result_vector[idx]);
  }
}

ASLAM_UNITTEST_ENTRYPOINT
