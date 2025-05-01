#include <gtest/gtest.h>
#include <chrono>
#include "core/InterestRateCurve.hpp"
#include "utils/CurveInterpolation.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/DayCount.hpp"
#include "utils/TimeSchedule.hpp"
#include "curves/FlatCurve.hpp"

TEST(InterestRateCurveTest, CheckLoadInterestRateCurveFromCSV) {
    std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/flat_curve.csv";
    std::chrono::sys_days valuation_date = std::chrono::year{2025} / 1 / 1;

    InterestRateCurve curve(filepath, valuation_date);

    auto date_variant = curve.get_valuation_date();
    ASSERT_TRUE(std::holds_alternative<std::chrono::sys_days>(date_variant));
    ASSERT_EQ(std::get<std::chrono::sys_days>(date_variant), valuation_date);

    auto data_variant = curve.get_curve_data();
    ASSERT_TRUE(std::holds_alternative<std::vector<TupleDateDouble>>(data_variant));

    const auto& data = std::get<std::vector<TupleDateDouble>>(data_variant);
    double expected_rate = 0.03;

    for (const auto& entry : data) {
        double rate = std::get<1>(entry);
        ASSERT_DOUBLE_EQ(rate, expected_rate);
    }
}

TEST(InterpolationTest, LinearInterpolation) {
    std::vector<std::tuple<double, double>> curve = {
        {0.0, 0.02}, {1.0, 0.04}, {2.0, 0.06}
    };
    std::vector<double> query = {0.5, 1.5, 2.0};

    auto interpolated = interpolate_rate_curve(curve, query);
    ASSERT_EQ(interpolated.size(), query.size());

    std::vector<double> expected = {0.03, 0.05, 0.06};
    for (size_t i = 0; i < query.size(); ++i) {
        ASSERT_NEAR(std::get<1>(interpolated[i]), expected[i], 1e-6);
    }
}

TEST(InterpolationTest, FlatExtrapolationBeforeFirst) {
    std::vector<std::tuple<double, double>> curve = {
        {1.0, 0.03}, {2.0, 0.05}
    };
    std::vector<double> query = {0.5};

    auto interpolated = interpolate_rate_curve(curve, query);
    ASSERT_NEAR(std::get<1>(interpolated[0]), 0.03, 1e-6);
}

TEST(InterpolationTest, FlatExtrapolationAfterLast) {
    std::vector<std::tuple<double, double>> curve = {
        {1.0, 0.03}, {2.0, 0.05}
    };
    std::vector<double> query = {3.0};

    auto interpolated = interpolate_rate_curve(curve, query);
    ASSERT_NEAR(std::get<1>(interpolated[0]), 0.05, 1e-10);
}

TEST(InterestRateCurveTest, InterpolationOnCurveExample) {
    std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/curve_example.csv";
    std::chrono::sys_days valuation_date = std::chrono::year{2024} / 5 / 10;

    InterestRateCurve curve(filepath, valuation_date);
    auto data_variant = curve.get_curve_data();
    ASSERT_TRUE(std::holds_alternative<std::vector<TupleDateDouble>>(data_variant));

    const auto& curve_data = std::get<std::vector<TupleDateDouble>>(data_variant);
    auto curve_time_rate = compute_year_fraction(valuation_date, curve_data, DayCountConvention::Actual360);

    std::vector<std::chrono::sys_days> test_dates = {
        std::chrono::year{2024} / 7 / 10,
        std::chrono::year{2024} / 12 / 10,
        std::chrono::year{2029} / 1 / 1
    };

    auto test_times = compute_year_fraction(valuation_date, test_dates, DayCountConvention::Actual360);
    auto interpolated = interpolate_rate_curve(curve_time_rate, test_times);

    std::vector<double> expected_rates = {0.0420294, 0.0233696, 0.0275507};
    for (size_t i = 0; i < expected_rates.size(); ++i) {
        ASSERT_NEAR(std::get<1>(interpolated[i]), expected_rates[i], 1e-5);
    }
}

TEST(FlatCurveTest, ConstructionStoresCorrectRateAndDate) {
    auto valuation_date = make_date(2025, 4, 30);
    auto maturity_date = make_date(2030, 4, 30);
    double rate = 0.03;

    FlatCurve curve(valuation_date, maturity_date, rate);
    auto data_variant = curve.get_curve_data();

    ASSERT_TRUE(std::holds_alternative<std::vector<TupleDateDouble>>(data_variant));
    const auto& data = std::get<std::vector<TupleDateDouble>>(data_variant);

    ASSERT_EQ(data.size(), 1);
    EXPECT_EQ(std::get<0>(data[0]), maturity_date);
    EXPECT_DOUBLE_EQ(std::get<1>(data[0]), rate);
}

TEST(FlatCurveTest, InterpolationReturnsFlatRate) {
    std::chrono::sys_days valuation_date = make_date(2025, 4, 30);
    std::chrono::sys_days maturity_date = make_date(2030, 4, 30);
    double rate = 0.05;

    FlatCurve curve(valuation_date, maturity_date, rate);
    std::vector<double> time_points = {0.5, 1.0, 2.5, 5.0};
    auto interpolated = curve.interpolate_curve(time_points);

    ASSERT_EQ(interpolated.size(), time_points.size());
    for (std::size_t i = 0; i < time_points.size(); ++i) {
        EXPECT_DOUBLE_EQ(std::get<0>(interpolated[i]), time_points[i]);
        EXPECT_DOUBLE_EQ(std::get<1>(interpolated[i]), rate);
    }
}
