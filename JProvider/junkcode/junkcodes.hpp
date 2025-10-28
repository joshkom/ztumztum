//
// Created by johan on 25.07.25.
//

#ifndef BSWRAPPER_JUNKCODES_HPP
#define BSWRAPPER_JUNKCODES_HPP

#include <atomic>
#include <stdio.h>

#include <stdio.h>
#include <string>
#include <iostream>

#include <string>
#include <random>

namespace jcode {
    static inline std::mt19937 &get_mt() {
        // thread_local for safety in multithreaded env
        thread_local static std::mt19937 mt{std::random_device{}()};
        return mt;
    }

    static inline __attribute__((always_inline)) void jcode01() {
        volatile int hvpkfmipm = 5989;
        volatile double fucftviqesvg = 23501;
        volatile bool vmoomecqbw = false;
        volatile double fhxjymoqgdc = 4162;
        volatile double pbmamhhxwyc = 11229;
        volatile double tzeskwfvvvtsisf = 13344;
        volatile int dawpgww = 3844;
        volatile double styxhj = 23390;

        for (int i = 60; i > 0; i--) { continue; }
        for (int i = 40; i > 0; i--) { continue; }
        for (int i = 48; i > 0; i--) { continue; }
        for (int i = 48; i > 0; i--) { continue; }

        // Use Mersenne Twister for random junk
        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(1, 1000);
        volatile int junk_rand = dist(mt);
        junk_rand *= 3;
    }

    static inline __attribute__((always_inline)) void jcode02() {
        volatile std::string qajmdf = "eugzisktccfzceaulfwzpbcqcwtrrvpbgtiwottd";
        volatile std::string pwqygzcxc = "soojusdriokuhkwrzlckygrlffmezhzmpzj";
        volatile double qtjbt = 3911;
        volatile int hurezugyooilifx = 4065;
        volatile bool ksalmongiu = false;
        volatile int cibiz = 6509;
        volatile std::string tftbinhsi = "zqpvbwdqtkhflkjbttwiyhlsbsqlycjmprucvirymowwthcqtohndwrbaceywejudufiffpwiulr";
        volatile double qbsly = 51733;
        volatile double dkhrlxhzgjnuz = 1453;

        for (int i = 30; i > 0; i--) { continue; }

        // Random junk with MT
        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(10, 500);
        volatile int junk_rand = dist(mt);
        junk_rand += 42;
    }

    static inline __attribute__((always_inline)) void jcode03() {
        volatile std::string fsercapuzhmiksd = "jugggvnbktjhkeaermptpwrnxoanmikesswrqdzud";
        volatile int mdvor = 6735;
        volatile int ectvqddbfanl = 2372;
        volatile bool daoxzyvuq = false;
        volatile double hznphwwom = 12465;
        volatile std::string zvfge = "ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj";
        volatile bool kckiekgpnsc = true;
        volatile double evadkofeffbw = 57785;

        for (int i = 64; i > 0; i--) { continue; }
        for (int i = 87; i > 0; i--) { continue; }
        for (int i = 46; i > 0; i--) { continue; }
        for (int i = 50; i > 0; i--) { continue; }
        for (int i = 85; i > 0; i--) { continue; }

        // MT random junk
        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(0, 9999);
        volatile int junk_rand = dist(mt);
        junk_rand -= 100;
    }

    static inline __attribute__((always_inline)) void jcode04() {
        for (int i = 100; i > 0; i--) { continue; }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(0, 0xFFFFFFFF);
        volatile int junk_rand = dist(mt);
        junk_rand ^= 0xdeadbeef;
    }

    static inline __attribute__((always_inline)) void jcode05() {
        volatile double a = 3.14;
        volatile double b = 2.71;
        volatile double c = a + b;
        for (int i = 50; i > 0; i--) { continue; }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(0, 100);
        volatile int junk_rand = dist(mt);
        junk_rand += (int) c;
    }

    static inline __attribute__((always_inline)) void jcode06() {
        volatile int x = 12345;
        volatile bool flag = true;
        volatile double y = 3.14159;
        volatile std::string s = "junkcodegenerationtest";
        for (int i = 20; i > 0; i--) { asm volatile("":: : "memory"); }
        for (int i = 15; i > 0; i--) { continue; }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(5, 5000);
        volatile int r = dist(mt);
        r ^= x;
        if (flag && r > 1000) {
            volatile int z = r / 3;
            (void) z;
        }
    }

    static inline __attribute__((always_inline)) void jcode07() {
        std::string a = "abcdefghijklmnopqrstuvwxyz";
        std::string b = "zyxwvutsrqponmlkjihgfedcba";
        volatile int m = 777;
        volatile int n = 555;
        volatile bool bval = false;

        for (int i = 10; i > 0; i--) {
            if (a != b) {
                asm volatile("":: : "memory");
                continue;
            }
        }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(1, 100);
        volatile int junk = dist(mt);
        junk += m - n;
    }

    static inline __attribute__((always_inline)) void jcode08() {
        volatile double p = 1.6180339;
        volatile double q = 2.71828;
        volatile double r = p * q;
        volatile bool condition = (r > 4.4);

        for (int i = 0; i < 25; i++) {
            asm volatile("":: : "memory");
        }

        if (condition) {
            volatile int x = 0;
            for (; x < 15; x++) {
                asm volatile("":: : "memory");
            }
        }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(100, 1000);
        volatile int val = dist(mt);
        val -= 50;
    }

    static inline __attribute__((always_inline)) void jcode09() {
        volatile int a = 0xdeadbeef;
        volatile int b = 0xcafebabe;
        volatile bool flag = (a == b);

        for (int i = 30; i > 0; i--) {
            asm volatile("":: : "memory");
        }

        if (!flag) {
            for (int j = 5; j > 0; j--) {
                asm volatile("":: : "memory");
            }
        }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(0, 10000);
        volatile int random_num = dist(mt);
        random_num ^= a;
    }

    static inline __attribute__((always_inline)) void jcode10() {
        std::string s1 = "Lorem ipsum dolor sit amet";
        std::string s2 = "consectetur adipiscing elit";
        volatile int val = 1024;

        for (int i = 0; i < 20; i++) {
            asm volatile("":: : "memory");
        }

        if (s1 != s2) {
            for (int k = 10; k > 0; k--) {
                asm volatile("":: : "memory");
            }
        }

        auto &mt = get_mt();
        std::uniform_int_distribution<int> dist(500, 1500);
        volatile int rand_val = dist(mt);
        rand_val += val;
    }
}


inline __attribute__((always_inline)) int junk001() {
    volatile std::string fsercapuzhmiksd = "jugggvnbktjhkeaermptpwrnxoanmikesswrqdzud";
    volatile int mdvor = 6735;
    volatile int ectvqddbfanl = 2372;
    volatile bool daoxzyvuq = false;
    volatile double hznphwwom = 12465;
    volatile std::string zvfge = "ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj";
    volatile bool kckiekgpnsc = true;
    volatile double evadkofeffbw = 57785;

    if (std::string("ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj") !=
        std::string("ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj")) {
        volatile int hucoa;
        for (hucoa = 64; hucoa > 0; hucoa--) {
            asm volatile("":: : "memory");
            continue;
        }
    }
    if (true != true) {
        volatile int lhgvared;
        for (lhgvared = 87; lhgvared > 0; lhgvared--) {
            asm volatile("":: : "memory");
            continue;
        }
    }
    if (57785 == 57785) {
        volatile int adumgohgtk;
        for (adumgohgtk = 46; adumgohgtk > 0; adumgohgtk--) {
            asm volatile("":: : "memory");
            continue;
        }
    }
    if (true != true) {
        volatile int sfulmve;
        for (sfulmve = 50; sfulmve > 0; sfulmve--) {
            asm volatile("":: : "memory");
            continue;
        }
    }
    if (std::string("ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj") ==
        std::string("ufxrujslyeuhyrrlvobviaerkoglggqtjekkbjxfjrdlnlztqpnylmucmj")) {
        volatile int ajapdltp;
        for (ajapdltp = 85; ajapdltp > 0; ajapdltp--) {
            asm volatile("":: : "memory");
            continue;
        }
    }

    return 33978;
}

#endif //BSWRAPPER_JUNKCODES_HPP
