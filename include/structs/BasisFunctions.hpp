#ifndef BASISFUNCTIONS_HPP
#define BASISFUNCTIONS_HPP


#include <functional>
#include <vector>
#include <Eigen/Dense>

struct BasisFunctions {
    std::vector<std::function<double(double)>> funcs;

    BasisFunctions() = default;
    BasisFunctions(const std::vector<std::function<double(double)>>& functions);

    void add(const std::function<double(double)>& f);
    Eigen::MatrixXd evaluate(const Eigen::VectorXd& x) const;
    size_t size() const;
};

BasisFunctions make_polynomial_basis(int degree);
BasisFunctions make_laguerre_basis(int degree);

#endif
