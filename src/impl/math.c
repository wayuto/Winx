#include "math.h"

#include "stdint.h"

uint64_t divide_u64_u32(uint64_t dividend, uint32_t divisor,
                        uint64_t *remainder) {
    if (divisor == 0) {
        return 0;
    }

    uint64_t quotient = 0;

    for (int i = 63; i >= 0; i--) {
        *remainder <<= 1;

        uint64_t bit = (dividend >> i) & 1;

        *remainder |= bit;

        if (*remainder >= divisor) {
            *remainder -= divisor;
            quotient |= (1ULL << i);
        }
    }

    return quotient;
}

uint64_t divide_u64_u32_no_mod(uint64_t dividend, uint32_t divisor) {
    if (divisor == 0) {
        return 0;
    }

    uint64_t quotient = 0;

    uint64_t remainder = 0;

    for (int i = 63; i >= 0; i--) {
        remainder <<= 1;

        uint64_t bit = (dividend >> i) & 1;

        remainder |= bit;

        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= (1ULL << i);
        }
    }
    return quotient;
}

uint64_t mod_u64_u32(uint64_t dividend, uint32_t divisor) {
    if (divisor == 0) {
        return 0;
    }
    uint64_t remainder = 0;

    uint64_t quotient = 0;

    for (int i = 63; i >= 0; i--) {
        remainder <<= 1;

        uint64_t bit = (dividend >> i) & 1;

        remainder |= bit;

        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= (1ULL << i);
        }
    }
    return remainder;
}

uint64_t div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder) {
    union {
        uint64_t v64;
        uint32_t v32[2];
    } d = {dividend};
    uint32_t upper;

    upper = d.v32[1];
    d.v32[1] = 0;

    if (upper >= divisor) {
        d.v32[1] = upper / divisor;
        upper %= divisor;
    }

    asm("divl %2"
        : "=a"(d.v32[0]), "=d"(*remainder)
        : "rm"(divisor), "0"(d.v32[0]), "1"(upper));

    return d.v64;
}
