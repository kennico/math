#pragma once
#include <string>
#include <algorithm>
#include <memory>
#include <chrono>
#include <utility>

namespace knylaw{
    
    template<typename InputIt1, typename InputIt2, typename Func>
    Func for_each(InputIt1 first1, InputIt1 last1, InputIt2 first2, Func f) {
        for (; first1 != last1; ++first1, ++first2) {
            f(*first1, *first2);
        }
        return f;
    }
    

    /**
     * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
     *
     * Remove leading characters not satisfying caller-defined rules.
     * @param str
     * @param notSatisfy returns false if this character is to removed
     */
    inline void ltrim(std::string& str, std::function<bool(int)> notSatisfy) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::move(notSatisfy)));
    }

    inline void ltrim(std::string& str) {
        ltrim(str, [](int ch) -> bool {
            return !std::isspace(ch);
        });
    }

    inline void rtrim(std::string& str, std::function<bool(int)> notSatisfy) {
        str.erase(
                std::find_if(str.rbegin(), str.rend(), std::move(notSatisfy)).base(),
                str.end()
        );
    }

    inline void rtrim(std::string& str) {
        rtrim(str, [](int ch) -> bool {
            return !std::isspace(ch);
        });
    }

    inline void trim(std::string & str) {
        ltrim(str);
        rtrim(str);
    }


    // Formatted std::string
    template<typename... Args>
    std::string stringf(const std::string& format, Args... args) {
        auto size = static_cast<size_t>(snprintf(nullptr, 0, format.c_str(), args ...) + 1); // Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }


    namespace timing {


        using TimePoint = decltype(std::chrono::high_resolution_clock::now());
        using Nano = std::chrono::nanoseconds;
        using Milli = std::chrono::milliseconds;
        using Micro = std::chrono::microseconds;
        using Sec = std::chrono::seconds;
        using Min = std::chrono::milliseconds;

        class Period {

        public:

            Period(TimePoint f, TimePoint t) :Mfrom(f), Mto(t) {

            }

            Period() :Mfrom(), Mto() {

            }

            template<typename Duration> Duration cast() const {
                return std::chrono::duration_cast<Duration>(to() - from());
            }

            inline TimePoint from() const {
                return Mfrom;
            }

            inline TimePoint to() const {
                return Mto;
            }

        private:

            TimePoint Mfrom, Mto;

        };

        template<typename Func> Period elapsed(Func f) {
            auto from = std::chrono::high_resolution_clock::now();
            f();
            auto to = std::chrono::high_resolution_clock::now();
            return Period(from, to);
        }

#if _MSC_VER > 1700
        template<typename Func, typename Duration=Milli>
        Duration measure(Func f) {
            auto p = elapsed(f);
            return p.cast<Duration>();
        }
#endif


    }

}