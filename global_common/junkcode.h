#ifndef TALESTARS_V63_322_JUNKCODE_H
#define TALESTARS_V63_322_JUNKCODE_H

//
// converted and obfuscated to C (strings replaced with hash-like decoys)
// created from original: johan 25.07.25
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* simple xorshift32 RNG used as a replacement for mt19937 */
static inline __attribute__((always_inline)) uint32_t rng_xor32(void) {
    /* function-local static seed */
    static uint32_t s;
    if (s == 0) {
        /* mix time and address to seed */
        uint32_t t = (uint32_t)time(NULL);
        uintptr_t a = (uintptr_t)&s;
        s = t ^ (uint32_t)(a >> 4) ^ 0x9e3779b9u;
        if (s == 0) s = 0xdeadbeefu;
    }
    /* xorshift32 */
    uint32_t x = s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s = x;
    return x;
}

/* produce an integer in [lo, hi] */
static inline __attribute__((always_inline)) int rng_range(int lo, int hi) {
    if (lo >= hi) return lo;
    uint32_t r = rng_xor32();
    uint32_t span = (uint32_t)(hi - lo + 1);
    return lo + (int)(r % span);
}

/* All strings converted to decoy "hash-like" literals */
static const char *DEC_STR_01 = "5d8f3a7c9b1e4f2a6c0d7e8b1a9c2f4d6b3e0a7f";
static const char *DEC_STR_02 = "8c7d5b2f1e3a9c0d4b6f2e7a1d3c8b5f0a9e6d2";
static const char *DEC_STR_03 = "f2a9c8b1e4d7f0a3c6b5e2d9f1a4c8b3e7d0f5a";
static const char *DEC_STR_04 = "c9b3e5f1a7d2c8b0f4e6a1d9c3b5f7e0a2d4c6b";
static const char *DEC_STR_05 = "1a2f3c4b5d6e7f8091a2b3c4d5e6f708192a3b4";
static const char *DEC_STR_06 = "7e5f2a1c3b4d6f8a9c0d1b2e3f4a5c6d7b8e9f0";
static const char *DEC_STR_07 = "dab4c6e8f1a2b3d4c5e6f7098a1b2c3d4e5f607";
static const char *DEC_STR_08 = "9c8b7a6d5e4f3b2a1c0d9e8f7a6b5c4d3e2f1a0";
static const char *DEC_STR_09 = "0f1e2d3c4b5a69788796a5b4c3d2e1f0a1b2c3d";
static const char *DEC_STR_10 = "b1c2d3e4f5a6b7c8d9e0f1a2b3c4d5e6f7a8b9c";

/* jittery junk functions (inline for potential inlining / optimization prevention by volatile) */

static inline __attribute__((always_inline)) void jcode01(void) {
    volatile int hvpkfmipm = 5989;
    volatile double fucftviqesvg = 23501.0;
    volatile bool vmoomecqbw = false;
    volatile double fhxjymoqgdc = 4162.0;
    volatile double pbmamhhxwyc = 11229.0;
    volatile double tzeskwfvvvtsisf = 13344.0;
    volatile int dawpgww = 3844;
    volatile double styxhj = 23390.0;

    (void)hvpkfmipm; (void)fucftviqesvg; (void)vmoomecqbw;
    (void)fhxjymoqgdc; (void)pbmamhhxwyc; (void)tzeskwfvvvtsisf;
    (void)dawpgww; (void)styxhj;

    for (int i = 60; i > 0; i--) { continue; }
    for (int i = 40; i > 0; i--) { continue; }
    for (int i = 48; i > 0; i--) { continue; }
    for (int i = 48; i > 0; i--) { continue; }

    volatile int junk_rand = rng_range(1, 1000);
    junk_rand *= 3;
    (void)junk_rand;
}

static inline __attribute__((always_inline)) void jcode02(void) {
    volatile const char *qajmdf = DEC_STR_01;
    volatile const char *pwqygzcxc = DEC_STR_02;
    volatile double qtjbt = 3911.0;
    volatile int hurezugyooilifx = 4065;
    volatile bool ksalmongiu = false;
    volatile int cibiz = 6509;
    volatile const char *tftbinhsi = DEC_STR_03;
    volatile double qbsly = 51733.0;
    volatile double dkhrlxhzgjnuz = 1453.0;

    (void)qajmdf; (void)pwqygzcxc; (void)qtjbt; (void)hurezugyooilifx;
    (void)ksalmongiu; (void)cibiz; (void)tftbinhsi; (void)qbsly; (void)dkhrlxhzgjnuz;

    for (int i = 30; i > 0; i--) { continue; }

    volatile int junk_rand = rng_range(10, 500);
    junk_rand += 42;
    (void)junk_rand;
}

static inline __attribute__((always_inline)) void jcode03(void) {
    volatile const char *fsercapuzhmiksd = DEC_STR_04;
    volatile int mdvor = 6735;
    volatile int ectvqddbfanl = 2372;
    volatile bool daoxzyvuq = false;
    volatile double hznphwwom = 12465.0;
    volatile const char *zvfge = DEC_STR_05;
    volatile bool kckiekgpnsc = true;
    volatile double evadkofeffbw = 57785.0;

    (void)fsercapuzhmiksd; (void)mdvor; (void)ectvqddbfanl; (void)daoxzyvuq;
    (void)hznphwwom; (void)zvfge; (void)kckiekgpnsc; (void)evadkofeffbw;

    for (int i = 64; i > 0; i--) { continue; }
    for (int i = 87; i > 0; i--) { continue; }
    for (int i = 46; i > 0; i--) { continue; }
    for (int i = 50; i > 0; i--) { continue; }
    for (int i = 85; i > 0; i--) { continue; }

    volatile int junk_rand = rng_range(0, 9999);
    junk_rand -= 100;
    (void)junk_rand;
}

static inline __attribute__((always_inline)) void jcode04(void) {
    for (int i = 100; i > 0; i--) { continue; }

    volatile int junk_rand = (int)rng_xor32();
    junk_rand ^= (int)0xdeadbeef;
    (void)junk_rand;
}

static inline __attribute__((always_inline)) void jcode05(void) {
    volatile double a = 3.14;
    volatile double b = 2.71;
    volatile double c = a + b;
    for (int i = 50; i > 0; i--) { continue; }

    volatile int junk_rand = rng_range(0, 100);
    junk_rand += (int)c;
    (void)junk_rand;
}

static inline __attribute__((always_inline)) void jcode06(void) {
    volatile int x = 12345;
    volatile bool flag = true;
    volatile double y = 3.14159;
    volatile const char *s = DEC_STR_06;
    for (int i = 20; i > 0; i--) { __asm volatile("":: : "memory"); }
    for (int i = 15; i > 0; i--) { continue; }

    volatile int r = rng_range(5, 5000);
    r ^= x;
    if (flag && r > 1000) {
        volatile int z = r / 3;
        (void)z;
    }
    (void)y; (void)s;
}

static inline __attribute__((always_inline)) void jcode07(void) {
    const char *a = "7d4f2b9c5e1a8f3d0b6c4e7a9d2f1b0c";
    const char *b = "c8a3f1e7b5d0c9a2f4b6e3d1a7c0b5e8";
    volatile int m = 777;
    volatile int n = 555;
    volatile bool bval = false;

    (void)a; (void)b; (void)m; (void)n; (void)bval;

    for (int i = 10; i > 0; i--) {
        if (a != b) {
            __asm volatile("":: : "memory");
            continue;
        }
    }

    volatile int junk = rng_range(1, 100);
    junk += m - n;
    (void)junk;
}

static inline __attribute__((always_inline)) void jcode08(void) {
    volatile double p = 1.6180339;
    volatile double q = 2.71828;
    volatile double r = p * q;
    volatile bool condition = (r > 4.4);

    (void)p; (void)q; (void)r;

    for (int i = 0; i < 25; i++) {
        __asm volatile("":: : "memory");
    }

    if (condition) {
        volatile int x = 0;
        for (; x < 15; x++) {
            __asm volatile("":: : "memory");
        }
    }

    volatile int val = rng_range(100, 1000);
    val -= 50;
    (void)val;
}

static inline __attribute__((always_inline)) void jcode09(void) {
    volatile int a = (int)0xdeadbeef;
    volatile int b = (int)0xcafebabe;
    volatile bool flag = (a == b);

    (void)b;

    for (int i = 30; i > 0; i--) {
        __asm volatile("":: : "memory");
    }

    if (!flag) {
        for (int j = 5; j > 0; j--) {
            __asm volatile("":: : "memory");
        }
    }

    volatile int random_num = rng_range(0, 10000);
    random_num ^= a;
    (void)random_num;
}

static inline __attribute__((always_inline)) void jcode10(void) {
    volatile const char *s1 = DEC_STR_07;
    volatile const char *s2 = DEC_STR_08;
    volatile int val = 1024;

    (void)s1; (void)s2;

    for (int i = 0; i < 20; i++) {
        __asm volatile("":: : "memory");
    }

    if (s1 != s2) {
        for (int k = 10; k > 0; k--) {
            __asm volatile("":: : "memory");
        }
    }

    volatile int rand_val = rng_range(500, 1500);
    rand_val += val;
    (void)rand_val;
}

/* free function mirroring junk001 from the original, returns int */
inline __attribute__((always_inline)) int junk001(void) {
    volatile const char *fsercapuzhmiksd = DEC_STR_04;
    volatile int mdvor = 6735;
    volatile int ectvqddbfanl = 2372;
    volatile bool daoxzyvuq = false;
    volatile double hznphwwom = 12465.0;
    volatile const char *zvfge = DEC_STR_05;
    volatile bool kckiekgpnsc = true;
    volatile double evadkofeffbw = 57785.0;

    (void)fsercapuzhmiksd; (void)mdvor; (void)ectvqddbfanl; (void)daoxzyvuq;
    (void)hznphwwom; (void)zvfge; (void)kckiekgpnsc; (void)evadkofeffbw;

    /* keep original conditional loop shapes but make them no-ops */
    if (zvfge == DEC_STR_05) {
        volatile int hucoa;
        for (hucoa = 64; hucoa > 0; hucoa--) {
            __asm volatile("":: : "memory");
            continue;
        }
    }
    if (true != true) {
        volatile int lhgvared;
        for (lhgvared = 87; lhgvared > 0; lhgvared--) {
            __asm volatile("":: : "memory");
            continue;
        }
    }
    if ((int)57785 == 57785) {
        volatile int adumgohgtk;
        for (adumgohgtk = 46; adumgohgtk > 0; adumgohgtk--) {
            __asm volatile("":: : "memory");
            continue;
        }
    }
    if (false) {
        volatile int sfulmve;
        for (sfulmve = 50; sfulmve > 0; sfulmve--) {
            __asm volatile("":: : "memory");
            continue;
        }
    }
    if (zvfge && zvfge == DEC_STR_05) {
        volatile int ajapdltp;
        for (ajapdltp = 85; ajapdltp > 0; ajapdltp--) {
            __asm volatile("":: : "memory");
            continue;
        }
    }

    return 33978;
}

#endif //TALESTARS_V63_322_JUNKCODE_H
