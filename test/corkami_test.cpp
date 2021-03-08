#include <catch2/catch.hpp>
#include <string>
#include <unordered_set>

#include <pe-parse/parse.h>

#include "utils.h"

// Whether the corkami testset has been downloaded
// Path to corkami PEs
#if defined(CORKAMI_PE_PATH)

namespace peparse {

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

TEST_CASE("Corkami PEs smoketest", "[corkami]") {
  for (auto path : PEFilesInDir(CORKAMI_PE_PATH)) {
    auto pe_name = path.filename().string();
    DYNAMIC_SECTION(pe_name) {
      auto *p = ParsePEFromFile(path.c_str());

      if (kKnownPEFailure.count(pe_name)) {
        CHECKED_ELSE(p == nullptr) {
          FAIL("Previously failing test now passes! Remove from set");
        }
      } else {
        CHECKED_ELSE(p != nullptr) {
          FAIL(GetPEErrString() + " at " + GetPEErrLoc());
        }
        DestructParsedPE(p);
      }
    }
  }
}

} // namespace peparse
#endif
