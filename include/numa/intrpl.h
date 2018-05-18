#pragma once
#include <vector>
#include <algorithm>
#include <cassert>

namespace knylaw {
namespace math {

    namespace impl {

        template<typename Real> class Point2d;
        template<typename Real> class Point2ddy;
        template<typename Elem, typename Cont = std::vector<Elem>> class IntrplBaseType;
        template<typename Coord> class XyData;
        template<typename Coordd> class XyDerivData;

        template<typename Real>
        class Point2d {

        public:

            using Numeric = Real;

            Numeric x, y;

        public:

            Point2d() = default;

            Point2d(Numeric x_, Numeric y_) : x(x_), y(y_) {

            }

            virtual ~Point2d() = default;
        };

        template<typename Elem, typename Cont>
        class IntrplBaseType {

        public:

            using DataCont = Cont;
            using Element = Elem;

        protected:

            IntrplBaseType() = default;

            explicit IntrplBaseType(size_t size) : Mdata(size) {

            }

            inline Element& data(size_t pos) {
                return Mdata[pos];
            }

            inline DataCont& data() {
                return Mdata;
            }

            inline void assgin(const DataCont& container) {
                this->Mdata = container;
            }

        public:

            explicit IntrplBaseType(const DataCont& container) : Mdata(container) {

            }

            virtual ~IntrplBaseType() = default;

        public:

            inline size_t count() const {
                return Mdata.size();
            }

            inline const Element & data(size_t pos) const {
                return Mdata[pos];
            }

            inline const DataCont & data() const {
                return Mdata;
            }

        private:

            DataCont Mdata;

        };


        template<typename Coord>
        class XyData : public IntrplBaseType<Coord> {

        public:

            using Numeric = typename Coord::Numeric;
            using DataCont = typename IntrplBaseType<Coord>::DataCont;

        protected:

            XyData() = default;

            inline Numeric & X(size_t pos) {
                return this->data(pos).x;
            }

            inline Numeric & Y(size_t pos) {
                return this->data(pos).y;
            }

        public:
            //
            //Pass points by calling constructor
            //
            explicit XyData(const DataCont& container) : IntrplBaseType<Coord>(container) {

            }

            ~XyData() override = default;

            inline Numeric X(size_t pos) const {
                return this->data(pos).x;
            }

            inline Numeric Y(size_t pos) const {
                return this->data(pos).y;
            }

            inline size_t N() const {
                return this->count() - 1;
            }
        };

        template<typename Real>
        class Point2ddy :
            virtual public Point2d<Real> {

        public:

            using Numeric = typename Point2d<Real>::Numeric;

            Numeric dy;

        public:

            Point2ddy() = default;

            ~Point2ddy() override = default;

            Point2ddy(Numeric x_, Numeric y_, Numeric dy_) : Point2d<Numeric>(x_, y_), dy(dy_) {

            };

        };

        template<typename Coordd>
        class XyDerivData : public XyData<Coordd> {

        public:

            using Numeric = typename XyData<Coordd>::Numeric;
            using DataCont = typename XyData<Coordd>::DataCont;

        protected:

            XyDerivData() = default;

            inline Numeric & DY(size_t pos) {
                return this->data(pos).dy;
            }

        public:
            //
            // template argument list is needed on calling constructor of super class
            //
            explicit XyDerivData(const DataCont& container) : XyData<Coordd>(container) {

            }

            ~XyDerivData() override = default;

            inline Numeric DY(size_t pos) const {
                return this->data(pos).dy;
            }
        };


    }

    using Point2d = impl::Point2d<double>;
    using Point2ddy = impl::Point2ddy<double>;

    class NewtonPoly
        : public impl::XyData<Point2d> {

    public:

        using InputCont = typename XyData<Point2d>::DataCont;

        explicit NewtonPoly(const InputCont& container) : XyData(container), Mdiffs() {
            for (size_t i = 0; i < count(); i++) {
                Mdiffs.push_back(Y(i));
            }
        
            for (size_t d = 1; d < count(); d++) { // compute the differences of degree d+1
                for (size_t i = count() - 1; i >= d; i--) {
                    Mdiffs[i] = (Mdiffs[i] - Mdiffs[i-1]) / (X(i) - X(i-d));
                }
            }
        }

        ~NewtonPoly() override = default;

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

    class LagrangePoly 
        : public impl::XyData<Point2d> {

    public:

        using InputCont = typename XyData<Point2d>::DataCont;

        // O(N^2)
        explicit LagrangePoly(const InputCont& container): XyData(container), Mweights() {

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

        ~LagrangePoly() override = default;

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

    class SectionedCubicHermite
        : public impl::XyDerivData<Point2ddy> {
    protected:

        SectionedCubicHermite() = default;

        // operator() overloaded
        struct compare {
            inline bool operator()(const Point2ddy& pd, Numeric xx) {
                return pd.x < xx;
            }
        };

    public:

        using InputCont = typename XyDerivData<Point2ddy>::DataCont;
        //
        // Unlike Newtion polynomial or Lagrange polynomial,
        // sectioned cubic hermite method requires data non-decreasingly ordered by x coordinate.
        //
        explicit SectionedCubicHermite(const InputCont& container) : XyDerivData(container) {

        }

        ~SectionedCubicHermite() override = default;

        inline Numeric input(Numeric xx) const {
            // search for a range(x_i, x_{i+1}) that xx lies within
            auto ite = std::lower_bound(data().cbegin(), data().cend(), xx, compare());
            auto d = ite - data().cbegin();
            
            assert(d > 0);
            assert(d < count());

            Numeric yy = 0;
            size_t i = static_cast<size_t>(d - 1);
            // h_i(x)
            yy += (1 + 2 * (xx - X(i)) / (X(i+1) - X(i))) * std::pow((xx-X(i+1)) / (X(i)-X(i+1)), 2) * Y(i);
            // h_{i+1}(x)
            yy += (1 + 2 * (xx - X(i+1)) / (X(i) - X(i+1))) * std::pow((xx-X(i)) / (X(i+1)-X(i)), 2) * Y(i+1);
            // h_i^1(x)
            yy += (xx - X(i)) * std::pow((xx - X(i+1)) / (X(i) - X(i+1)),2) * DY(i);
            // h_{i+1}^1(x)
            yy += (xx - X(i+1)) * std::pow((xx - X(i)) / (X(i+1) - X(i)),2) * DY(i+1);

            return yy;
        }

    };
    /**
     * It seems that it only works for cubic functions such as f(x) = 2x^3 + 5x^2 + 7 x – 8.
     * For polynomials that have lower degree it behaves inaccurately.
     */
    class CubicSpline
        : public SectionedCubicHermite {

    public:

        using InputCont = typename NewtonPoly::InputCont;

    public:

        explicit CubicSpline(const InputCont& cont) : SectionedCubicHermite() {
            auto back = std::back_inserter(data());
            std::for_each(cont.cbegin(), cont.cend(), [&](auto p) {
                back = Point2ddy(p.x, p.y, 0.0);
            });

            initialize(1.0, 0, 
                3.0 / (X(1) - X(0)) * (Y(1)-Y(0)),
                3.0 / (X(N()) - X(N()-1)) * (Y(N()) - Y(N()-1))
            );
        }
        
        /*
        * Apply cubic spline method using given values of 1st derivative at x=x_0 and x_n
        */
        CubicSpline(const InputCont& cont, Numeric dy0, Numeric dyN) : SectionedCubicHermite() {
            auto back = std::back_inserter(data());
            std::for_each(cont.cbegin(), cont.cend(), [&](auto p) {
                back = Point2ddy(p.x, p.y, 0.0);
            });

            initialize(0, 1, 2*dy0, 2*dyN);
        }

    private:

        void initialize(Numeric al0, Numeric alN, Numeric bt0, Numeric btN) {

            std::vector<Numeric> h(count());
            // compute h_i
            for (size_t i = 0; i < N(); i++) {
                h[i] = X(i+1) - X(i);
            }

            std::vector<Numeric> alpha(count()), beta(count());
            alpha[0] = al0;
            alpha[N()] = alN;
            beta[0] = bt0;
            beta[N()] = btN;

            // compute alpha_i and beta_i
            for (size_t i = 1; i < N(); i++) {
                alpha[i] = h[i-1] / (h[i-1] + h[i]);
                beta[i] = 3.0 * ((1.0 - alpha[i]) / h[i-1] * (Y(i) - Y(i-1)) + alpha[i] / h[i] * (Y(i+1) - Y(i)));
            }

            // compute a_i and b_i
            std::vector<Numeric> a(count()), b(count());
            a[0] = -alpha[0] / 2.0;
            b[0] = beta[0] / 2.0;
            for (size_t i = 1; i < count(); i++) {
                a[i] = -alpha[i] / (2.0 + (1.0 - alpha[i]) * a[i-1]);
                b[i] = (beta[i] - (1.0 - alpha[i]) * b[i-1]) / (2 + (1 - alpha[i])*a[i-1]);
            }
            
            // compute and set dy_i          
            DY(N()) = b[N()];
            for (int i = N()-1; i > 0; i--) {
                DY(i) = a[i] * DY(i+1) + b[i];
            }
        }

    };
}
}