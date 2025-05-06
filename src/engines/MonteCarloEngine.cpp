#include "engines/MonteCarloEngine.hpp"
#include "structs/OptionPriceResult.hpp"
#include "enums/DayCountConvention.hpp"
#include "enums/ModelNames.hpp"
#include "utils/DayCount.hpp"
#include "utils/DayCount.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/Simulation.hpp"

MonteCarloEngine::MonteCarloEngine(int n_sim, int n_step)
: n_simulation(n_sim), n_steps(n_step) {}

int MonteCarloEngine::get_n_sim() const{
  return this->n_simulation;
}

OptionPriceResult MonteCarloEngine::compute_price(const Option& option) const{
  switch (this->model->get_model_name()){
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double S = bs->get_spot();
      double sigma = bs->get_sigma();
      double r = bs->get_interest_rate();
      int n_sim = this->get_n_sim();
      double K = option.get_strike(); 
      
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

      if (option.get_path_dependent() == true){
        return OptionPriceResult(0.0);
      }
      else{
        auto vec_asset = simulate_geometrical_brownian_terminal_distribution(S,tau, r, sigma, n_sim);     
        auto payoff = option.get_payoff();
        
        Eigen::VectorXd realised_payoff = Eigen::VectorXd::Zero(vec_asset.size());
        
        for(auto i = 0; i < realised_payoff.size(); ++i){
          realised_payoff[i] = std::exp(-r * tau) * payoff(vec_asset[i], K);
        }

        double price = realised_payoff.mean();
        std::tuple<double,double> confidence_interval = compute_confidence_interval(realised_payoff);
        return OptionPriceResult(price, confidence_interval);
      }
         
    }
    case ModelName::Heston : {
      return OptionPriceResult(0.0);
    }
  }
}
