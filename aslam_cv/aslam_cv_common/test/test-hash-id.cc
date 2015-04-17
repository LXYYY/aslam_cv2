#include <algorithm>
#include <unordered_set>

#include <gtest/gtest.h>

#include <aslam/common/entrypoint.h>
#include <aslam/common/hash_id.h>

using namespace aslam;

TEST(HashIdTest, Different) {
  HashId a(HashId::random()), b(HashId::random());
  EXPECT_NE(a, b);
}

TEST(HashIdTest, Validity) {
  HashId a, b;
  EXPECT_FALSE(a.isValid());
  EXPECT_FALSE(b.isValid());
  EXPECT_EQ(a,b);
  a.randomize();
  EXPECT_TRUE(a.isValid());
}

TEST(HashIdTest, String) {
  HashId a(HashId::random()), b(HashId::random());
  std::string as(a.hexString()), bs(b.hexString());
  EXPECT_NE(as, bs);
  EXPECT_EQ(as.length(), 32);
  EXPECT_EQ(bs.length(), 32);
}

TEST(HashIdTest, hashId_deserialize) {
  HashId a;
  std::string as(a.hexString());
  HashId b;
  EXPECT_TRUE(b.fromHexString(as));
  EXPECT_EQ(a, b);
}

TEST(HashIdTest, hashId_stdHash) {
  std::unordered_set<HashId> hashes;
  HashId needle(HashId::random());
  hashes.insert(needle);
  hashes.insert(HashId::random());
  std::unordered_set<HashId>::iterator found = hashes.find(needle);
  EXPECT_TRUE(found != hashes.end());
  EXPECT_EQ(*found, needle);
}

ASLAM_UNITTEST_ENTRYPOINT
