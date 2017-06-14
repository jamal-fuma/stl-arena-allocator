
void accum_sample( Result<uint64_t> & result, const ChronoClockSteady::time_type & begin, const ChronoClockSteady::time_type & end, uint64_t iterations)
{
    // find variance with integer only variant
    uint64_t sample = ChronoClockSteady::clock_type::nanoseconds(begin,end);
    m_mean     += sample;
    m_variance += (sample * sample);
}

void accum_sample( Result<uint64_t> & result, const uint64_t & begin, const uint64_t & end, uint64_t iterations)
{
    uint64_t sample = (end - begin);
    // find variance with integer only variant
    m_mean     += sample;
    m_variance += (sample * sample);
}

void accum_sample( Result<double> & result, const uint64_t & begin, const uint64_t & end, uint64_t iterations)
{
    // find variance with knuths algorithym
    double sample = static_cast<double>(end - begin);
    double delta  = sample - m_mean;
    result.m_mean        += delta / (iterations + 1);
    result.m_variance    += delta * (sample - result.m_mean);
}

void commit_samples( Result<double> & result, uint64_t rounds)
{
    result.m_variance /= (rounds-1);
}

void commit_samples( Result<uint64_t> & result, uint64_t rounds)
{
    // std deviation integer only
    m_mean /= rounds;
    m_variance = ((rounds * m_variance) - (m_mean * m_mean));
}
