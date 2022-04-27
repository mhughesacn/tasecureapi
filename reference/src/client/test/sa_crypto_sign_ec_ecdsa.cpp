/**
 * Copyright 2020-2022 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sa.h" // NOLINT
#include "client_test_helpers.h"
#include "gtest/gtest.h"

using namespace client_test_helpers;

namespace {
    TEST(SaCryptoSign, failsEcEcdsaBadOutLengthEcp256) {
        auto curve = SA_ELLIPTIC_CURVE_NIST_P256;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);

        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);

        size_t out_length = 0;
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(nullptr, &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, nullptr, 0,
                &parameters);
        ASSERT_EQ(status, SA_STATUS_OK);
        ASSERT_EQ(out_length, key_size * 2);

        auto out = std::vector<uint8_t>(out_length);
        out_length -= 1;
        auto in = random(25);
        status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(), in.size(),
                &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcEcdsaBadOutLengthEcp384) {
        auto curve = SA_ELLIPTIC_CURVE_NIST_P384;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);
        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);
        if (*key == UNSUPPORTED_KEY)
            GTEST_SKIP() << "key type not supported";

        size_t out_length = 0;
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(nullptr, &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, nullptr, 0,
                &parameters);
        ASSERT_EQ(status, SA_STATUS_OK);
        ASSERT_EQ(out_length, key_size * 2);

        auto out = std::vector<uint8_t>(out_length);
        out_length -= 1;
        auto in = random(25);
        status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(), in.size(),
                &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcdsaNullParameters) {
        auto clear_key = sample_rsa_2048_pkcs8();

        sa_rights rights;
        rights_set_allow_all(&rights);

        auto key = create_sa_key_rsa(&rights, clear_key);
        ASSERT_NE(key, nullptr);

        auto out = std::vector<uint8_t>(512);
        size_t out_length = out.size();
        auto in = random(25);
        sa_status status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_RSA_PSS, *key, in.data(),
                in.size(), nullptr);
        ASSERT_EQ(status, SA_STATUS_NULL_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcEcdsaBadCurveX25519) {
        auto curve = SA_ELLIPTIC_CURVE_X25519;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);
        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);
        if (*key == UNSUPPORTED_KEY)
            GTEST_SKIP() << "key type not supported";

        auto out = std::vector<uint8_t>(key_size * 2);
        size_t out_length = out.size();
        auto in = random(25);
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(),
                in.size(), &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcEcdsaBadCurveX448) {
        auto curve = SA_ELLIPTIC_CURVE_X448;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);
        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);
        if (*key == UNSUPPORTED_KEY)
            GTEST_SKIP() << "key type not supported";

        auto out = std::vector<uint8_t>(key_size * 2);
        size_t out_length = out.size();
        auto in = random(25);
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(),
                in.size(), &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcEcdsaBadCurveED25519) {
        auto curve = SA_ELLIPTIC_CURVE_ED25519;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);
        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);
        if (*key == UNSUPPORTED_KEY)
            GTEST_SKIP() << "key type not supported";

        auto out = std::vector<uint8_t>(key_size * 2);
        size_t out_length = out.size();
        auto in = random(25);
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(),
                in.size(), &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }

    TEST(SaCryptoSign, failsEcEcdsaBadCurveED448) {
        auto curve = SA_ELLIPTIC_CURVE_ED448;
        auto key_size = ec_get_key_size(curve);
        auto clear_key = random_ec(key_size);
        sa_digest_algorithm digest_algorithm = SA_DIGEST_ALGORITHM_SHA256;

        sa_rights rights;
        rights_set_allow_all(&rights);
        auto key = create_sa_key_ec(&rights, curve, clear_key);
        ASSERT_NE(key, nullptr);
        if (*key == UNSUPPORTED_KEY)
            GTEST_SKIP() << "key type not supported";

        auto out = std::vector<uint8_t>(key_size * 2);
        size_t out_length = out.size();
        auto in = random(25);
        sa_sign_parameters_ecdsa parameters = {digest_algorithm, false};
        sa_status status = sa_crypto_sign(out.data(), &out_length, SA_SIGNATURE_ALGORITHM_ECDSA, *key, in.data(),
                in.size(), &parameters);
        ASSERT_EQ(status, SA_STATUS_BAD_PARAMETER);
    }
} // namespace