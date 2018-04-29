#pragma once
#include "common.h"

#include <algorithm>
#include <cassert>

namespace knylaw {
namespace math {


    class NewtonPoly: public impl::NumaDataxy<Point2d> {

    public:

        NewtonPoly(const Container& container) : NumaDataxy(container), Mdiffs() {

            for (size_t i = 0; i < count(); i++) {
                Mdiffs.push_back(Y(i));
            }
        
            for (size_t d = 1; d < count(); d++) { // compute the differences of degree d+1
                for (size_t i = count() - 1; i >= d; i--) {
                    Mdiffs[i] = (Mdiffs[i] - Mdiffs[i - 1]) / (X(i) - X(i - d));
                }
            }
        }

        //inline void initialize() {

        //    for (size_t i = 0; i < count(); i++) {
        //        Mdiffs.push_back(Y(i));
        //    }

        //    for (size_t d = 1; d < count(); d++) { // compute the differences of degree d+1
        //        for (size_t i = count() - 1; i >= d; i--) {
        //            Mdiffs[i] = (Mdiffs[i] - Mdiffs[i - 1]) / (X(i) - X(i - d));
        //        }
        //    }
        //}

        inline Numeric input(Numeric xx) const {

            Numeric w = 1, ret = Mdiffs[0];
            for (size_t i = 1; i < count(); i++) {
                w *= (xx - X(i - 1));
                ret += Mdiffs[i] * w;
            }
            return ret;
        }

    private:
        std::vector<Numeric> Mdiffs;

    };

    class LagrangePoly : public impl::NumaDataxy<Point2d> {

    public:

        // O(N^2)
        LagrangePoly(const Container& container): NumaDataxy(container), Mweights() {

            for (size_t i = 0; i < count(); ++i) {
                Numeric w = 1.0;
                for (size_t j = 0; j < count(); ++j) {
                    if (j != i) {
                        w *= X(i) - X(j);
                    }
                }
                w = 1.0 / w;
                Mweights.push_back(w);
            }
        }

        // O(N^2)
        inline Numeric input(Numeric xx) const {
            // Not optimized
            Numeric ret = 0;
            for (size_t i = 0; i < count(); i++) {
                auto w = Mweights[i] * Y(i);
                for (size_t j = 0; j < count(); j++) {
                    if (j != i) {
                        w *= xx - X(j);
                    }
                }
                ret += w;
            }
            return ret;
        }

    private:
        std::vector<Numeric> Mweights;

    };

    namespace impl {

        template<typename Real>
        class Point2dderiv:
            virtual public Point2d<Real> {

        public:

            using Numeric = typename Point2d<Real>::Numeric;

            Point2dderiv() = default;
            
            ~Point2dderiv() = default;

            Point2dderiv(Numeric x_, Numeric y_, Numeric dy_) : Point2d<Numeric>(x_, y_), Mdy(dy_) {

            };

            inline Numeric getdy() const {
                return Mdy;
            }

        private:

            Numeric Mdy;

        };

        template<typename Coordd>
        class NumaDataxydy: public NumaDataxy<Coordd> {

        public:

            using Numeric   = typename NumaDataxy<Coordd>::Numeric;
            using Container = typename NumaDataxy<Coordd>::Container;

        public:
            //
            // template argument list is needed on calling constructor of super class
            //
            NumaDataxydy(const Container& container) : NumaDataxy<Coordd>(container) {

            }

            inline Numeric DY(size_t pos) const {
                return this->data(pos).getdy();
            }
        };


    }

    using Point2dderiv = impl::Point2dderiv<double>;

    class SectionedCubicHermite: 
        public impl::NumaDataxydy<Point2dderiv> {
    public:

        SectionedCubicHermite(const Container& container) : NumaDataxydy(container) {

        }

        inline Numeric input(Numeric xx) const {
            auto ite = std::lower_bound(data().cbegin(), data().cend(), xx, compare());
            auto d = ite - data().cbegin();
            
            assert(d > 0);
            assert(d < count());

            Numeric yy = 0;
            auto i = d - 1;
            yy += (1 + 2 * (xx - X(i)) / (X(i+1) - X(i))) * std::pow((xx-X(i+1)) / (X(i)-X(i+1)), 2) * Y(i);
            yy += (1 + 2 * (xx - X(i+1)) / (X(i) - X(i+1))) * std::pow((xx-X(i)) / (X(i+1)-X(i)), 2) * Y(i+1);
            yy += (xx - X(i)) * std::pow((xx - X(i+1)) / (X(i) - X(i+1)),2) * DY(i);
            yy += (xx - X(i+1)) * std::pow((xx - X(i)) / (X(i+1) - X(i)),2) * DY(i+1);

            return yy;
        }

    private:

        struct compare {
            inline bool operator()(const Point2dderiv& pd, Numeric xx) {
                return pd.getx() < xx;
            }
        };

    };

}
}