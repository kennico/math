#pragma once

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <limits>
#include <cassert>


namespace knylaw {
namespace math {

    /**
     * TODO mutable or immutable?
     */
    class BigInteger {

#ifdef KNY_MATH_DEBUG
    public:
#else
    private:
#endif

        using SigType   = int32_t;
        using DgtType   = uint32_t;
        using SgnTmpDgt    = int64_t;
        using USgnTmpDgt   = uint64_t;
        /**
         * Digits are stored in <b>LITTLE ENDIAN</b> order. The least significant digit
         * lies at the front of the container.
         * std::vector has already overloaded assignment operator for rvalue.
         */
        using DgtStorage = std::vector<DgtType>;

        /**
         * Construct an integer with a value of 0
         */
        BigInteger() = default;
//        BigInteger(BigInteger&&) = default;

        BigInteger(DgtStorage&& digits, SigType sign): Mdigits(digits), Msign(sign) {

        }

        BigInteger(const DgtStorage& digits, SigType sign): Mdigits(digits), Msign(sign){

        }

    public:

        constexpr static DgtType  DGTMASK = std::numeric_limits<DgtType>::max();
        constexpr static int      DGTLEN  = std::numeric_limits<DgtType>::digits;

        BigInteger(const BigInteger& other) = default;
        BigInteger(BigInteger&& other) = default;

        ~BigInteger() = default;
        /**
         *
         * @param base must reside between 2 and 36
         * @return
         */
        std::string str(int base=10) const;

        inline size_t length() const noexcept {
            return Mdigits.size();
        }

        inline SigType sign() const noexcept {
            return Msign;
        }

        /**
         * Explicit defaulted overloading assignment operator
         * @return *this
         */
        BigInteger& operator=(const BigInteger&) = default;
        BigInteger& operator=(BigInteger&&) = default;

        inline BigInteger operator-() const {
            return BigInteger(Mdigits, static_cast<SigType>(-Msign));
        }

        inline explicit operator bool() const noexcept {
            return static_cast<bool>(Msign);
        }

        inline bool operator!() const noexcept {
            return !Msign;
        }

        friend bool operator==(const BigInteger&, const BigInteger&);
        friend bool operator!=(const BigInteger&, const BigInteger&);
        friend bool operator>(const BigInteger&, const BigInteger&);
        friend bool operator<(const BigInteger&, const BigInteger&);
        friend bool operator>=(const BigInteger&, const BigInteger&);
        friend bool operator<=(const BigInteger&, const BigInteger&);


        inline BigInteger& operator+=(const BigInteger& other) {
            if (other.sign() != 0) {
                if (sign() == 0) {
                    // (zero) += (pos) / (zero) += (neg)
                    assert(emptyStorage());
                    operator=(other);
                } else if (sign() == other.sign()) {
                    // (pos) += (pos) / (neg) += (neg)
                    addInPlace(Mdigits, other.Mdigits);
                } else {
                    // (pos) += (neg) / (neg) += (pos)
                    if(subInPlace(Mdigits, other.Mdigits)) {
                        // we encounter a borrowing
                        // abs(Mdigits) < abs(other.Mdigits)
                        Msign = inverseSignNumber(Msign);
                    } else if (Mdigits.empty()) {
                        Msign = 0;
                    }
                }
            } else {
                // (n) += 0
                assert(other.emptyStorage());
            }
            return *this;
        }

        inline BigInteger&operator-=(const BigInteger& other) {
            if (other.sign() != 0) {
                if (sign() == 0) {
                    // (zero) -= (pos) / (zero) -= (neg)
                    assert(emptyStorage());
                    Mdigits = other.Mdigits;
                    Msign = inverseSignNumber(other.Msign);
                } else if (sign() == other.sign()){
                    // (pos) -= (pos) / (neg) -= (neg)
                    if (subInPlace(Mdigits, other.Mdigits)) {
                        Msign = inverseSignNumber(Msign);
                    } else if (Mdigits.empty()) {
                        Msign = 0;
                    }
                } else {
                    // (pos) -= (neg) / (neg) -= (pos)
                    addInPlace(Mdigits, other.Mdigits); // The sign won't change
                }
            } else {
                // (n) -= 0
                assert(other.emptyStorage());
            }
            return *this;
        }

        inline static BigInteger valueOf(long number) {
            return number == 0 ? BigInteger() : BigInteger(
                    {static_cast<DgtType>(std::abs(number))}, number > 0 ? 1 : -1
            );
        }

        static BigInteger valueOf(std::string src, int base = 10);


#ifdef KNY_MATH_DEBUG
    public:
#else
    private:
#endif
        /**
         * Used for debug assertion
         * @return true if DigitsStorage is empty
         */
        inline bool emptyStorage() const noexcept {
            return Mdigits.empty();
        };
        /**
         * Used for debug assertion
         * @return true if the no zero is stored at the big end of DgtStorage
         */
        inline static bool notStartsWithZero(const DgtStorage& digits) noexcept {
            return digits.back() != 0;
        }
        /**
         * Convert std::string digits into DgtStorage.
         *
         * The value of <i>srcDigitBase^srcDigitLen</i> must be not greater than the maximum of type ImplDigit can hold.
         * Every number returned is stored with the new base <i>srcDigitBase^srcDigitLen</i> in <b>LITTLE ENDIAN</b>.
         *
         * TODO use char array instead?
         *
         * @param src assumed to be non-negative and stored in <b>LITTLE ENDIAN</b>
         * @param reqDgtLen requested digit len
         * @param srcDgtBase base in <b>src</b> since the callee can't detect its original base
         * @return DgtStorage
         */
        static DgtStorage strToLargerBaseDgt(const std::string &str, size_t reqDgtLen, int srcDgtBase);
        /**
         * @param a DgtStorage
         * @param b DgtStorage
         * @return whether <b>a</b> is less than <b>b</b>
         */
        inline static bool dgtLessThan(const DgtStorage &a, const DgtStorage &b) {
            /*
             * Use reversed iterator due to the little endian storage
             */
            return a.size() != b.size() ? a.size() < b.size() :
                   std::lexicographical_compare(a.crbegin(), a.crend(), b.crbegin(), b.crend());
        }
        /**
         * Perform addition <b>IN PLACE</b>.
         *
         * Pad zeros to its big end if the length of a is smaller than b's and add b to a.
         *
         * @param a
         * @param b
         * @return true if the addition produces a carry
         */
        static void addInPlace(DgtStorage &a, const DgtStorage &b);
        /**
         * @param minuend
         * @param subtrahend
         * @return true if a borrowing is propagated, which indicates a negative result.
         */
        static bool subInPlace(DgtStorage &minuend, const DgtStorage &subtrahend);
        /**
         * Add a digit at given position. This equals the operation "a += digit * radix ^ offset"
         *
         * @param a
         * @param digit 
         * @param offset not greater than a.size() but allowed to be equal
         * @return
         */
        static void addRadixExpInPlace(DgtStorage &a, DgtType digit, size_t offset = 0);
        /**
         * "Subtract" a digit from given position. This equals the operation "a -= digit * radix ^ offset"
         * Leading zeros will get removed.
         * 
         * @param a
         * @param digit
         * @param offset not greater than a.size() but allowed to be equal
         * @return true if a borrowing is propagated, which indicates a negative result.
         *              That only happens if <b>a</b> holds only one digit and a is smaller than b.
         */
        static bool subRadixExpInPlace(DgtStorage &a, DgtType digit, size_t offset = 0);
        /**
         * Remove zeros from its big end
         * @param digits
         */
        inline static void stripLeadingZeros(DgtStorage& digits) {
            while (!digits.empty() && digits.back() == 0) {
                digits.pop_back();
            }
        }
        /**
         * Pad zeros to src's big end until src holds digits as many as tar's.
         * If the count of digits in src is not less than tar, no zero is prepended.
         * @param src
         * @param tar
         */
        inline static bool zeroPaddingAlign(DgtStorage &src, const DgtStorage &tar) {
            bool happened = (src.size() < tar.size());
            while (src.size() < tar.size()) {
                src.push_back(0);
            }
            return happened;
        }
        /**
         * 
         * @param sign 
         * @return 
         */
        inline static SigType inverseSignNumber(SigType sign) {
            return -1 * sign;
        }
        /**
         * Perform division and divide unsigned digits(dividend) by the given digit(divisor)
         * @param dividend
         * @param divisor unsigned and must not be zero
         * @return
         */
        static DgtType uncheckedDivInPlace(DgtStorage &dividend, USgnTmpDgt divisor);

    private:
        /**
         * Digits are stored in <b>LITTLE ENDIAN</b> order. The least significant digit
         * lies at the front of the container.
         */
        DgtStorage   Mdigits{};
        /*
         * -1, 0, 1
         */
        SigType    Msign{0};
    };
    /**
     * Not tested.
     * @param a 
     * @param b 
     * @return 
     */
    inline bool operator==(const BigInteger& a, const BigInteger& b) {
        return a.Msign == b.Msign && a.Mdigits == b.Mdigits;
    }

    inline bool operator!=(const BigInteger& a, const BigInteger& b) {
        return !(a == b);
    }

    inline bool operator<(const BigInteger& a, const BigInteger& b) {
        return a.Msign != b.Msign? a.Msign < b.Msign
                :((a.Msign != 0) && (a.Msign == 1? BigInteger::dgtLessThan(a.Mdigits, b.Mdigits)
                                                 : BigInteger::dgtLessThan(b.Mdigits, a.Mdigits)));
    }

    inline bool operator>(const BigInteger& a, const BigInteger& b) {
        return b < a;
    }

    inline bool operator<=(const BigInteger& a, const BigInteger& b) {
        return !(a > b);
    }

    inline bool operator>=(const BigInteger& a, const BigInteger& b) {
        return !(a < b);
    }


    inline std::string to_string(const BigInteger& bigInteger) {
        return bigInteger.str();
    }
    
}
}