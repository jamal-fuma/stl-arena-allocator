#ifndef LIB_FUMA_DARWIN_TIMER_HPP
#define LIB_FUMA_DARWIN_TIMER_HPP

#include <new>
#include <iostream>
#include <iomanip>

#include <boost/current_function.hpp>

#include <time.h>
#include <sys/time.h>

#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

struct TimeBase
{
	TimeBase()
	{
		m_timebase.numer = 0;
		m_timebase.denom = 0;
		mach_timebase_info(&m_timebase);
	}
	~TimeBase() = default;

    uint64_t tick() const {
		return mach_absolute_time();
    }

	uint64_t to_nanoseconds(uint64_t ticks) const
	{
		return ticks * (uint64_t)m_timebase.numer / (uint64_t)m_timebase.denom;
	}

	mach_timebase_info_data_t m_timebase;
};

struct TimeSpec
{
	TimeSpec(uint64_t ns_diff)
	{
		reset(ns_diff);
	}

	void reset(uint64_t ns_diff)
	{
		m_tv_sec  = ns_diff / 1000000000UL;
		m_tv_nsec = ns_diff % 1000000000UL;

	}
	~TimeSpec() = default;

	uint64_t m_tv_sec;
	uint64_t m_tv_nsec;
};

struct HiResTimer
{
	HiResTimer()
        : m_timebase{}
		, m_ns_first_tick(m_timebase.tick())
	{
	}
	~HiResTimer() = default;

	uint64_t sample() const
	{
		// sample monatonic clock
		uint64_t ns_start = m_timebase.tick() - m_ns_first_tick;

		// convert to adjusted nanoseconds
		return m_timebase.to_nanoseconds(ns_start);
	}

	TimeBase m_timebase;
	uint64_t m_ns_first_tick;
};

template <typename T, class ...Args>
uint64_t profile_op(
        HiResTimer & timer,
        uint64_t rounds,
        const std::function<T> & operation,
        Args && ...args)
{
    // initialisation
	uint64_t total    = 0,
             ns_start = 0,
             ns_end   = 0;

	for(uint64_t iterations=0; iterations != rounds; ++iterations)
	{
		ns_start = timer.sample();
        operation(args...);
		ns_end = timer.sample();

		total += (ns_end - ns_start);
	}

	total /= rounds;

    TimeSpec ts(total);
    double nanoseconds  = static_cast<double>(ts.m_tv_sec * (1000 * 1000 * 1000));
           nanoseconds  += static_cast<double>(ts.m_tv_nsec);

    double seconds      = static_cast<double>(ts.m_tv_sec);
           seconds     += static_cast<double>(ts.m_tv_nsec) / (1000 * 1000 * 1000);

    auto write_out = [](const char *str, double si_unit){
        std::cout << str
                  << std::fixed << std::setprecision(10) << si_unit << "\n";
    };

    // read em and weep ;)
    write_out("Timer elapsed sc: ",seconds);
    write_out("Timer elapsed ms: ",seconds / 1000);
    write_out("Timer elapsed mu: ",nanoseconds / 1000);
    write_out("Timer elapsed ns: ",nanoseconds);
	std::cout << "Timer rounds: " << rounds << "\n\n";

	return total;
}


#endif /* ndef LIB_FUMA_DARWIN_TIMER_HPP */
