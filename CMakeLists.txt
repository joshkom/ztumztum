cmake_minimum_required(VERSION 3.22.1)
project("royale")

set(ANDROID_NDK_HOME "/home/johan/Android/Sdk/ndk/26.1.10909125")
set(OMVLL_PLUGIN "${CMAKE_SOURCE_DIR}/omvll_ndk_r26d.so")
set(ENV{OMVLL_CONFIG} "${CMAKE_SOURCE_DIR}/omvll_config.py")

#! TODO : REPLACE THIS BUILD SHIT WITH THE OTHER ONE DOWN FURTHER COMMENTED
#set(OLLVM_COMPILE_FLAGS
#        -fpass-plugin=${OMVLL_PLUGIN}
#        -fPIC
#        -fvisibility=hidden
#        -fvisibility-inlines-hidden
#        -fno-rtti
#        -fno-exceptions
#        -g                             # emit DWARF debug info (function names, file/line)
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)
#
## Link flags — remove global stripping so symbols remain available for backtraces
## (Keep gc-sections/exclude-libs if you want smaller binary but keep symbols)
#set(OLLVM_LINK_FLAGS
#        -fpass-plugin=${OMVLL_PLUGIN}
#        -Wl,--gc-sections
#        -Wl,--exclude-libs,ALL
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)

set(OLLVM_COMPILE_FLAGS
        -fpass-plugin=${OMVLL_PLUGIN}
        -fPIC
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-rtti
        -fno-exceptions
        --target=aarch64-linux-android26
        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
)
set(OLLVM_LINK_FLAGS
        -fpass-plugin=${OMVLL_PLUGIN}
        -Wl,--strip-all
        -Wl,--gc-sections
        -Wl,--exclude-libs,ALL
        -s
        --target=aarch64-linux-android26
        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
)

# Common compile & link options # OLD
#set(OLLVM_COMPILE_FLAGS
#        -fpass-plugin=${OMVLL_PLUGIN}
#        -fPIC
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)
#
#set(OLLVM_LINK_FLAGS
#        -fpass-plugin=${OMVLL_PLUGIN}
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)

# Add BLAKE3 sources
file(GLOB_RECURSE BLAKE3_SOURCES
        ${CMAKE_SOURCE_DIR}/deps/BLAKE3/c/*.c
        ${CMAKE_SOURCE_DIR}/deps/BLAKE3/c/*.h
)

list(FILTER BLAKE3_SOURCES EXCLUDE REGEX ".*(example|test|x86|windows|msvc|avx|sse).*")

add_library(blake3 STATIC ${BLAKE3_SOURCES})

target_include_directories(blake3 PUBLIC ${CMAKE_SOURCE_DIR}/deps/BLAKE3/c)

target_compile_options(blake3 PRIVATE -fPIC -std=c11)

#list(FILTER BLAKE3_SOURCES EXCLUDE REGEX ".*(example|test|x86|windows|msvc).*")

# Include directory for BLAKE3
include_directories(${CMAKE_SOURCE_DIR}/deps/BLAKE3/c)

# Set OpenSSL include and static lib paths
set(UNSTDLIB_INCLUDE_DIR "/home/johan/LIB/X/Programming/Projects/unstdlib/src")
set(OPENSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/openssl/include")
set(OPENSSL_LIB_DIR "${CMAKE_SOURCE_DIR}/deps/openssl/lib")

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${UNSTDLIB_INCLUDE_DIR})
include_directories(.)

# Sources
file(GLOB_RECURSE unstdlib
        ${UNSTDLIB_INCLUDE_DIR}/unstdbool*
        ${UNSTDLIB_INCLUDE_DIR}/unstdcommon*
        ${UNSTDLIB_INCLUDE_DIR}/unstdstring*
        ${UNSTDLIB_INCLUDE_DIR}/unstdmemory*
        ${UNSTDLIB_INCLUDE_DIR}/unstdchar*
        ${UNSTDLIB_INCLUDE_DIR}/_unstdcommon*
        ${UNSTDLIB_INCLUDE_DIR}/unstdprintf*
)

file(GLOB_RECURSE global_common
#        global_common/*.cpp
#        global_common/*.hpp
        global_common/*.h
)

file(GLOB_RECURSE taletools
#        global_common/taletools/*.cpp
#        global_common/taletools/*.hpp
)

file(GLOB_RECURSE tale_protect
#        TaleProtect/*.cpp
        TaleProtect/*.c
#        TaleProtect/*.hpp
        TaleProtect/*.h
)

list(FILTER tale_protect EXCLUDE REGEX ".*(sniffer).*")

foreach(f ${tale_protect})
    if(f MATCHES ".*/aes-whitebox/.*")
        list(REMOVE_ITEM tale_protect ${f})
    endif()
endforeach()

foreach(f ${tale_protect})
    if(f MATCHES ".*/aes-whitebox.bak/.*")
        list(REMOVE_ITEM tale_protect ${f})
    endif()
endforeach()

#file(GLOB_RECURSE tale_rasp talerasp/*.cpp talerasp/*.hpp)

# Add curl
find_package(curl REQUIRED CONFIG)

# Add a new static library target for whitebox
add_library(whitebox STATIC
        TaleProtect/unpack_procedure/libg/aes-whitebox/aes_whitebox_tables.cc
        TaleProtect/unpack_procedure/libg/aes-whitebox/aes_whitebox.cc
)

# Compile WITHOUT OLLVM
target_compile_options(whitebox PRIVATE
        -fPIC
        -std=c++11
)

#target_compile_options(tale.protect PRIVATE ${OLLVM_COMPILE_FLAGS} -std=c11)

# libtale.protect
add_library(tale.protect SHARED
#        ${BLAKE3_SOURCES}
        ${unstdlib}
        ${global_common}
        #        ./TaleProtect/unpack_procedure/libg/aes-whitebox/aes_whitebox.cc
        #        ./TaleProtect/unpack_procedure/libg/aes-whitebox/aes_whitebox_tables.cc
        TaleProtect/tale-protect.c
        ${tale_protect}
)

target_compile_options(tale.protect PRIVATE ${OLLVM_COMPILE_FLAGS} -std=c11)
target_link_options(tale.protect PRIVATE ${OLLVM_LINK_FLAGS})

target_link_libraries(tale.protect
        android
        log
        #        ${OPENSSL_LIB_DIR}/libssl.a
        #        ${OPENSSL_LIB_DIR}/libcrypto.a
        whitebox
        blake3
        curl::curl_static
)

# libtale.rasp
#add_library(tale.rasp SHARED
#        ${global_common}
##        ${taletools}
#        talerasp/tale-rasp.cpp
#        ${tale_rasp}
#)

#target_compile_options(tale.rasp PRIVATE ${OLLVM_COMPILE_FLAGS})
#target_link_options(tale.rasp PRIVATE ${OLLVM_LINK_FLAGS})
#
#target_link_libraries(tale.rasp
#        android
#        log
##        ${OPENSSL_LIB_DIR}/libssl.a
##        ${OPENSSL_LIB_DIR}/libcrypto.a
#)

# libjprovider
add_library(b SHARED
        ${taletools}
        JProvider/polymorph-lib.h
        JProvider/jprovider.cpp
)

target_compile_options(b PRIVATE ${OLLVM_COMPILE_FLAGS})
target_link_options(b PRIVATE ${OLLVM_LINK_FLAGS})

target_link_libraries(b
        android
        log
##        ${OPENSSL_LIB_DIR}/libssl.a
##        ${OPENSSL_LIB_DIR}/libcrypto.a
)


#=============================================================


#add_custom_command(TARGET tale.protect POST_BUILD
#        COMMAND ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-objcopy
#        --remove-section=.comment
#        --remove-section=.note
#        --remove-section=.gnu.version
#        $<TARGET_FILE:tale.protect>
#)
#
#add_custom_command(TARGET tale.rasp POST_BUILD
#        COMMAND ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-objcopy
#        --remove-section=.comment
#        --remove-section=.note
#        --remove-section=.gnu.version
#        $<TARGET_FILE:tale.rasp>
#)

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")
#set(CMAKE_C_FLAGS_RELEASE "-O3 -flto")
#
#set(OLLVM_COMPILE_FLAGS
#        -O3
#        -flto
#        -fpass-plugin=${OMVLL_PLUGIN}
#        -fPIC
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)
#
#set(OLLVM_LINK_FLAGS
#        -flto
#        -fpass-plugin=${OMVLL_PLUGIN}
#        --target=aarch64-linux-android26
#        --sysroot=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64/sysroot
#)