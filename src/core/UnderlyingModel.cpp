#include "core/UnderlyingModel.hpp"
#include "enums/ModelNames.hpp"

UnderlyingModel::UnderlyingModel(ModelName model_name)
  : model_name(model_name) {}

ModelName UnderlyingModel::get_model_name() const{
  return this->model_name;
}
