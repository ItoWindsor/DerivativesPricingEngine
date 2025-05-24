#include <cmath>
#include "structs/BasisFunctions.hpp"

BasisFunctions::BasisFunctions(const std::vector<std::function<double(double)>>& functions)
    : funcs(functions) {}

void BasisFunctions::add(const std::function<double(double)>& f) {
    funcs.push_back(f);
}

Eigen::MatrixXd BasisFunctions::evaluate(const Eigen::VectorXd& x) const {
    Eigen::Index n = x.size();
    Eigen::Index m = static_cast<Eigen::Index>(funcs.size());

    Eigen::MatrixXd mat(n, m);
    for (Eigen::Index i = 0; i < n; ++i) {
        for (Eigen::Index j = 0; j < m; ++j) {
            mat(i, j) = funcs[static_cast<size_t>(j)](x[i]);
        }
    }
    return mat;
}

size_t BasisFunctions::size() const {
    return funcs.size();
}


BasisFunctions make_polynomial_basis(int degree) {
    BasisFunctions basis;
    for (int d = 0; d <= degree; ++d) {
        basis.add([d](double x) { return std::pow(x, d); });
    }
    return basis;
}

std::function<double(double)> laguerre_polynomial(int n) {
    return [n](double x) {
        if (n == 0) return 1.0;
        if (n == 1) return 1.0 - x;

        double Lnm2 = 1.0;
        double Lnm1 = 1.0 - x;
        double Ln = 0.0;

        for (int k = 2; k <= n; ++k) {
            Ln = ((2 * k - 1 - x) * Lnm1 - (k - 1) * Lnm2) / k;
            Lnm2 = Lnm1;
            Lnm1 = Ln;
        }
        return Ln;
    };
}

BasisFunctions make_laguerre_basis(int degree) {
    BasisFunctions basis;
    for (int n = 0; n <= degree; ++n) {
        basis.add(laguerre_polynomial(n));
    }
    return basis;
}
