/* MIT License
 *
 * Copyright (c) 2021 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CRANDOM_H_INCLUDED
#define CRANDOM_H_INCLUDED

/*
// crandom: Pseudo-random number generator
#include "stc/crandom.h"
int main() {
    uint64_t seed = 123456789;
    stc64_t rng = stc64_init(seed);
    stc64_uniform_t dist1 = stc64_uniform_init(1, 6);
    stc64_uniformf_t dist2 = stc64_uniformf_init(1.0, 10.0);
    stc64_normalf_t dist3 = stc64_normalf_init(1.0, 10.0);

    uint64_t i = stc64_rand(&rng);
    int64_t iu = stc64_uniform(&rng, &dist1);
    double xu = stc64_uniformf(&rng, &dist2);
    double xn = stc64_normalf(&rng, &dist3);
}
*/
#include "ccommon.h"
#include <string.h>
#include <math.h>

typedef struct {uint64_t state[5];}                        stc64_t;
typedef struct {int64_t lower; uint64_t range, threshold;} stc64_uniform_t;
typedef struct {double lower, range;}                      stc64_uniformf_t;
typedef struct {double mean, stddev, next; unsigned has_next;} stc64_normalf_t;

/* PRNG stc64.
 * Extremely fast PRNG suited for parallel usage with Weyl-sequence parameter.
 * 320bit state, 256bit mutable.
 * Noticable faster than xoshiro and pcg, but slighly slower than wyrand64, 
 * which only has a single 64-bit state and is unfit when longer periods or
 * multiple threads are required.
 * stc64 supports 2^63 unique threads with a minimum 2^64 period lengths each.
 * Passes PractRand tested up to 8TB output, Vigna's Hamming weight test,
 * and simple correlation tests, i.e. interleaved streams with one-bit diff state.
 * The 16-bit version with LR=6, RS=5, LS=3 passes PractRand to multiple TB input.
 */

STC_API stc64_t stc64_init(uint64_t seed);
STC_API stc64_t stc64_with_seq(uint64_t seed, uint64_t seq);

STC_INLINE uint64_t stc64_rand(stc64_t* rng) {
    uint64_t *s = rng->state; enum {LR=24, RS=11, LS=3};
    const uint64_t result = (s[0] ^ (s[3] += s[4]|1)) + s[1];
    s[0] = s[1] ^ (s[1] >> RS);
    s[1] = s[2] + (s[2] << LS);
    s[2] = ((s[2] << LR) | (s[2] >> (64 - LR))) + result;
    return result;
}

/* Global random() */
static stc64_t stc64_global = {{0x26aa069ea2fb1a4d, 0x70c72c95cd592d04, 0x504f333d3aa0b359, 0x26aa069ea2fb1a4d, 0x6a09e667a754166b}};
STC_INLINE void stc64_srandom(uint64_t seed) { stc64_global = stc64_init(seed); }
STC_INLINE uint64_t stc64_random(void) { return stc64_rand(&stc64_global); }

/* Float64 random number in range [0.0, 1.0). */
STC_INLINE double stc64_randf(stc64_t* rng) {
    union {uint64_t i; double f;} u = {0x3FF0000000000000ull | (stc64_rand(rng) >> 12)};
    return u.f - 1.0;
}

/* Int64 uniform distributed RNG, range [low, high]. */
STC_API stc64_uniform_t stc64_uniform_init(int64_t low, int64_t high);

/* Float64 uniform distributed RNG, range [low, high). */
STC_INLINE stc64_uniformf_t stc64_uniformf_init(double low, double high) {
    return c_make(stc64_uniformf_t){low, high - low};
}
STC_INLINE double stc64_uniformf(stc64_t* rng, stc64_uniformf_t* dist) {
    return stc64_randf(rng)*dist->range + dist->lower;
}

/* Unbiased bounded uniform distribution. */
STC_INLINE int64_t stc64_uniform(stc64_t* rng, stc64_uniform_t* d) {
    uint64_t lo, hi;
#ifdef c_umul128
    do { c_umul128(stc64_rand(rng), d->range, &lo, &hi); } while (lo < d->threshold);
#else
    hi = stc64_rand(rng) % d->range;
#endif
    return d->lower + hi;
}

/* Normal distributed RNG, Float64. */
STC_INLINE stc64_normalf_t stc64_normalf_init(double mean, double stddev) {
    return c_make(stc64_normalf_t){mean, stddev, 0.0, 0};
}
STC_API double stc64_normalf(stc64_t* rng, stc64_normalf_t* dist);


#if !defined(STC_HEADER) || defined(STC_IMPLEMENTATION)

STC_DEF stc64_t stc64_init(uint64_t seed) {
    return stc64_with_seq(seed, seed + 0x3504f333d3aa0b34);
}
STC_DEF stc64_t stc64_with_seq(uint64_t seed, uint64_t seq) {
    stc64_t rng = {{seed, seed, seed, seed, (seq << 1) | 1}};
    for (int i = 0; i < 12; ++i) stc64_rand(&rng);
    return rng;
}

/* Very fast unbiased uniform int RNG with bounds [low, high] */
STC_DEF stc64_uniform_t stc64_uniform_init(int64_t low, int64_t high) {
    stc64_uniform_t dist = {low, (uint64_t) (high - low + 1)};
    dist.threshold = (uint64_t)(-dist.range) % dist.range;
    return dist;
}

/* Marsaglia polar method for gaussian/normal distribution. */
STC_DEF double stc64_normalf(stc64_t* rng, stc64_normalf_t* dist) {
    double u1, u2, s, m;
    if (dist->has_next++ & 1)
        return dist->next * dist->stddev + dist->mean;
    do {
        u1 = 2.0 * stc64_randf(rng) - 1.0;
        u2 = 2.0 * stc64_randf(rng) - 1.0;
        s = u1*u1 + u2*u2;
    } while (s >= 1.0 || s == 0.0);
    m = sqrt(-2.0 * log(s) / s);
    dist->next = u2 * m;
    return (u1 * m) * dist->stddev + dist->mean;
}

#endif
#endif
