cmake_minimum_required(VERSION 3.29)
project(talecrpt)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)     # Find OpenSSL package

add_executable(test main.cpp keys.hpp talecrypt/talecrypt.cpp)

target_include_directories(test PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(test PRIVATE OpenSSL::Crypto)  # Link to libcrypto
