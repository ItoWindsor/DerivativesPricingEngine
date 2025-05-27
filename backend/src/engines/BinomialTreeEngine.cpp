#include "engines/BinomialTreeEngine.hpp"
#include "core/UnderlyingModel.hpp"
#include "enums/ExerciseConvention.hpp"
#include "enums/ModelNames.hpp"
#include "utils/DayCount.hpp"
#include "models/BlackScholeModel.hpp"
#include "enums/TreeModel.hpp"
#include <memory>
#include <stdexcept>
#include <tuple>

BinomialTreeEngine::BinomialTreeEngine(std::shared_ptr<MarketData> market_data)
  : PricingEngine(market_data) {}

BinomialTreeEngine::BinomialTreeEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model)
  : PricingEngine(market_data,underlying_model) {}

BinomialTreeEngine::BinomialTreeEngine(int n_step, TreeModel tree_model)
: n_steps(n_step), tree_model(tree_model) {}

int BinomialTreeEngine::get_n_steps() const{
  return this->n_steps;
}

TreeModel BinomialTreeEngine::get_tree_model() const{
  return this->tree_model;
}

std::tuple<double,double> BinomialTreeEngine::generate_proba_tree(double T) const{
  
  double pu = 0.0;
  double pd = 0.0;
  double step = T/(this->get_n_steps());
  
  switch (this->model->get_model_name()){
    
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      switch (this->get_tree_model()) {
        case TreeModel::CoxRossRubinstein : {

          auto [u,d] = this->generate_up_down_tree(T);
          double r = bs->get_interest_rate();

          pu = (std::exp(r*step) - d)/(u-d);
          pd = 1.0 - pu;
          return std::make_tuple(pu,pd);
        }
        case TreeModel::JarrowRudd : {
          pu = 0.5;
          pd = 0.5;
          return std::make_tuple(pu,pd);
        }
      default:
        throw std::runtime_error("Unexpected three model");
      }
    }
    
    case ModelName::Heston : {
      return std::make_tuple(pu,pd);
    }
    default:
      throw std::runtime_error("Unexpected underlying model");
  }

}

std::tuple<double,double> BinomialTreeEngine::generate_up_down_tree(double T) const{
  double u = 0.0;
  double d = 0.0;
  double step = T/(this->get_n_steps());

  switch(this->model->get_model_name()){

    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double sigma = bs->get_sigma();
      switch (this->get_tree_model()){

        case TreeModel::CoxRossRubinstein : {
          u = std::exp(sigma * std::sqrt(step));
          d = 1/u;
          return std::make_tuple(u,d);
        }

        case TreeModel::JarrowRudd : {
          double r = bs->get_interest_rate();
          // double q = ... TODO
          u = std::exp( (r - 0.5*sigma*sigma)*step + sigma*std::sqrt(step) );
          d = std::exp( (r - 0.5*sigma*sigma)*step - sigma*std::sqrt(step) );
          return std::make_tuple(u,d);
        }
        default:
          throw std::runtime_error("Unexpected tree model");
      }

    }
    case ModelName::Heston : {
        return std::make_tuple(u,d);
      }
    default:
      throw std::runtime_error("Unexpected underlying model");
  }
}

Eigen::MatrixXd BinomialTreeEngine::generate_tree_mat(double T) const{

  switch(this->model->get_model_name()){
    
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      
      std::tuple<double,double> movements = this->generate_up_down_tree(T);
      double u = std::get<0>(movements);
      double d = std::get<1>(movements);
      double S0 = bs->get_spot();
      
      Eigen::MatrixXd mat_movements = Eigen::MatrixXd::Zero(this->n_steps + 1, this->n_steps + 1);
      
      for(int j = 0; j < this->n_steps +1 ; ++j) {
        for(int i = 0; i < j+1 ; ++i){
          mat_movements(i,j) = S0*std::pow(u,j-i)*std::pow(d,i);
        }
      }
      return mat_movements;
    }
    case ModelName::Heston : {
      return Eigen::MatrixXd::Zero(this->n_steps + 1, this->n_steps + 1); 
    }
    default:
      throw std::runtime_error("Unexpected underlying model");
  }
}

double BinomialTreeEngine::compute_price(const Option& option) const{
  double price = 0.0;
  auto payoff_option = option.get_payoff();

  switch (this->model->get_model_name()){
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double K = option.get_strike();
      double r = bs->get_interest_rate();
      
      std::variant<std::chrono::sys_days, double> valuation_date = option.get_valuation_date();
      std::variant<std::chrono::sys_days, double> maturity_date = option.get_maturity_date();
      double tau = 0.0;
      
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
      
      if (tau <= 0.0) {
          throw std::runtime_error("Invalid time to maturity (tau <= 0.0)");
      }

      double step = tau/this->get_n_steps();
      double df = std::exp(- r * step);

      Eigen::MatrixXd stock_mat = this->generate_tree_mat(tau);
      auto [pu,pd] = this->generate_proba_tree(tau);
      Eigen::MatrixXd option_mat = Eigen::MatrixXd::Zero(this->n_steps+1, this->n_steps+1);
      assert(option_mat.rows() > 0 && option_mat.cols() > 0);

      for(int j = this->n_steps; j >= 0; --j){
        for(int i = 0; i <= j; ++i){

          if (j == this->n_steps){
            option_mat(i,j) = payoff_option(stock_mat(i,j), K);
          }

          else{
            double continuation_value = df * (pu * option_mat(i, j + 1) + pd * option_mat(i + 1, j + 1));

            switch(option.get_exercise_kind()){

              case ExerciseKind::American : {
                option_mat(i,j) = std::max(continuation_value, payoff_option(stock_mat(i,j), K));
                break;
              }

              case ExerciseKind::European : {
                option_mat(i,j) = continuation_value;
                break;
              }

            }

          }
        }
      }
      price = option_mat(0,0);
      break;
    }
    case ModelName::Heston : {
      break;
    }
  }
  return price;
} 

