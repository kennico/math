#pragma once
#include <list>
#include <iterator>

/*
Integer + - * / = % copy ctor Integer(1)
*/


namespace knylaw {
namespace math {

    using CInteger = long;

    namespace details {

        template<typename Integer> Integer gcd_recur(Integer a, Integer b);
        template<typename Integer> Integer gcd_recur(Integer a, Integer b, Integer* xpp, Integer* xp);
        template<typename Integer> void highest_power(Integer number, Integer base, Integer* exp, Integer* rem);

    }





    /*
    Using euclidean algorithm.
    Return the greatest common (positive)divisor of a and b
    Chap.6
    */
    template<typename Integer> Integer gcd(Integer a, Integer b) {
        if (a == 0 || b == 0)
            return std::max(std::abs(a), std::abs(b));
        else
            return details::gcd_recur<Integer>(std::abs(a), std::abs(b));
    }

    template<typename InIt,
        typename Integer = typename std::iterator_traits<InIt>::value_type>
    Integer gcdm(InIt first, InIt last) {

        Integer g = *(first++);
        std::for_each(first, last, [&](auto n) {
            g = gcd<Integer>(g, n);
        });

        return g;
    }
    /*
    Using extended euclidean algorithm
    Return the greatest common (positive)divisor of a and b
    Chap.6
    */
    template<typename Integer>
    Integer gcd(Integer a, Integer b, Integer* X, Integer* Y) {

        Integer x, y, d;

        if (a == 0 || b == 0) {
            d = std::max(std::abs(a), std::abs(b));
            x = std::min(std::abs(a), 1);
            y = std::min(std::abs(b), 1);
        } else {
            Integer xpp = 1, xp = 0;
            d = details::gcd_recur<Integer>(std::abs(a), std::abs(b), &xpp, &xp);
            x = (std::abs(a) / a) * xp;
            y = (d - x * a) / b;
        }

        if (X)
            *X = x;

        if (Y)
            *Y = y;

        return d;
    }

    template<typename InIt,
        typename OutIt,
        typename Integer = typename std::iterator_traits<InIt>::value_type>
    Integer gcdm(InIt first, InIt last, OutIt output) {

        Integer a = *(first++);
        // 1 is pushed.
        std::list<Integer> temp = { 1 };
        // Call gcd() function on every number along with the previous gcd
        std::for_each(first, last, [&](auto n) {
            Integer x, y;
            a = gcd<Integer>(a, n, &x, &y);
            // TODO
            std::for_each(temp.begin(), temp.end(), [=](Integer& xx) {
                xx = xx * x;
            });
            temp.push_back(y);
        });

        std::for_each(temp.cbegin(), temp.cend(), [&](auto n) {
            output = n;
        });

        return a;
    }
    /*
    Linear equation. If parameter positive is true then *ptrX is always positive.
    Return the greatest common divisor of a and b; 0 when there is no solution to ax+by=c and the value where pointers point to remains the same.
    Chap.6
    */
    template<typename Integer>
    Integer lieq(Integer a, Integer b, Integer c, Integer* X = NULL, Integer* Y = NULL, bool positive = false) {

        Integer x, g = gcd<Integer>(a, b, &x, NULL);

        if (c%g != 0)  // No solution
            return 0;

        x *= c / g;

        if (positive && x < 0)
            x = ((x % b) + b) % b;

        Integer y = (c - a * x) / b;

        if (ptrX != NULL)
            *ptrX = x;

        if (ptrY != NULL)
            *ptrY = y;

        return g;
    }

    template<typename InIt,
        typename Integer = typename std::iterator_traits<InIt>::value_type>
    Integer lieq(InIt first, InIt last, Integer c) {
        Integer g = gcdm<InIt, Integer>(first, last);
        return c%g == 0 ? g : 0;
    }

    template<typename InIt,
        typename OutIt,
        typename Integer = typename std::iterator_traits<InIt>::value_type>
    Integer lieq(InIt first, InIt last, Integer c, OutIt output) {

        std::list<Integer> xxlist;

        Integer g = gcdm<InIt, OutIt, Integer>(first, last, std::back_inserter(xxlist)); // Compute xx and gcd

        if (c%g != 0) // No solution
            return 0;

        std::for_each(xxlist.begin(), xxlist.end(), [=](auto xx) {
            output = xx * (c / g);
        });

        return g;
    }



    // (prime, power) pair
    template<typename Integer>
    using FactorPowerPair = std::pair<Integer, Integer>;
    /*
    Factorization.
    Return the number of distinct primes dividing the given number
    Chap.7
    */
    template<typename OutIt,
        typename Integer = typename std::iterator_traits<InIt>::value_type>
    Integer factor(Integer number, OutIt pairs) {
        Integer count = 1;

        pairs = FactorPowerPair<Integer>(1, 1);

        Integer primeBound = Integer(sqrt(number));

        for (Integer factor = 2; factor <= number; factor++) {
            Integer power = 0;
            details::highest_power(number, factor, &power, &number);

            if (power > 0) {
                pairs = FactorPowerPair<Integer>(factor, power);
                count++;
            }
            /*
            * If a number is prime, add itself as the last factor
            */
            if (count == 1 && factor >= primeBound) {
                pairs = FactorPowerPair<Integer>(number, 1);
                break;
            }

        }
        return count;
    }
    /*
    Linear congruence. If parameter postive is true, All solutions are positive.
    Return the number of solutions of ax=c (mod m)
    Chap.8
    */
    template<typename OutIt, typename Integer = long>
    Integer licong(Integer a, Integer c, Integer m, OutIt output, bool positive = false) {

        Integer x, g = lieq(a, m, c, &x, NULL, positive);
        if (g == 0)
            return 0;
        Integer times = std::abs(g);
        for (Integer t = 0; t < times; t++) {
            output = x + t * (m / g);
        }

        return times;
    }
    /*
    Successive squaring
    Chap.16
    */
    template<typename Integer>
    Integer ssqrt(Integer number, Integer exponent, Integer modulus) {

        Integer base = std::abs(number);
        Integer powers = base, result = 1;

        while (exponent != 0) {

            if (exponent & 1) {
                result *= powers;
                result %= modulus;
            }

            powers *= powers;
            powers %= modulus;

            exponent = (exponent >> 1);
        }
        return result;
    }
    /*
    Euler's phi function.
    Return the count of integers that are relatively prime to n
    */
    template<typename Integer> Integer phi(Integer number) {

        std::list<FactorPowerPair<Integer>> factors;
        factor(number, std::back_inserter(factors));

        Integer ret = number;
        std::for_each(factors.cbegin(), factors.cend(), [&](auto pair) {
            ret *= (pair.first - 1);
            ret /= pair.first;
        });

        return ret;
    }
    /*
    Euler's sigma function.
    Return the sum of proper divisors of n.
    */
    template<typename Integer> Integer sigma(Integer number) {
        return Integer();
    }






    // implementations
    namespace details {

        template<typename Integer> Integer gcd_recur(Integer a, Integer b) {
            Integer rem = a % b;
            return rem == 0 ? b : gcd_recur<Integer>(b, rem);
        }

        template<typename Integer>
        Integer gcd_recur(Integer a, Integer b, Integer* xpp, Integer* xp) {
            ldiv_t res = ldiv(a, b);
            if (res.rem == 0)
                return b;

            *xpp = *xpp - res.quot * (*xp);
            std::swap(*xpp, *xp);
            return gcd_recur<Integer>(b, res.rem, xpp, xp);
        }


        template<typename Integer>
        void highest_power(Integer number, Integer base, Integer* exp, Integer* rem) {

            Integer power = 0;
            ldiv_t res;

            do {
                res = std::ldiv(number, base);
                if (res.rem == 0)
                    power++;
                else
                    break;
                number = res.quot;
            } while (true);

            if (exp)
                *exp = power;
            if (rem)
                *rem = number;
        }

    }

}
}