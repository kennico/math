#pragma once
#include <vector>
#include <algorithm>
#include <cassert>

#include "utils.h"

namespace knylaw {
namespace math {

    namespace impl {

        template<typename DigitType, 
            typename ContainerType=std::vector<DigitType>>
        class BigInteger_ImplBase {

        public:

            using Digit     = DigitType;
            using Container = ContainerType;
            using Element   = typename Container::value_type;

        public:

            BigInteger_ImplBase() : Mdata({ 0 }) {

            }

            // return number of digits used.
            inline size_t dgtnum() const {
                return Mdata.size();
            }

        protected:

            // can not be declared as "friend inline" ?
            inline static void swap(
                BigInteger_ImplBase<Digit, Container>& a, 
                BigInteger_ImplBase<Digit, Container>& b
            ) {
                std::swap(a.Mdata, b.Mdata);
            }

            // access the digit at given index
            inline Digit& dgt(size_t index) {
                return Mdata[index];
            }

            // access the digit at given index
            inline const Digit& dgt(size_t index) const {
                return Mdata[index];
            }

            // add a digit to the MSB(Most Significant Bit)
            inline void pushd(Digit d) {
                Mdata.push_back(d);
            }

            // remove the MSB
            inline void popd() {
                Mdata.pop_back();
            }

        private:
            // LSB is stored at index 0
            Container Mdata;

        };

        //
        // class template BigInteger_Operators
        //      template parameters:
        //          - BASE
        //          - Digit: digit type
        //          - Container: container type
        //          - Carry: class template; handle a computational result which may be greater that BASE or exceed Digit
        //              template parameters:
        //                  - BASE
        //                  - Digit
        //              function members:
        //                  - modBase
        //                  - fromAdd
        //                  - fromMul
        //          - Ctor: class template; construct from (unsigned)short, (unsigned)int, (unsigned)long, (unsigned)long long, etc.
        //              template parameters:
        //                  - Carry
        //
        //
        // to_string: how to convert it into string
        //

        class BigInteger_1E4 :
            public impl::BigInteger_ImplBase<int> {

        protected:

            using ImplBase = impl::BigInteger_ImplBase<int>;

        public:
            
            const static int BASE = int(1e4);

            BigInteger_1E4() = default;

            BigInteger_1E4(int number) : ImplBase() {
                popd();
                while (number) {
                    pushd(modBase(number, number));
                }

            }
            
        protected:

            inline static void modBase(Element num, Element & quot, Digit & result) {
                result = modBase(num, quot);
            }

            inline static Digit modBase(Element num, Element & quot) {
                std::div_t res = std::div(num, BASE);
                quot = res.quot;
                return res.rem;
            }

            // Obtain and store carry digit and result digit.
            inline static void fromAdd(Element num, Digit& carry, Digit& result) {
                result = fromAdd(num, carry);
            }

            // Obtain and store carry digit. Return result digit.
            inline static Digit fromAdd(Element num, Digit& c) {
                auto t = c;
                auto r = modBase(num, t);
                assert(c == 1 || c == 0);
                c = t;
                return r;
            }

        public:

            inline std::string str() const {
                std::string repr = "";
                for (size_t pos = 0; pos < dgtnum(); pos++) {
                    repr = stringf("%04d", dgt(pos)) + repr;
                }
                repr = repr.erase(0, repr.find_first_not_of('0'));
                return repr == "" ? "0" : repr;
            }

            // can be turned into a member of class template
            inline BigInteger_1E4& operator+=(const BigInteger_1E4& N) {                
                
                Digit c = 0;
                size_t pos = 0, minlen = std::min(dgtnum(), N.dgtnum());

                for (; pos < minlen; pos++) {
                    dgt(pos) = fromAdd(dgt(pos) + N.dgt(pos) + c, c);
                }
                
                for (; pos < dgtnum(); pos++) {
                    dgt(pos) = fromAdd(dgt(pos) + c, c);
                }

                for (; pos < N.dgtnum(); pos++) {
                    pushd(fromAdd(N.dgt(pos) + c, c));
                }

                if (c == 1) {
                    pushd(1);
                }

                return *this;
            }

            inline BigInteger_1E4& operator=(BigInteger_1E4 N) {
                ImplBase::swap(*this, N);
                return *this;
            }

            inline BigInteger_1E4 operator+(BigInteger_1E4 N) const {
                N.operator+=(*this);
                return N;
            }

            
        };


    }

}
}