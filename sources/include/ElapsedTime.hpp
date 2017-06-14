#ifndef LIB_FUMA_ELAPSED_TIME_HPP
#define LIB_FUMA_ELAPSED_TIME_HPP

#include <cstdint>

// helper to format time into Si units without chrono
class ElapsedTime
{
    public:
        // implicitly convertable from uint64_t
        ElapsedTime(uint64_t ns_diff = 0);
        ~ElapsedTime() = default;

        // copyable
        ElapsedTime(const ElapsedTime &rhs) = default;
        ElapsedTime operator=(const ElapsedTime & rhs) = default;

        uint64_t nanoseconds() const;
        double microseconds() const;
        double milliseconds() const;
        double seconds() const;
    private:
        uint64_t m_nanoseconds;
};
#endif /* ndef LIB_FUMA_ELAPSED_TIME_HPP */ 
