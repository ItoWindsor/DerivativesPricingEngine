#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <string>
#include "core/MarketData.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "engines/MonteCarloEngine.hpp"
#include "enums/TreeModel.hpp"
#include "utils/UsualPayoffs.hpp"
#include "models/BlackScholeModel.hpp"
#include "structs/BasisFunctions.hpp"
#include "rest_api/server.hpp"
#include "crow.h"

int main(){

  crow::SimpleApp app; 

  CROW_ROUTE(app, "/")([](){
          return "Hello world";
      });

  app.bindaddr("0.0.0.0").port(8080).multithreaded().run();

}
