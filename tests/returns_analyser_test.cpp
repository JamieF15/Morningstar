#include "gtest/gtest.h"
#include "returns_analyser.h"
#include "returns_helper.h"
#include "returns_processor.h"
#include <string>
#include <vector>
#include <memory> // for std::make_unique

// We use std::move(helper) in theses testes because returns_analyser takes ownership of the 
// std::unique_ptr<returns_helper>. unique_ptr is non-copyable, so moving transfers 
// ownership from the caller to returns_analyser. After the move, the original pointer 
// no longer owns the object.

TEST(ReturnsAnalyser, TestGetReturns) {
    std::string expected_returns = "0.2, 0.3, 0.1";
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.2, 0.3, 0.1 }, std::move(helper));

    EXPECT_EQ(analyser.get_returns(), expected_returns);
}

TEST(ReturnsAnalyser, TestEraseValueThatExists) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.1, 0.2, 0.3, 0.2 }, std::move(helper));

    analyser.erase_values(0.2);

    // remaining order should be 0.1, 0.3
    EXPECT_EQ(analyser.get_returns(), "0.1, 0.3");
}

TEST(ReturnsAnalyser, TestEraseValueThatDoesNotExist) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.1, 0.2, 0.3 }, std::move(helper));

    analyser.erase_values(0.5); // value not present
    EXPECT_EQ(analyser.get_returns(), "0.1, 0.2, 0.3");
}

TEST(ReturnsAnalyser, TestGetMaxAllWithAllNegativeReturns) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ -0.5, -0.2, -1.0 }, std::move(helper));

    EXPECT_DOUBLE_EQ(analyser.get_max(), -0.2);
}

TEST(ReturnsAnalyser, TestGetMaxAllPositiveReturns) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.2, 0.3, 0.1 }, std::move(helper));

    EXPECT_DOUBLE_EQ(analyser.get_max(), 0.3);
}

TEST(ReturnsAnalyser, TestGetMean) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 3.0 }, std::move(helper));

    EXPECT_DOUBLE_EQ(analyser.get_mean(), 2.0);
}

TEST(ReturnsAnalyser, TestGetMeanWithNoReturns) {
    // pass nullptr helper to avoid allocating helper when not needed
    returns_analyser analyser({}, nullptr);

    EXPECT_DOUBLE_EQ(analyser.get_mean(), 0.0);
}

TEST(ReturnsAnalyser, TestReturnsOptimal) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 3.0 }, std::move(helper));

    EXPECT_EQ(analyser.returns_optimal(), true);
}

TEST(ReturnsAnalyser, TestReturnsNotOptimal) {
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 0.1 }, std::move(helper));

    EXPECT_EQ(analyser.returns_optimal(), false);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
