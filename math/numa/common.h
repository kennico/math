#pragma once
#include <vector>

namespace knylaw {
namespace math {

    namespace impl {

        template<typename Real>
        class Point2d {

        public:

            using Numeric = Real;

        public:

            Point2d() = default;

            Point2d(Numeric x_, Numeric y_) : Mx(x_), My(y_) {

            }

            ~Point2d() = default;

            //Point2d<Numeric>& operator=(const Point2d<Numeric> & other) {
            //    this->x = other.x;
            //    this->y = other.y;
            //    return *this;
            //}

            inline Numeric getx() const {
                return Mx;
            }

            inline Numeric gety() const {
                return My;
            }

        private:

            Numeric Mx, My;
        };

    }

    using Point2d = impl::Point2d<double>;
    

    namespace impl {

        template<typename Elem, 
            typename Cont=std::vector<Elem>>
        class NumaData {

        public:

            using Element = Elem;
            using Container = Cont;

        public:

            NumaData(const Container& container) : Mdata(container) {

            }

        public:

            inline const Container & data() const {
                return Mdata;
            }

            inline const Element & data(size_t pos) const {
                return Mdata[pos];
            }

            inline size_t count() const {
                return Mdata.size();
            }

        protected:

            inline Element& data(size_t pos) {
                return Mdata[pos];
            }

        private:

            Container Mdata;

        };


        template<typename Coord>
        class NumaDataxy: public NumaData<Coord> {

        public:
            
            using Numeric   = typename Coord::Numeric;
            using Container = typename NumaData<Coord>::Container;

        public:

            NumaDataxy(const Container& container): NumaData<Coord>(container) {

            }

            inline Numeric X(size_t pos) const {
                return this->data(pos).getx();
            }

            inline Numeric Y(size_t pos) const {
                return this->data(pos).gety();
            }
        };

    }


    template<typename NumaMeth> // numerical analysis method
    class Helper {

    public:
        
        using Container = typename NumaMeth::Container;

    public:

        inline NumaMeth from(const Container& container) const {
            return NumaMeth(container);
        }

        template<typename InIt> // underlying container holding data of type the numerical analysis method expects
        inline NumaMeth from(InIt firstp, InIt lastp) const {
            Container contianer;
            std::for_each(firstp, lastp, [&](auto e) {
                contianer.push_back(e);
            });
            return from(contianer);
        }

    };

}
}
