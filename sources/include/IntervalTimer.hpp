#ifndef LIB_FUMA_INTERVAL_TIMER_HPP
#define LIB_FUMA_INTERVAL_TIMER_HPP

#include <chrono>
#include <cstddef>

// helper to format time into Si units with chrono
template <typename ClockT>
struct IntervalTimer
{
    using clock_type       = ClockT;
    using time_type        = std::chrono::time_point<ClockT>; 
    using nanosecond_diff  = std::chrono::duration_cast<std::chrono::nanoseconds>;
    using microsecond_diff = std::chrono::duration_cast<std::chrono::microseconds>;
    using milisecond_diff  = std::chrono::duration_cast<std::chrono::milliseconds>;
    using second_diff      = std::chrono::duration_cast<std::chrono::seconds>;

    template <typename PrecisionT, typename T>
	static T duration_cast(time_type begin, time_type end=now())
    {
        return static_cast<T>(
                std::chrono::duration_cast<PrecisionT>(end - begin).count()
        );
    }

    static uint64_t nanoseconds(time_type begin, time_type end=now())
    {
        return duration_cast<std::chrono::nanoseconds>(begin,end);
    }
    static double microseconds() (time_type begin, time_type end=now())
    {
        return duration_cast<std::chrono::microseconds>(begin,end);
    }
    static double milliseconds() (time_type begin, time_type end=now())
    {
        return duration_cast<std::chrono::milliseconds>(begin,end);
    }
    static double seconds() (time_type begin, time_type end=now())
    {
        return duration_cast<std::chrono::seconds>(begin,end);
    }

    static time_type now()
    {
        return ClockT::now()
    }
};

template <typename T>
struct Result
{
    T  m_mean;
    T  m_variance;
};

void accum_sample( Result<uint64_t> & result, const ChronoClockSteady::time_type & begin, const ChronoClockSteady::time_type & end, uint64_t iterations);
void accum_sample( Result<uint64_t> & result, const uint64_t & begin, const uint64_t & end, uint64_t iterations);
void accum_sample( Result<double> & result, const uint64_t & begin, const uint64_t & end, uint64_t iterations);
void commit_samples( Result<double> & result, uint64_t rounds);
void commit_samples( Result<uint64_t> & result, uint64_t rounds);

template <typename T,typename FuncT, class ...Args>
void profile_op(
        Result<T> & result,
        TimerT & timer,
        uint64_t rounds, 
        const std::function<FuncT> & operation, 
        Args && ...args)
{
    // initialisation
    auto ns_first = timer.now();
    auto ns_start = ns_first;
    auto ns_end   = ns_first;
    
    // run and average out the runtimes, the more rounds, the more accurate the measure
   	for(uint64_t iterations=0; iterations != rounds; ++iterations)
	{
        // relative ticks since boot up
		ns_start = timer.now();
        operation(args...);
		ns_end = timer.now();
        
        // convert to absolute duration prior to accumulating mean
        ns_start -= ns_first;

        // make stats pluggable
        accum_sample(result,ns_start,ns_end,iterations);
    }
    commit_samples(result,rounds);
}

#endif /* ndef LIB_FUMA_INTERVAL_TIMER_HPP */ 
