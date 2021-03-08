#include <pe-parse/parse.h>

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace peparse;

TEST(SimpleTestPE, example) {
  auto path = fs::path(ASSETS_DIR) / "example.exe";
  auto *p = ParsePEFromFile(path.c_str());

  EXPECT_TRUE(p);

  auto dos = p->peHeader.dos;
  EXPECT_EQ(dos.e_magic, 0x5a4d);
  EXPECT_EQ(dos.e_cp, 0x3);
  EXPECT_EQ(dos.e_crlc, 0x0);
  EXPECT_EQ(dos.e_cparhdr, 0x4);
  EXPECT_EQ(dos.e_minalloc, 0x0);
  EXPECT_EQ(dos.e_maxalloc, 0xffff);
  EXPECT_EQ(dos.e_ss, 0x0);
  EXPECT_EQ(dos.e_sp, 0xb8);
  EXPECT_EQ(dos.e_csum, 0x0);
  EXPECT_EQ(dos.e_ip, 0x0);
  EXPECT_EQ(dos.e_cs, 0x0);
  EXPECT_EQ(dos.e_lfarlc, 0x40);
  EXPECT_EQ(dos.e_ovno, 0x0);
  EXPECT_EQ(dos.e_res[0], 0x0);
  EXPECT_EQ(dos.e_res[1], 0x0);
  EXPECT_EQ(dos.e_res[2], 0x0);
  EXPECT_EQ(dos.e_res[3], 0x0);
  EXPECT_EQ(dos.e_oemid, 0x0);
  EXPECT_EQ(dos.e_oeminfo, 0x0);
  EXPECT_EQ(dos.e_res2[0], 0x0);
  EXPECT_EQ(dos.e_res2[1], 0x0);
  EXPECT_EQ(dos.e_res2[2], 0x0);
  EXPECT_EQ(dos.e_res2[3], 0x0);
  EXPECT_EQ(dos.e_res2[4], 0x0);
  EXPECT_EQ(dos.e_res2[5], 0x0);
  EXPECT_EQ(dos.e_res2[6], 0x0);
  EXPECT_EQ(dos.e_res2[7], 0x0);
  EXPECT_EQ(dos.e_res2[8], 0x0);
  EXPECT_EQ(dos.e_res2[9], 0x0);
  EXPECT_EQ(dos.e_lfanew, 0xf8);

  DestructParsedPE(p);
}
