#
# Copyright 2020-2022 Comcast Cable Communications Management, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.16)

project(saclient)

if (DEFINED ENABLE_CLANG_TIDY_TESTS)
    find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
    if (CLANG_TIDY_COMMAND)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND}; )
        set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_COMMAND}; )
        message("clang-tidy found--enabling")
    else ()
        message("clang-tidy not found")
    endif ()
else ()
    message("clang-tidy disabled")
endif ()

add_library(saclient SHARED
        include/sa.h
        include/sa_crypto.h
        include/sa_key.h
        include/sa_svp.h
        include/sa_ta_types.h
        include/sa_types.h
        include/sa_cenc.h
        )

set_target_properties(saclient PROPERTIES
        LINKER_LANGUAGE C
        SO_VERSION 3.0
        VERSION 3.0.8
        )

target_include_directories(saclient
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    # using Clang
    target_link_libraries(saclient
            PRIVATE
            -Wl,-all_load
            saclientimpl
            -Wl,-noall_load
            )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    target_link_libraries(saclient
            PRIVATE
            -Wl,--whole-archive
            saclientimpl
            -Wl,--no-whole-archive
            )
endif ()

target_clangformat_setup(saclient)

# Google test
add_executable(saclienttest
        test/client_test_helpers.cpp
        test/client_test_helpers.h
        test/environment.cpp
        test/sa_crypto_cipher_common.h
        test/sa_crypto_cipher_common.cpp
        test/sa_crypto_cipher_init.cpp
        test/sa_crypto_cipher_init_aes_cbc.cpp
        test/sa_crypto_cipher_init_aes_cbc_pkcs7.cpp
        test/sa_crypto_cipher_init_aes_ctr.cpp
        test/sa_crypto_cipher_init_aes_ecb.cpp
        test/sa_crypto_cipher_init_aes_ecb_pkcs7.cpp
        test/sa_crypto_cipher_init_aes_gcm.cpp
        test/sa_crypto_cipher_init_ec_elgamal.cpp
        test/sa_crypto_cipher_init_chacha20.cpp
        test/sa_crypto_cipher_init_chacha20_poly1305.cpp
        test/sa_crypto_cipher_init_rsa_oaep.cpp
        test/sa_crypto_cipher_init_rsa_pkcs1v15.cpp
        test/sa_crypto_cipher_process.cpp
        test/sa_crypto_cipher_process_aes_cbc.cpp
        test/sa_crypto_cipher_process_aes_cbc_pkcs7.cpp
        test/sa_crypto_cipher_process_aes_ctr.cpp
        test/sa_crypto_cipher_process_aes_ecb.cpp
        test/sa_crypto_cipher_process_aes_ecb_pkcs7.cpp
        test/sa_crypto_cipher_process_aes_gcm.cpp
        test/sa_crypto_cipher_process_chacha20.cpp
        test/sa_crypto_cipher_process_chacha20_poly1305.cpp
        test/sa_crypto_cipher_process_ec_elgamal.cpp
        test/sa_crypto_cipher_process_last.cpp
        test/sa_crypto_cipher_process_last_aes_cbc.cpp
        test/sa_crypto_cipher_process_last_aes_cbc_pkcs7.cpp
        test/sa_crypto_cipher_process_last_aes_ctr.cpp
        test/sa_crypto_cipher_process_last_aes_ecb.cpp
        test/sa_crypto_cipher_process_last_aes_ecb_pkcs7.cpp
        test/sa_crypto_cipher_process_last_aes_gcm.cpp
        test/sa_crypto_cipher_process_last_chacha20_poly1305.cpp
        test/sa_crypto_cipher_process_last_ec_elgamal.cpp
        test/sa_crypto_cipher_process_last_rsa_oaep.cpp
        test/sa_crypto_cipher_process_last_rsa_pkcs1v15.cpp
        test/sa_crypto_cipher_process_rsa_oaep.cpp
        test/sa_crypto_cipher_process_rsa_pkcs1v15.cpp
        test/sa_crypto_cipher_release.cpp
        test/sa_crypto_cipher_update_iv.cpp
        test/sa_crypto_cipher_update_iv_aes_cbc.cpp
        test/sa_crypto_cipher_update_iv_aes_cbc_pkcs7.cpp
        test/sa_crypto_cipher_update_iv_aes_ctr.cpp
        test/sa_crypto_cipher_update_iv_aes_ecb.cpp
        test/sa_crypto_cipher_update_iv_aes_ecb_pkcs7.cpp
        test/sa_crypto_cipher_update_iv_aes_gcm.cpp
        test/sa_crypto_cipher_update_iv_chacha20.cpp
        test/sa_crypto_cipher_update_iv_chacha20_poly1305.cpp
        test/sa_crypto_cipher_update_iv_ec_elgamal.cpp
        test/sa_crypto_cipher_update_iv_rsa_oaep.cpp
        test/sa_crypto_cipher_update_iv_rsa_pkcs1v15.cpp
        test/sa_crypto_mac_common.cpp
        test/sa_crypto_mac_common.h
        test/sa_crypto_mac_compute.cpp
        test/sa_crypto_mac_init.cpp
        test/sa_crypto_mac_process.cpp
        test/sa_crypto_mac_process_key.cpp
        test/sa_crypto_mac_release.cpp
        test/sa_crypto_random.cpp
        test/sa_crypto_sign.cpp
        test/sa_crypto_sign_common.cpp
        test/sa_crypto_sign_common.h
        test/sa_crypto_sign_ec_ecdsa.cpp
        test/sa_crypto_sign_ec_eddsa.cpp
        test/sa_crypto_sign_rsa_pkcs1v15.cpp
        test/sa_crypto_sign_rsa_pss.cpp
        test/sa_get_device_id.cpp
        test/sa_get_name.cpp
        test/sa_get_ta_uuid.cpp
        test/sa_get_version.cpp
        test/sa_key_common.cpp
        test/sa_key_common.h
        test/sa_key_derive.cpp
        test/sa_key_derive_ansi_x963.cpp
        test/sa_key_derive_cmac.cpp
        test/sa_key_derive_common.cpp
        test/sa_key_derive_common.h
        test/sa_key_derive_concat.cpp
        test/sa_key_derive_hkdf.cpp
        test/sa_key_derive_netflix.cpp
        test/sa_key_derive_root_key_ladder.cpp
        test/sa_key_digest.cpp
        test/sa_key_exchange.cpp
        test/sa_key_exchange_common.cpp
        test/sa_key_exchange_common.h
        test/sa_key_exchange_dh.cpp
        test/sa_key_exchange_ecdh.cpp
        test/sa_key_exchange_netflix.cpp
        test/sa_key_export.cpp
        test/sa_key_generate.cpp
        test/sa_key_generate_dh.cpp
        test/sa_key_generate_ec.cpp
        test/sa_key_generate_rsa.cpp
        test/sa_key_generate_symmetric.cpp
        test/sa_key_get_public.cpp
        test/sa_key_get_public_dh.cpp
        test/sa_key_get_public_ec.cpp
        test/sa_key_get_public_rsa.cpp
        test/sa_key_get_public_symmetric.cpp
        test/sa_key_header.cpp
        test/sa_key_import.cpp
        test/sa_key_import_common.cpp
        test/sa_key_import_common.h
        test/sa_key_import_ec_private_bytes.cpp
        test/sa_key_import_exported.cpp
        test/sa_key_import_rsa_private_key_info.cpp
        test/sa_key_import_soc.cpp
        test/sa_key_import_symmetric_bytes.cpp
        test/sa_key_import_typej.cpp
        test/sa_key_release.cpp
        test/sa_key_unwrap.cpp
        test/sa_key_unwrap_common.cpp
        test/sa_key_unwrap_common.h
        test/sa_key_unwrap_aes_cbc.cpp
        test/sa_key_unwrap_aes_ctr.cpp
        test/sa_key_unwrap_aes_ecb.cpp
        test/sa_key_unwrap_aes_gcm.cpp
        test/sa_key_unwrap_ec.cpp
        test/sa_key_unwrap_rsa.cpp
        test/sa_crypto_cipher_multiple_thread.cpp
        test/sa_process_common_encryption.cpp
        test/sa_process_common_encryption_common.cpp
        test/sa_process_common_encryption_common.h
        test/sa_svp_buffer_alloc.cpp
        test/sa_svp_buffer_check.cpp
        test/sa_svp_buffer_copy.cpp
        test/sa_svp_buffer_create.cpp
        test/sa_svp_buffer_release.cpp
        test/sa_svp_buffer_write.cpp
        test/sa_svp_key_check.cpp
        test/sa_svp_common.cpp
        test/sa_svp_common.h
        )

find_package(OpenSSL REQUIRED)

target_compile_options(saclienttest PRIVATE -Werror -Wall -Wextra -Wno-type-limits -Wno-unused-parameter)

target_include_directories(saclienttest
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${OPENSSL_INCLUDE_DIR}
        )

target_link_libraries(saclienttest
        PRIVATE
        gtest
        gmock_main
        saclient
        ${OPENSSL_CRYPTO_LIBRARY}
        )

if (COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(saclienttest
            PRIVATE
            gcov
            )
endif ()

target_clangformat_setup(saclienttest)

add_test(saclienttest saclienttest)

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS saclient saclienttest EXPORT sa-client-config
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Doxygen
option(BUILD_DOC "Build documentation" ON)
if (BUILD_DOC)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(saclientdocs ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM
                )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif (BUILD_DOC)
