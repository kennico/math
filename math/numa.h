#pragma once
#include <list>
#include <vector>
#include <cassert>

namespace knylaw {
namespace math {


    using Float = double;
    
    struct Point {
        Float x, y;
    };

    namespace details {
        

        class InterpolationBase {

        public:

            InterpolationBase() = default;

            // Expect container holding struct of Point type
            template<typename InIt> void accept(InIt first, InIt last) {
                std::for_each(first, last, [=](auto p) {
                    this->Mpoints.push_back(p);
                });
            }

            inline const std::vector<Point>& points() const {
                return Mpoints;
            }

            inline const Point& p(int index) const {
                return Mpoints[index];
            }
        
        private:
            
            std::vector<Point> Mpoints;

        };

    }

    class NewtonPoly : 
        public details::InterpolationBase {

    public:

        inline void initialize() {
            assert(points().size() > 0);

            for (auto p : points()) {
                Mdiffs.push_back(p.y);
            }

            for (size_t d = 1; d <= Mdiffs.size() - 1; d++) { // compute the differences of degree d+1
                for (size_t i = Mdiffs.size() - 1; i >= d; i--) {
                    Mdiffs[i] = (Mdiffs[i] - Mdiffs[i - 1]) / (p(i).x - p(i-d).x);
                }
            }
        }

    public:

        inline Float input(Float xx) const {
            assert(Mdiffs.size() > 0);

            Float w = 1, ret = Mdiffs[0];
            for (size_t i = 1; i < points().size() ; i++) {
                w *= (xx - p(i - 1).x);
                ret += Mdiffs[i] * w;
            }
            return ret;
        }

    private:
        std::vector<Float> Mdiffs;
    };

    class LagrangePoly : 
        public details::InterpolationBase {
    public:
        inline void initialize() {
            // TODO
        }

        inline Float input(Float xx) const {
            // TODO
            return 0.0;
        }
    };

    template<typename Intrpl, // interpolation type
        typename InIt> // iterator of container holding Point struct
    Intrpl interpolation(InIt first, InIt last) {
        Intrpl intrpl;
        intrpl.accept(first, last);
        intrpl.initialize();
        return intrpl;
    }

    template<typename Intrpl, // interpolation type
        typename InIt, // iterator of container holding numbers
        typename Func> // function that will be applied to the numbers above
    Intrpl interpolation(InIt first, InIt last, Func f) {
        std::vector<Point> ps;
        std::for_each(first, last, [&ps, f](auto num) {
            ps.push_back({ num, f(num) });
        });
        return interpolation<Intrpl>(ps.begin(), ps.end());
    }
    
    
}
}