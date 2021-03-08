#include <pe-parse/parse.h>

#include <catch2/catch.hpp>

#include "utils.h"

// Whether the corkami testset has been downloaded
// Path to corkami PEs
#if defined(CORKAMI_PE_PATH)

namespace peparse {

TEST_CASE("Corkami PEs smoketest", "[corkami]") {
  for (auto path : PEFilesInDir(CORKAMI_PE_PATH)) {
    DYNAMIC_SECTION(path.filename()) {
      auto *p = ParsePEFromFile(path.c_str());
      if (p == nullptr) {
        FAIL(GetPEErrString() + " at " + GetPEErrLoc());
      }
      DestructParsedPE(p);
      SUCCEED();
    }
  }
  WARN("Could not find Corkami dataset");
}

} // namespace peparse
#endif
