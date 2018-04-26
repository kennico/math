#pragma once
#include <vector>

namespace knylaw {
namespace math {

    namespace impl {
        template<typename Numeric> class Point;
    }

    using Point2d = impl::Point<double>;

    namespace impl {

        template<typename Numeric>
        class Point {

        public:
            Point() = default;
            
            Point(Numeric xx, Numeric yy) : x(xx), y(yy) {

            }

            ~Point() = default;

        public:
            Numeric x, y;
        };

        template<typename Numeric>
        struct Xgeter {
            inline Numeric operator()(const Point<Numeric>& coord) const {
                return coord.x;
            }
        };

        template<typename Numeric>
        struct Ygeter {
            inline Numeric operator()(const Point<Numeric>& coord) const {
                return coord.y;
            }
        };

        template<typename Coord> struct CoordTraits;

        template<typename Float> struct CoordTraits<Point<Float>> {
            using Numeric = Float;
            using Xgeter = Xgeter<Numeric>;
            using Ygeter = Ygeter<Numeric>;
        };

        //
        // Numerical analysis data collection
        //
        template<typename Elem,
            typename Cont = std::vector<Elem>>
        class NumaData {

        public:

            using Element   = Elem;
            using Container = Cont;

        public:
            //
            // https://stackoverflow.com/questions/13576055/how-is-default-different-from-for-default-constructor-and-destructor
            // 
            // Using a default constructor keeps abstract subclass
            // free from writing its own explicit constructor.
            //
            NumaData() = default;

        protected:
            //
            // this constructor may be redundant because a default one 
            // and the "accpet" function below are provided.
            //
            NumaData(const Container& container) : Mdata(container) {

            }


        public:

            ~NumaData() = default;
            //
            // calling this function is the only way to modify underlying data content.
            //
            inline void accept(const Container& contianer) {
                this->Mdata = contianer;
            }

            inline size_t count() const {
                return Mdata.size();
            }

            inline const Element& data(size_t pos) const {
                return Mdata[pos];
            }

            inline const Container& data() const {
                return Mdata;
            }

        private:
            Container Mdata;

        };

        //
        // Data that consists of (x,y) coordinates
        //
        template<typename Coord,
            typename Traits = CoordTraits<Coord>>
        class Coord2dBaseType
            :virtual public NumaData<Coord> {

        public:
            using Numeric = typename Traits::Numeric;

            inline Numeric X(size_t pos) const {
                return Mgetx(this->data(pos));
            }

            inline Numeric Y(size_t pos) const {
                return Mgety(this->data(pos));
            }

        private:

            typename Traits::Xgeter Mgetx; // require operator()(Input) overloading to get x
            typename Traits::Ygeter Mgety; // require operator()(Input) overloading to get y

        };

    }

    template<typename NumaMeth> // numerical analysis method
    class Helper {

    public:
        using Input = typename NumaMeth::Element;
        using Container = typename NumaMeth::Container;

    public:

        inline NumaMeth from(const Container& container) {
            NumaMeth meth;
            meth.accept(container);
            meth.initialize();
            return meth;
        }

        template<typename InIt> // underlying container holding data of type the numerical analysis method expects
        inline NumaMeth from(InIt firstp, InIt lastp) {
            Container contianer;

            std::for_each(firstp, lastp, [&](auto e) {
                contianer.push_back(e);
            });
            return from(contianer);
        }

    };
}
}