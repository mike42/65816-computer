#include "gtest/gtest.h"

#include "DebugSymbols.h"

TEST(DebugSymbols, CanParseFile) {
    auto symbols = DebugSymbols();
    auto testRes = boost::filesystem::path(TEST_RESOURCES_PATH);
    auto testImage = testRes.append("testing_example/test.bin");
    symbols.loadSymbolsForBinary(testImage);
    ASSERT_EQ(3, symbols.labels.size());
    ASSERT_TRUE(symbols.labels.contains("test_this_will_pass"));
    ASSERT_EQ(0xe001, symbols.labels["test_this_will_pass"].getOffset());
}
