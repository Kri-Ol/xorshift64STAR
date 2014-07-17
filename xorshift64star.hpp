// -*- C++ -*-

#pragma once

#include <cstdint>
#include <utility>

namespace OTI
{
    class xorshift64star
    {
#pragma region Typedefs/Constants
        public: typedef uint_fast64_t result_type;
        public: typedef result_type   seed_type;

        // those values are from S.Vigna "An experimental exploration of Marsaglia's xorshift generators, scrambled",
        // http://arxiv.org/abs/1402.6246, known as M(12,25,27) with scrambling
        public: static constexpr uint32_t shift_a = 12U;
        public: static constexpr uint32_t shift_b = 25U;
        public: static constexpr uint32_t shift_c = 27U;

        public: static constexpr result_type  mult  = 2685821657736338717ULL;

        public: static constexpr seed_type    default_seed   = 1ULL;        

        public: static constexpr float        norm  = float{double{1.0}/double{result_type{-1LL}}};
#pragma endregion

#pragma region Data
        private: mutable seed_type _seed;
#pragma endregion

#pragma region Ctor/Dtor/op=
        public: xorshift64star(seed_type seed = default_seed):
            _seed{seed}
        {
        }

        public: xorshift64star(xorshift64star const& r):
            _seed{r._seed}
        {
        }

        public: xorshift64star(xorshift64star&& r):
            _seed{std::move(r._seed)}
        {
        }        

        public: xorshift64star& operator=(xorshift64star const& r)
        {
            _seed = r._seed;
            return *this;
        }

        public: xorshift64star& operator=(xorshift64star&& r)
        {
            _seed = std::move(r._seed);
            return *this;
        }

        public: ~xorshift64star()
        {
        }
#pragma endregion

#pragma region Observers
        public: seed_type seed() const
        {
            return _seed;
        }
#pragma endregion

#pragma region Helpers
        public: static seed_type sample(seed_type seed)
        {
            seed ^= seed >> shift_a;
            seed ^= seed << shift_b;
            seed ^= seed >> shift_c;

            return mult * seed;
        }
#pragma endregion

#pragma region Mutators
        public: void sample() const
        {
            _seed = sample(_seed);
        }

        public: float number() const
        {
            sample();
            return float(_seed) * norm;
        }

        public: void skip(uint64_t ns) const;
#pragma endregion
    };    
}
