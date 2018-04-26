#pragma once
#include "types.h"

namespace knylaw {
namespace math {

    namespace impl {

        template<typename Input>
        class IntrplBaseType :
            virtual public NumaData<Input> {
        public:
            //
            // N+1 points in total due to interpolation points arranged from index 0 to N
            //
            inline size_t N() const {
                return this->count() - 1;
            }

        };

    }

    //
    // Newton polynomial and Lagrange polynormial can be applied on unordered input
    //

    class NewtonPoly :
        public impl::IntrplBaseType<Point2d>,
        public impl::Coord2dBaseType<Point2d> {

    public:

        using Container = NumaData::Container;

        inline void initialize() {
            for (size_t i = 0; i < count(); i++) {
                Mdiffs.push_back(Y(i));
            }

            for (size_t d = 1; d <= N(); d++) { // compute the differences of degree d+1
                for (size_t i = N(); i >= d; i--) {
                    Mdiffs[i] = (Mdiffs[i] - Mdiffs[i-1]) / (X(i) - X(i-d));
                }
            }
        }

        inline Numeric input(Numeric xx) const {

            Numeric w = 1, ret = Mdiffs[0];
            for (size_t i = 1; i < count(); i++) {
                w *= (xx - X(i-1));
                ret += Mdiffs[i] * w;
            }
            return ret;
        }

    private:
        std::vector<Numeric> Mdiffs;

    };

    class LagrangePoly :
        public impl::IntrplBaseType<Point2d>,
        public impl::Coord2dBaseType<Point2d> {

    public:

        using Container = NumaData::Container;

        // O(N^2)
        inline void initialize() {
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

}
}
