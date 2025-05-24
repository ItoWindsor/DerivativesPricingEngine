#include "engines/MonteCarloEngine.hpp"
#include "enums/ExerciseConvention.hpp"
#include "structs/OptionPriceResult.hpp"
#include "enums/ModelNames.hpp"
#include "utils/DayCount.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/Simulation.hpp"

MonteCarloEngine::MonteCarloEngine(int n_sim, int n_step)
: n_simulation(n_sim), n_steps(n_step) {}

int MonteCarloEngine::get_n_sim() const{
  return this->n_simulation;
}

const BasisFunctions& MonteCarloEngine::get_basis_functions() const{
  return this->basis_functions;
}

int MonteCarloEngine::get_n_steps() const{
  return this->n_steps;
}

void MonteCarloEngine::set_basis_functions(const BasisFunctions& basis){
  this->basis_functions = basis;
}

OptionPriceResult MonteCarloEngine::compute_price(const Option& option) const {
  switch (this->model->get_model_name()) {
    case ModelName::BlackScholes: {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double S = bs->get_spot();
      double sigma = bs->get_sigma();
      double r = bs->get_interest_rate();
      int n_sim = this->get_n_sim();
      int n_time = this->get_n_steps();
      double K = option.get_strike(); 
      
      auto payoff_option = option.get_payoff();
      
      std::variant<std::chrono::sys_days, double> valuation_date = option.get_valuation_date();
      std::variant<std::chrono::sys_days, double> maturity_date = option.get_maturity_date();
      double tau;
      
      if (std::holds_alternative<std::chrono::sys_days>(valuation_date) &&
          std::holds_alternative<std::chrono::sys_days>(maturity_date)) {
        std::chrono::sys_days val = std::get<std::chrono::sys_days>(valuation_date);
        std::chrono::sys_days mat = std::get<std::chrono::sys_days>(maturity_date);
        tau = compute_year_fraction(val, mat, option.get_day_convention());
      }
      else if (std::holds_alternative<double>(valuation_date) &&
               std::holds_alternative<double>(maturity_date)) {
        double val = std::get<double>(valuation_date);
        double mat = std::get<double>(maturity_date);
        tau = compute_year_fraction(val, mat); 
      }
      else {
        throw std::runtime_error("Mismatched date types for valuation and maturity.");
      }

      if (option.get_path_dependent() || option.get_exercise_kind() == ExerciseKind::American) {
        
        auto mat_asset = simulate_geometrical_brownian_paths(S, tau, r, sigma, n_sim, n_time);
        Eigen::MatrixXd mat_option = Eigen::MatrixXd::Zero(mat_asset.rows(), mat_asset.cols());
        double time_step = tau / static_cast<double>(n_time);

        // Terminal payoffs
        for (int i = 0; i < n_sim; ++i) {
          mat_option(i, n_time) = payoff_option(mat_asset(i, n_time), K);
        }

        if (option.get_exercise_kind() == ExerciseKind::American) {
          BasisFunctions basis = this->get_basis_functions();

          for (int j = n_time - 1; j >= 1; --j) {
            std::vector<int> itm_indices;
            Eigen::VectorXd x(n_sim);
            Eigen::VectorXd y(n_sim);

            for (int i = 0; i < n_sim; ++i) {
              x[i] = mat_asset(i, j);
              y[i] = 0.0;

              // Discounted future cashflow
              for (int k = j + 1; k <= n_time; ++k) {
                if (mat_option(i, k) > 1e-8) {
                  y[i] = std::exp(-r * (k - j) * time_step) * mat_option(i, k);
                  break;
                }
              }

              if (payoff_option(x[i], K) > 1e-8) {
                itm_indices.push_back(i);
              }
            }

            if (!itm_indices.empty()) {
              Eigen::VectorXd x_itm(static_cast<Eigen::Index>(itm_indices.size()));
              Eigen::VectorXd y_itm(static_cast<Eigen::Index>(itm_indices.size()));

              for (Eigen::Index idx = 0; idx < static_cast<Eigen::Index>(itm_indices.size()); ++idx) {
                x_itm[idx] = x[itm_indices[static_cast<size_t>(idx)]];
                y_itm[idx] = y[itm_indices[static_cast<size_t>(idx)]];
              }

              Eigen::MatrixXd X = basis.evaluate(x_itm);
              Eigen::VectorXd coeffs = X.colPivHouseholderQr().solve(y_itm);
              Eigen::VectorXd continuation_value = X * coeffs;

              for (Eigen::Index idx = 0; idx < static_cast<Eigen::Index>(itm_indices.size()); ++idx) {
                int i = itm_indices[static_cast<size_t>(idx)];
                double exercise_val = payoff_option(x[i], K);
                if (exercise_val > continuation_value[idx]) {
                  mat_option(i, j) = exercise_val;
                  for (int k = j + 1; k <= n_time; ++k) {
                    mat_option(i, k) = 0.0;
                  }
                }
              }
            }
          }
        }

        Eigen::VectorXd discounted_cashflows(n_sim);
        for (int i = 0; i < n_sim; ++i) {
          discounted_cashflows[i] = 0.0;
          for (int j = 1; j <= n_time; ++j) {
            if (mat_option(i, j) > 1e-8) {
              discounted_cashflows[i] = mat_option(i, j) * std::exp(-r * j * time_step);
              break;
            }
          }
        }

        double price = discounted_cashflows.mean();
        auto conf_interval = compute_confidence_interval(discounted_cashflows);
        return OptionPriceResult(price, conf_interval);
      }
      else { 
        auto vec_asset = simulate_geometrical_brownian_terminal_distribution(S, tau, r, sigma, n_sim);     
        Eigen::VectorXd realised_payoff = Eigen::VectorXd::Zero(static_cast<Eigen::Index>(vec_asset.size()));

        for (Eigen::Index i = 0; i < realised_payoff.size(); ++i) {
          realised_payoff[i] = std::exp(-r * tau) * payoff_option(vec_asset[i], K);
        }

        double price = realised_payoff.mean();
        std::tuple<double, double> confidence_interval = compute_confidence_interval(realised_payoff);
        return OptionPriceResult(price, confidence_interval);
      }
    }
    case ModelName::Heston: {
      return OptionPriceResult(0.0);
    }
    default:
      throw std::runtime_error("Unknown model type");
  }
}
