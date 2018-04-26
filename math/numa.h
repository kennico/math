#pragma once
#include <list>
#include <vector>
#include <algorithm>

#include <cassert>

#include "utils.h"

namespace knylaw {
namespace math {

#if _MSC_VER > 1700
    using Float = double;
#else
	typedef double Float;
#endif

    struct Point {
        Float x, y;
    };

    namespace details {
        
        class InterpolationBase {

        public:

            // Expect two containers
            template<typename InIt> 
            void accept(InIt pfirst, InIt plast) {
                std::for_each(pfirst, plast, [=](auto p) {
                    this->Mpoints.push_back(p);
                });
            }

            // constexpr?
            inline size_t N() const {
                return count() - 1;
            }

			// constexpr?
			inline size_t count() const {
				return Mpoints.size();
			}

			inline Float x(int index) const {
				return Mpoints[index].x;
			}

			inline Float y(int index) const {
				return Mpoints[index].y;
			}

            inline const std::vector<Point>& points() const {
                return Mpoints;
            }
        
        private:
            
			std::vector<Point> Mpoints;

        };	

    }


    class NewtonPoly : 
        public details::InterpolationBase {

    public:
		// O(N^2)
        inline void initialize() {
            
            for (size_t i = 0; i < count(); i++) {
                Mdiffs.push_back(y(i));
            }

            for (size_t d = 1; d <= N(); d++) { // compute the differences of degree d+1
                for (size_t i = N(); i >= d; i--) {
                    Mdiffs[i] = (Mdiffs[i] - Mdiffs[i - 1]) / (x(i) - x(i-d));
                }
            }
        }

		inline Float diff(size_t index) const {
			return Mdiffs[index];
		}

    public:

        inline Float input(Float xx) const {
            assert(Mdiffs.size() > 0);

            Float w = 1, ret = Mdiffs[0];

			for (size_t i = 1; i < count() ; i++) {
                w *= (xx - x(i-1));
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
		// O(N^2)
        inline void initialize() {
			for(size_t i = 0; i < count(); ++i) {
				Float w = 1.0;
				for(size_t j = 0; j < count(); ++j) {
					if (j != i) {
						w *= x(i) - x(j);
					}
				}
				w = 1.0 / w;
				Mweights.push_back(w);
			}
        }

		inline Float weight(int index) const {
			return Mweights[index];
		}

		// O(N^2)
        inline Float input(Float xx) const {
            // Not optimized
			Float ret = 0;
			for (size_t i = 0; i < count(); i++) {
				auto w = Mweights[i] * y(i);
				for(size_t j = 0; j < count() ; j++) {
					if (j != i) {
						w *= xx - x(j);
					}
				}
				ret += w;
			}
            return ret;
        }

	private:
		std::vector<Float> Mweights;

    };

	//
	// Is the input data needed to be ordered?
	//
	class CubicSpline : 
		public details::InterpolationBase {

	public:

        void initialize();

        inline Float input(Float xx) {
            using std::pow;

            auto pos = std::lower_bound(
                points().cbegin(),  points().cend(),  xx, [](const Point& p, auto x) { return p.x < x; });

            auto i = pos - points().cbegin() - 1;
            assert(i >= 0);
            assert(i < count());

            Float yy = 0.0;
            yy += (1 + 2 * (xx - x(i)) / (x(i + 1) - x(i))) * pow((xx - x(i + 1)) / (x(i) - x(i + 1)), 2) * y(i);
            yy += (1 + 2 * (xx - x(i + 1)) / (x(i) - x(i + 1))) * pow((xx - x(i)) / (x(i + 1) - x(i)), 2) * y(i + 1);
            yy += (xx - x(i)) * pow((xx - x(i + 1)) / (x(i) - x(i + 1)), 2) * Mm[i];
            yy += (xx - x(i + 1)) * pow((xx - x(i)) / (x(i + 1) - x(i)), 2) * Mm[i + 1];

            return yy;
        }

	protected:

		std::vector<Float> Mm;
	};

    //
    // For cubic spline interpolation, the collection of x shall be ordered.
    //
    template<typename Intrpl, // interpolation type
        typename InIt> // iterator of container holding y
    Intrpl interpolationp(InIt pfirst, InIt plast) {
        Intrpl intrpl;
        intrpl.accept(pfirst, plast);
        intrpl.initialize();
        return intrpl;
    }
    //
    // For cubic spline interpolation, the collection of x shall be ordered.
    //
	template<typename Intrpl, // interpolation type
		typename InIt1, // iterator of container holding x
		typename InIt2> // iterator of container holding y
	Intrpl interpolationp(InIt1 xfirst, InIt1 xlast, InIt2 yfirst) {
        std::list<Point> temp;
        for_each(xfirst, xlast, yfirst, [&](auto x, auto y) {
            temp.push_back({ x,y });
        });
        return interpolationp<Intrpl>(temp.begin(), temp.end());
	}
    //
    // For cubic spline interpolation, the collection of x shall be ordered.
    //
    template<typename Intrpl, // interpolationp type
        typename InIt, // iterator of container holding numbers
        typename Func> // function that will be applied to the numbers above
    Intrpl interpolationf(InIt xfirst, InIt xlast, Func f) {
        std::list<Point> temp;
        std::for_each(xfirst, xlast, [&temp, f](auto x) {
            temp.push_back({ x,f(x) });
        });
        return interpolationp<Intrpl>(temp.begin(), temp.end());
    }
    
    
}
}