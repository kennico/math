#include <cassert>
#include <cstring>

#include "numt/bigint.h"
#include "utils.h"

//#define IS_POWERS_OF2(positive) (postive & (positive-1) == 0))



namespace knylaw {
namespace math {
    /**
     * b ^ RadixLength[b] <= 4294967296
     */
    static BigInteger::DgtType RadixLength[] = {
            0, 0,
            32, 20, 16, 13, 12, 11, 10, 10, 9, // base 2~10
            9, 8, 8, 8, 8, 8, 7, 7, 7, 7, // base 11~20
            7, 7, 7, 6, 6, 6, 6, 6, 6, 6, // base 21~30
            6, 6, 6, 6, 6, 6 // base 31~36
    };
    /**
     * RadixPowers[b] <= 4294967296
     */
    static BigInteger::USgnTmpDgt RadixPowers[] = {
            0, 0,
            4294967296, 3486784401, 4294967296, 1220703125, 2176782336, 1977326743, 1073741824, 3486784401, 1000000000,
            2357947691, 429981696, 815730721, 1475789056, 2562890625, 4294967296, 410338673, 612220032, 893871739, 1280000000,
            1801088541, 2494357888, 3404825447, 191102976, 244140625, 308915776, 387420489, 481890304, 594823321, 729000000,
            887503681, 1073741824, 1291467969, 1544804416, 1838265625, 2176782336
    };
    /**
     *
     * Divide an integer consists of multiple digits by an individual digit <b>IN PLACE</b>.
     * The function template modifies data stored behind iterators. As a result, the integer
     * that represented by the container behind iterators may starts with unexpected zeros.
     *
     * @tparam LShiftOneDigitAdd a callable f(A, B)
     * @tparam Modular a callable f(A)
     * @tparam Divide a callable f(A)
     * @tparam DgtInOutIt iterator of container storing unsigned digits
     * @tparam DgtType the value type container storing unsigned digits usually deduced from <b>DigitInIt</b>
     *
     * @param first stores the <b>MOST</b> significant digit
     * @param last excluded
     * @param lsftAdd calculate <i>A * base + B</i>, where both A and B are digits while base is associated
     *                  with the digits stored in container. Its return type must be large enough to hold <i>A * base + B</i>
     * @param divide calculate <i>A (mod modulus)</i> where modulus is caller-defined.
     *              Its parameter type must be large enough to hold <i>A * B</i>
     * @param mod calculate <i>A / divisor</i> where divisor is caller-defined.
     *              Its parameter type must be large enough to hold <i>A * B</i>
     * @return remainder, while original container is overridden by the quotient
     */
    template <typename LShiftOneDigitAdd,
            typename Modular,
            typename Divide,
            typename DgtInOutIt,
            typename DgtType = typename std::iterator_traits<DgtInOutIt>::value_type>
    DgtType divDgtInPlaceImpl(DgtInOutIt first, DgtInOutIt last, LShiftOneDigitAdd lsftAdd, Modular mod, Divide divide) {

        DgtType remainder = 0, quotient;

        for(;first != last; ++first) {
            auto temp = lsftAdd(remainder, *first);

            quotient  = static_cast<DgtType>(divide(temp));
            remainder = static_cast<DgtType>(mod(temp));

            *first = quotient;
        }

        return remainder;
    }

    template <typename USgnTmpDgt,
            typename DgtInIt,
            typename DgtType= typename std::iterator_traits<DgtInIt>::value_type>
    DgtType divDgtInPlaceImpl(DgtInIt first, DgtInIt last, DgtType divisor, USgnTmpDgt base) {
        return divDgtInPlaceImpl(
                first, last,
                [&](auto a, auto b) -> USgnTmpDgt { return a * base + b; },
                [&](auto d) -> DgtType { return d % divisor; },
                [&](auto d) -> DgtType { return d / divisor; }
        );
    };


    /**
     * Divide a DigitStorage by an individual digit <b>IN PLACE</b>
     *
     * @param dividend
     * @param divisor
     * @return remainder, while original dividend is overridden by the quotient
     */
    BigInteger::DgtType BigInteger::uncheckedDivInPlace(
            BigInteger::DgtStorage &dividend, USgnTmpDgt divisor) {

        auto ret = divDgtInPlaceImpl(
                dividend.rbegin(), dividend.rend(),
                [&](auto a, auto b) -> USgnTmpDgt { return (((USgnTmpDgt) a) << DGTLEN) | b; },
                [&](auto d) -> DgtType { return d % divisor; },
                [&](auto d) -> DgtType { return d / divisor; }
        );
        stripLeadingZeros(dividend);
        return ret;
    }

    void BigInteger::addRadixExpInPlace(BigInteger::DgtStorage &a, DgtType digit, size_t offset) {
        assert(offset <= a.size());

        USgnTmpDgt carry = digit;
        for(; offset < a.size(); offset++) {
            carry += a[offset];
            a[offset] = static_cast<DgtType>(carry & DGTMASK);
            carry = (carry >> DGTLEN) & DGTMASK;
            assert(carry == 1 || carry == 0);
        }

        if (carry > 0) {
            a.push_back(static_cast<unsigned int &&>(carry));
        }
    }

    bool BigInteger::subRadixExpInPlace(BigInteger::DgtStorage &a, BigInteger::DgtType digit, size_t offset) {
        assert(offset <= a.size());

        DgtType& borrow = digit;
        for (DgtType temp; offset < a.size(); offset++) {
            temp = a[offset] - borrow;
            borrow = static_cast<DgtType>(a[offset] < borrow);
            a[offset] = temp;
        }

        stripLeadingZeros(a);
        return borrow == 1;
    }

    void BigInteger::addInPlace(BigInteger::DgtStorage &a, const BigInteger::DgtStorage &b) {
        // Zero padding
        zeroPaddingAlign(a, b);

        size_t pos = 0;
        USgnTmpDgt carry = 0;
        for (; pos < b.size(); pos++) {
            carry += a[pos]; // implicit type promotion in case of wrapping around
            carry += b[pos]; // TODO this needs to be verified
            a[pos] = static_cast<DgtType>(carry & DGTMASK);
            carry = (carry >> DGTLEN) & DGTMASK;
            assert(carry == 1 || carry == 0);
        }
        if (carry == 1) {
            addRadixExpInPlace(a, static_cast<DgtType>(carry), pos);
        }
        assert(notStartsWithZero(a));
    }

    /*
     * (1)
     *      (0101)    (0101)    (0000)'    0000(0)'    0001(1)
     *  -                                  0011(3)     0011(3)
     * -------------------------------------------------------
     *      (0101)    (0100)    (0000)'    1100(C)     1110(E)
     *
     *
     * (2)
     *              0000(0)'    0001(1)
     *  -           0011(3)     0011(3)
     * --------------------------------
     *  b           1100(C)     1110(E)
     * --------------------------------
     *  ~           0011(3)     0001(1)
     *  +                       0001(1)
     * --------------------------------
     *              0011(3)     0002(2)
     *
     * The subtractions listed above on digits(e.g. hexadecimal) take the advantage of
     * the well-defined subtraction between unsigned integers.
     *
     * From C11 standard, "...a result that cannot be represented by the resulting
     * unsigned integer type is reduced <b>modulo</b> the number that is one greater than
     * the largest value that can be represented by the resulting type".
     *
     * Borrowing can be easily detected by comparing two digits.
     *
     *
     * TODO Perform zero padding and detect borrow?
     */
    bool BigInteger::subInPlace(BigInteger::DgtStorage &minuend, const BigInteger::DgtStorage &subtrahend) {
        const auto dbgMinuendSrcSize = minuend.size();
        const auto dbgSubtracthendSrcSize = subtrahend.size();

        zeroPaddingAlign(minuend, subtrahend);
        size_t pos = 0;
        DgtType borrow = 0, temp;
        for (;pos < subtrahend.size(); ++pos) {
            temp = minuend[pos] - subtrahend[pos] - borrow;
            borrow = static_cast<DgtType>(minuend[pos] < subtrahend[pos] + borrow);
            minuend[pos] = temp;
        }
        if(subRadixExpInPlace(minuend, borrow, pos)) {
            assert(dbgMinuendSrcSize <= dbgSubtracthendSrcSize);
            for(auto& digit: minuend) {
                digit = ~digit;
            }
            addRadixExpInPlace(minuend, 1);
            return true;
        } else {
            assert(dbgMinuendSrcSize >= dbgSubtracthendSrcSize);
            assert(minuend.empty() || notStartsWithZero(minuend));
            return false;
        }
    }
    
    std::string BigInteger::str(int base) const {
        /**
         * Only the base of 10 is allowed at present
         */
        assert(base==10);
        if (Msign == 0) {
            assert(emptyStorage());
            return "0";
        }

        auto quot = Mdigits;
        std::string str;
        while (true) {
            DgtType rem = uncheckedDivInPlace(quot, RadixPowers[base]);
            if (quot.empty()) {
                if (rem != 0){
                    str = std::to_string(rem) + str;
                }
                break;
            } else {
                str = stringf("%09u", rem) + str;
            }
        }
        assert(!str.empty());
        return Msign == -1? ("-" + str) : str;
    }

    BigInteger::DgtStorage BigInteger::strToLargerBaseDgt(const std::string &src, size_t reqDgtLen, int srcDgtBase) {
        
        DgtStorage dgt;
        auto rpos = (src.length() >= reqDgtLen) ? static_cast<long>(src.length() - reqDgtLen) : -1;
        for(; rpos >= 0; rpos -= reqDgtLen) {
            /*
             * TODO detect non-digit character and throw exception?
             */
            /*
             * http://en.cppreference.com/w/cpp/string/basic_string/substr
             *
             *  If the requested substring extends past the end of the string,
             *  or if count == npos, the returned substring is [pos, size()).
             */
            auto d =stoul(src.substr(rpos, reqDgtLen), nullptr, srcDgtBase);
            dgt.push_back(static_cast<DgtType&&>(d));
        }

        if ((src.length() % reqDgtLen) != 0) {
            auto d = stoul(src.substr(0, src.length() % reqDgtLen), nullptr, srcDgtBase);
            dgt.push_back(static_cast<DgtType&&>(d));
        }

        return dgt;
    }

    BigInteger BigInteger::valueOf(std::string src, int base) {
        assert(base >= 2 && base <= 36);

        trim(src);
        if (src.length() < RadixLength[base]) {
            return valueOf(stol(src, nullptr, base));
        }

        SigType sign = 1;
        if (src[0] == '-') {
            sign = -1;
            src.erase(src.begin());
        }

        DgtStorage digits, quot = strToLargerBaseDgt(src, RadixLength[base], base);
        while (!quot.empty()) {     // if quotient doesn't equal 0, then continue
            DgtType rem = divDgtInPlaceImpl(
                    quot.rbegin(), quot.rend(),
                    // Though both a and b are of the same type(DgtType), we still need to cast the left operand into a larger data type. Otherwise, it will wrap around.
                    [&](auto a, auto b) -> USgnTmpDgt { return ((USgnTmpDgt) a) * RadixPowers[base] + b; },
                    [&](auto d) -> DgtType { return d & DGTMASK; },
                    [&](auto d) -> DgtType { return d >> DGTLEN; }
            );
            stripLeadingZeros(quot);
            digits.push_back(rem);
        }
        stripLeadingZeros(digits);
        return BigInteger(std::move(digits), digits.empty()? 0:sign);
    }

}
}