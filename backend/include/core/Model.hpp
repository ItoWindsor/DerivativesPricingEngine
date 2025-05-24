#ifndef MODEL_HPP
#define MODEL_HPP

#include<string>

class Model {
  public:
    virtual ~Model() = default;
    std::string get_name();
  private:
    std::string name;
};

#endif
