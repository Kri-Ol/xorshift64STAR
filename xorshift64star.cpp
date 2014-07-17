// -*- C++ -*-

#include "xorshift64star.hpp"

namespace OTI
{
    void xorshift64star::skip(uint64_t ns) const
    {
        while (ns)
        {
            sample();
            --ns;
        }
    }
}
