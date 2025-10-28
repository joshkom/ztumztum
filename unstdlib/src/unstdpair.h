#ifndef UNSTDLIB_UNSTDPAIR_H
#define UNSTDLIB_UNSTDPAIR_H

#ifndef unstdpair_pairM
#define unstdpair_pairM(type_first_arg, type_second_arg)   \
    struct {                                               \
        type_first_arg first;                              \
        type_second_arg second;                            \
    }
#endif

#ifndef unstdpair_pair_typeM
#define unstdpair_pair_typeM(type_first_arg, type_second_arg) \
    typedef struct {                                          \
        type_first_arg first;                                 \
        type_second_arg second;                               \
    }
#endif

#ifndef unstdpair_pair_type_namedM
#define unstdpair_pair_type_namedM(first_arg, second_arg) \
    typedef struct {                                      \
        first_arg;                                        \
        second_arg;                                       \
    }
#endif

#ifndef unstdpair_pair_type_named_legacyM
#define unstdpair_pair_type_named_legacyM(...) \
    typedef struct {                           \
        __VA_ARGS__                            \
    }
#endif

#endif /* UNSTDLIB_UNSTDPAIR_H */
