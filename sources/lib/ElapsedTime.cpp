#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ElapsedTime.hpp"

ElapsedTime::ElapsedTime(uint64_t nanoseconds)
    : m_nanoseconds{nanoseconds}
{
}

uint64_t ElapsedTime::nanoseconds() const
{
    return m_nanoseconds;
}

double ElapsedTime::microseconds() const
{
    return static_cast<double>(m_nanoseconds) / 1000;
}
double ElapsedTime::milliseconds() const
{
    return static_cast<double>(m_nanoseconds) / (1000000);
}

double ElapsedTime::seconds() const
{
    return static_cast<double>(m_nanoseconds) / (1000000000);
}
