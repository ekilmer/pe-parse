#include <gtest/gtest.h>
#include <pe-parse/parse.h>

#include <filesystem>
#include <string>
#include <unordered_set>

#include "utils.h"

namespace fs = std::filesystem;

// Whether the corkami testset has been downloaded
// Path to corkami PEs
#if defined(CORKAMI_PE_PATH)
namespace peparse {

class CorkamiTestSuite : public testing::TestWithParam<fs::path> {};

static const std::unordered_set<std::string> kKnownPEFailure{
    "virtsectblXP.exe", "maxsec_lowaligW7.exe",
    "maxsecXP.exe",     "nullSOH-XP.exe",
    "tinyXP.exe",       "tinydllXP.dll",
    "virtrelocXP.exe",  "foldedhdrW7.exe",
    "maxvals.exe",      "d_nonnull.dll",
    "reloccrypt.exe",   "d_resource.dll",
    "fakerelocs.exe",   "lfanew_relocW7.exe",
    "bigSoRD.exe",      "tinyW7.exe",
    "reloccryptW8.exe", "standard.exe",
    "exe2pe.exe",       "tinygui.exe",
    "dllfwloop.dll",    "tinydrivXP.sys",
    "tiny.exe",         "tinydll.dll",
    "foldedhdr.exe",    "dllmaxvals.dll",
    "reloccryptXP.exe", "dosZMXP.exe",
    "tinyW7_3264.exe",  "dllfw.dll",
    "hdrcode.exe",      "ibrelocW7.exe",
    "d_tiny.dll",       "sc.exe"};

TEST_P(CorkamiTestSuite, Corkami) {
  auto path = GetParam();
  auto pe_name = path.filename().string();

  auto *p = ParsePEFromFile(path.c_str());

  if (kKnownPEFailure.count(pe_name)) {
    // Known failure
    EXPECT_EQ(p, nullptr)
        << "Previously failing test now passes! Remove from set";
  } else {
    EXPECT_NE(p, nullptr) << GetPEErrString() << " at " << GetPEErrLoc();
    DestructParsedPE(p);
  }
}

INSTANTIATE_TEST_SUITE_P(CorkamiTests,
                         CorkamiTestSuite,
                         testing::ValuesIn(PEFilesInDir(CORKAMI_PE_PATH)));

} // namespace peparse
#endif
