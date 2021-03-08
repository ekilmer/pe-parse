#include <pe-parse/parse.h>

#include <gtest/gtest.h>

#include "utils.h"

namespace fs = std::filesystem;

// Whether the corkami testset has been downloaded
// Path to corkami PEs
#if defined(CORKAMI_PE_PATH)
namespace peparse {

class CorkamiTestSuite : public testing::TestWithParam<fs::path> {};

TEST_P(CorkamiTestSuite, Corkami) {
  auto path = GetParam();
  auto *p = ParsePEFromFile(path.c_str());
  EXPECT_TRUE(p) << GetPEErrString() << " at " << GetPEErrLoc();
  DestructParsedPE(p);
}

INSTANTIATE_TEST_SUITE_P(CorkamiTests,
                         CorkamiTestSuite,
                         testing::ValuesIn(PEFilesInDir(CORKAMI_PE_PATH)));

} // namespace peparse
#endif
