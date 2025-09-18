#include "gtest/gtest.h"
#include "returns_analyser.h"
#include "returns_helper.h"
#include "returns_processor.h"
#include <string>
#include <vector>
#include <memory>

// We use std::move(helper) in these tests because returns_analyser takes ownership of the
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
    std::string expected_returns = "0.1, 0.3";
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.1, 0.2, 0.3, 0.2 }, std::move(helper));
    analyser.erase_values(0.2);
    EXPECT_EQ(analyser.get_returns(), expected_returns);
}

TEST(ReturnsAnalyser, TestEraseValueThatDoesNotExist) {
    std::string expected_returns = "0.1, 0.2, 0.3";
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.1, 0.2, 0.3 }, std::move(helper));
    analyser.erase_values(0.5);
    EXPECT_EQ(analyser.get_returns(), expected_returns);
}

TEST(ReturnsAnalyser, TestGetMaxAllWithAllNegativeReturns) {
    double expected_max = -0.2;
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ -0.5, -0.2, -1.0 }, std::move(helper));
    EXPECT_DOUBLE_EQ(analyser.get_max(), expected_max);
}

TEST(ReturnsAnalyser, TestGetMaxAllPositiveReturns) {
    double expected_max = 0.3;
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 0.2, 0.3, 0.1 }, std::move(helper));
    EXPECT_DOUBLE_EQ(analyser.get_max(), expected_max);
}

TEST(ReturnsAnalyser, TestGetMean) {
    double expected_mean = 2.0;
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 3.0 }, std::move(helper));
    EXPECT_DOUBLE_EQ(analyser.get_mean(), expected_mean);
}

TEST(ReturnsAnalyser, TestGetMeanWithNoReturns) {
    double expected_mean = 0.0;
    // pass nullptr helper to avoid allocating helper when not needed
    returns_analyser analyser({}, nullptr);
    EXPECT_DOUBLE_EQ(analyser.get_mean(), expected_mean);
}

TEST(ReturnsAnalyser, TestReturnsOptimal) {
    bool expected_is_optimal = true;
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 3.0 }, std::move(helper));
    EXPECT_EQ(analyser.returns_optimal(), expected_is_optimal);
}

TEST(ReturnsAnalyser, TestReturnsNotOptimal) {
    bool expected_is_optimal = false;
    auto helper = std::make_unique<returns_helper>();
    returns_analyser analyser({ 1.0, 2.0, 0.1 }, std::move(helper));
    EXPECT_EQ(analyser.returns_optimal(), expected_is_optimal);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
