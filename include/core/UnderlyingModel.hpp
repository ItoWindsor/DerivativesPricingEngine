#ifndef UNDERLYINGMODEL_HPP
#define UNDERLYINGMODEL_HPP

#include "core/MarketData.hpp"
#include "enums/ModelNames.hpp"

class UnderlyingModel {
  public:
    UnderlyingModel(ModelName model_name);
    virtual ~UnderlyingModel() = default;
    virtual void fit(const MarketData& market_data) = 0;
    ModelName get_model_name() const;
  private:
    ModelName model_name;
};

#endif
