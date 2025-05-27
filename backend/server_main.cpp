#include "crow.h"
#include <Eigen/Dense>
#include <iostream>
#include <string>

#include "core/MarketData.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "engines/MonteCarloEngine.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/UsualPayoffs.hpp"
#include "structs/BasisFunctions.hpp"
#include "enums/TreeModel.hpp"
#include "enums/ExerciseConvention.hpp"
#include "instruments/equity/Option.hpp"
#include "instruments/equity/CallOption.hpp"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/price_option").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json) return crow::response(400, "Invalid JSON");

        try {
            // Extract parameters
            std::string engine = json["engine"].s();  // "monte_carlo" | "binomial_tree"
            std::string kind = json["kind"].s();      // "call" | "put"
            std::string exercise = json["exercise_kind"].s(); // "european" | "american"
            double spot = json["spot"].d();
            double strike = json["strike"].d();
            double sigma = json["volatility"].d();
            double r = json["rate"].d();
            double t0 = 0.0; // Not passed for now
            double T = json["maturity"].d();

            ExerciseKind ex_kind = (exercise == "american") ? ExerciseKind::American : ExerciseKind::European;
            auto payoff_fn = (kind == "call") ? call_payoff : put_payoff;

            // Set up model
            auto model = std::make_shared<BlackScholesModel>();
            model->set_spot(spot);
            model->set_sigma(sigma);
            model->set_r(r);

            auto market_data = std::make_shared<MarketData>();
            Option option(t0, t0, T, strike, payoff_fn, ex_kind, false);

            // Pricing engine
            if (engine == "monte_carlo") {
                MonteCarloEngine mc(10000, 1000);  // Simulations, Steps
                mc.set_model(model);
                mc.set_market_data(market_data);
                mc.set_basis_functions(make_polynomial_basis(3));

                OptionPriceResult result = mc.compute_price(option);
                double price = result.price;

                crow::json::wvalue response;
                response["price"] = price;
                if (result.confidence_interval) {
                    auto [low, high] = *result.confidence_interval;
                    response["confidence_interval"]["low"] = low;
                    response["confidence_interval"]["high"] = high;
                }
                return crow::response(response);

            } else if (engine == "binomial_tree") {
                BinomialTreeEngine bt(1000, TreeModel::JarrowRudd);
                bt.set_model(model);
                bt.set_market_data(market_data);
                double price = bt.compute_price(option);

                crow::json::wvalue response;
                response["price"] = price;
                return crow::response(response);
            }

            return crow::response(400, "Unknown engine");

        } catch (const std::exception& e) {
            return crow::response(500, std::string("Exception: ") + e.what());
        }
    });

    std::cout << "Starting server on http://localhost:8080..." << std::endl;
    app.port(8080).multithreaded().run();
}
