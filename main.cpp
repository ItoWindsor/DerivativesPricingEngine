#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <string>
#include "core/MarketData.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "enums/TreeModel.hpp"
#include "instruments/equity/CallOption.hpp"
#include "models/BlackScholeModel.hpp"


void print_ascii_tree(const Eigen::MatrixXd& tree) {
    const auto n_steps = static_cast<int>(tree.cols() - 1);
    
    for (int j = 0; j <= n_steps; ++j) { // Time steps
        // Add spacing to center the tree (explicit cast to size_t for string repetition)
        const auto spacing = static_cast<size_t>(2 * (n_steps - j));
        std::cout << std::string(spacing, ' ');
        
        // Print nodes
        for (int i = 0; i <= j; ++i) {
            std::cout << std::fixed << std::setprecision(2) << tree(i, j);
            if (i < j) std::cout << " —— ";
        }
        std::cout << "\n";
        
        // Draw connecting lines (if not last step)
        if (j < n_steps) {
            const auto line_spacing = static_cast<size_t>(2 * (n_steps - j) - 1);
            std::cout << std::string(line_spacing, ' ');
            
            for (int i = 0; i <= j; ++i) {
                std::cout << "/ \\ ";
            }
            std::cout << "\n";
        }
    }
}

int main(){

  int n_steps = 3;
  TreeModel tree_model = TreeModel::CoxRossRubinstein;
  BinomialTreeEngine engine = BinomialTreeEngine(n_steps,tree_model);
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 100;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;
  ExerciseKind exercise_kind = ExerciseKind::European;

  bs_model->set_r(r);
  bs_model->set_sigma(sigma);
  bs_model->set_spot(S0);
  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  CallOption call = CallOption(t,t,T,K,exercise_kind, false);
  auto payoff_option = call.get_payoff();

  Eigen::MatrixXd tree = engine.generate_tree_mat(T);
  std::cout << "tree : " << std::endl;
  std::cout << tree << std::endl;
   
  auto [u,d] = engine.generate_up_down_tree(T);
  std::cout << "u : " << u << std::endl; 
  std::cout << "d : " << d << std::endl;

  auto [pu,pd] = engine.generate_proba_tree(T);
  Eigen::MatrixXd option_mat = Eigen::MatrixXd::Zero(engine.get_n_steps()+1, engine.get_n_steps()+1);

  for(int j = engine.get_n_steps(); j >= 0; --j){
    for(int i = 0; i <= j; ++i){

      if (j == engine.get_n_steps()){
        option_mat(i,j) = payoff_option(tree(i,j), K);
      }

      else{
         option_mat(i,j) = (pu * option_mat(i, j + 1) + pd * option_mat(i + 1, j + 1));
      }
    }
  }

  std::cout << option_mat << std::endl;
  std::cout << option_mat(0,0) << std::endl;
  //std::chrono::sys_days start_date = std::chrono::year{2025} / std::chrono::month{1} / std::chrono::day{1}; 
  //std::chrono::sys_days valuation_date = start_date;
  //std::chrono::sys_days maturity_date = std::chrono::year{2025} / std::chrono::month{5} / std::chrono::day{12};
  //CompoundingFrequency compounding_frequency = CompoundingFrequency::monthly;
  //double nominal = 100;
  //double coupon_rate = 0.02;
  //double spread = 0.0;
  //
  //Bond bond(
  //start_date,
  //valuation_date,
  //maturity_date,
  //nominal,
  //coupon_rate,
  //spread,
  //DayCountConvention::Actual365,           
  //CompoundingMethod::actuarial,            
  //compounding_frequency,           
  //ShortSchedule::ShortEnd                 
  //);

  //std::cout << bond << std::endl;

  
}
