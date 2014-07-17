#include <cassert>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <utility>

#include <iostream>

#include "xorshift64star.hpp"

static uint64_t find_period(uint64_t N, std::ostream& os)
{
    constexpr uint32_t BILLION = 1024U*1024U*1024U;

    OTI::xorshift64star rng;
    OTI::xorshift64star::seed_type the_seed = rng.seed();

    N /= BILLION; // number of billions

    for( uint64_t i = 0; i != N; ++i )
    {
        for( uint32_t k = 0U; k != BILLION; ++k)
        {
            rng.sample();
            if (rng.seed() == the_seed)
            {
                return i*BILLION + uint64_t(k);
            }
        }
        os << "Passed: " << i << std::endl;
    }
    // skip the leftover...

    return 0ULL;
}

static inline std::pair<float,float> kahan_summation(std::pair<float,float> s, float r)
{
    float y = r - s.second;
    float t = s.first + y;

    return std::pair<float,float>{t, (t - s.first) - y};
}

static std::pair<float,float> test_mean_sigma(uint64_t n)
{
    std::pair<float,float> s{0.0, 0.0};
    std::pair<float,float> d{0.0, 0.0};

    OTI::xorshift64star rng;

    for(uint64_t k = 0; k != n; ++k)
    {
        float r = rng.number();

        s = kahan_summation(s, r);
        d = kahan_summation(d, r*r);
    }

    s.first /= float(n);
    d.first /= float(n);

    d.first = sqrt(d.first);
    d.first = (d.first - s.first)*(d.first + s.first);
    if (d.first < 0.0f)
        d.first = 0.0f;

    return std::pair<float,float>{s.first, d.first};
}

static bool test_skip_ahead(int64_t ns)
{
    assert( ns > 0LL);

    OTI::xorshift64star rng;
    OTI::xorshift64star rngs{rng};

    rngs.skip(ns);

    // manual skip ahead
    for(int64_t k = 0; k != ns; ++k)
    {
        rng.sample();
    }

    // compare manual and fast skip
    return (rng.seed() == rngs.seed());
}

static bool test_skip_zero()
{
    OTI::xorshift64star rng;

    OTI::xorshift64star::seed_type the_seed = rng.seed();

    rng.skip(0LL);

    return (rng.seed() == the_seed);
}


int main(int argc, char* argv[])
{
    OTI::xorshift64star::result_type period = find_period(2500000000ULL, std::cout); 
    std::cout << "Found period: " << period << std::endl;

    std::pair<float,float> r = test_mean_sigma(50000000ULL);
    std::cout << "Mean, Sigma " << r.first*2.0f << " " << r.second*12.0f << std::endl;

    bool q;

    q = test_skip_ahead(777777LL);
    assert(q);

    test_skip_zero();
    assert(q);

    return 0;
}
