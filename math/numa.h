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

            InterpolationBase() : Mpoints() {

            }

            // Expect container of coordinates
            template<typename InIt> void accept(InIt first, InIt last) {
                std::for_each(first, last, [=](auto p) {
                    this->Mpoints.push_back(p);
                });
            }

            // Expect x
            template<typename InIt, typename Function> void accept(InIt first, InIt last, Function f) {
                std::for_each(first, last, [=](auto x) {
                    this->Mpoints.push_back({ x, f(x) });
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

    class NewtonPolynormial : public details::InterpolationBase {

    public:

        NewtonPolynormial() : InterpolationBase(), Mdiffs() {

        }

    public:

        inline void computeDiffs() {
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

        inline Float input(Float xx) {
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

}
}