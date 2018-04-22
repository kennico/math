#pragma once
#include <string>
#include <memory>
#include <chrono>


namespace knylaw{
    
    template<typename InputIt1, typename InputIt2, typename Func>
    Func for_each(InputIt1 first1, InputIt1 last1, InputIt2 first2, Func f) {
        for (; first1 != last1; ++first1, ++first2) {
            f(*first1, *first2);
        }
        return f;
    }
    //template<typename InIt, typename Func>
    //Func for_each_step(InIt first, InIt last, size_t step, Func f) {        
    //}

    // Formatted std::string
    template<typename... Args>
    std::string stringf(const std::string& format, Args... args) {
        size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

    // For class instance that needs a name
    class NameUtility {

    public:

        NameUtility(std::string name=""): Mname(name) {

        }

        ~NameUtility() {

        }

        inline std::string name() const {
            return Mname;
        }

        inline void name(std::string n) {
            this->Mname = n;
        }

    private:

        std::string     Mname;
    };


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

        template<typename Func, typename Duration=Milli>
        Duration measure(Func f) {
            auto p = elapsed(f);
            return p.cast<Duration>();
        }
    }
    

}