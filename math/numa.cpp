#include "numa.h"

namespace knylaw {
namespace math {


void CubicSpline::initialize() {

    std::vector<Float> h(count()), alpha(count()), beta(count());
    // compute h_i
    for (size_t i = 0; i < N(); i++) {
        h[i] = x(i + 1) - x(i);
    }

    // compute alpha_i and beta_i
    alpha[0] = 1;		
    beta[0] = 3 / h[0] * (y(1) - y(0));
    alpha[N()] = 0;		
    beta[N()] = 3 / h[N() - 1] * (y(N()) - y(N() - 1));

    for (size_t i = 1; i < N(); i++) {
        alpha[i] = h[i - 1] / (h[i - 1] + h[i]);
        beta[i] = 3 * ((1 - alpha[i]) / h[i-1] * (y(i) - y(i-1)) + alpha[i] / h[i] * (y(i+1) - y(i)));
    }

    // compute a_i, b_i
    std::vector<Float> a(count()), b(count());
    a[0] = -alpha[0] / 2;
    b[0] = beta[0] / 2;
    for (size_t i = 1; i < count(); i++) {
        a[i] = -alpha[i] / (2 + (1 - alpha[i]) * a[i - 1]);
        b[i] = (beta[i] - (1 - alpha[i]) * b[i-1]) / (2 + (1 - alpha[i])*a[i-1]);
    }

    std::vector<Float> m(count());
    m[N()] = b[N()];
    for (int i = N() - 1; i >= 0; i--) {
        m[i] = m[i + 1] * a[i] + b[i];
    }
    this->Mm = m;
}

}
}


