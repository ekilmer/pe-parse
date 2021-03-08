#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

// Return a vector of all PE files immediately under `dir`
static std::vector<fs::path> PEFilesInDir(const fs::path &dir) {
  std::vector<fs::path> all_entries;
  if (!fs::exists(dir)) {
    return all_entries;
  }

  for (const auto &entry : fs::directory_iterator(dir)) {
    if ((entry.path().extension() == ".exe" ||
         entry.path().extension() == ".dll" ||
         entry.path().extension() == ".sys") &&
        fs::is_regular_file(entry)) {
      all_entries.emplace_back(entry.path());
    }
  }
  return all_entries;
}
