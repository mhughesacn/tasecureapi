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

#include "client.h"
#include "sa.h"
#include "ta_client.h"
#include <stdbool.h>

sa_status sa_get_version(sa_version* version) {

    if (version == NULL) {
        ERROR("version NULL");
        return SA_STATUS_NULL_PARAMETER;
    }

    void* session = client_session();
    if (session == NULL) {
        ERROR("client_session failed");
        return SA_STATUS_INTERNAL_ERROR;
    }

    sa_get_version_s* get_version = NULL;
    sa_status status;
    do {
        CREATE_COMMAND(sa_get_version_s, get_version);
        if (get_version == NULL) {
            ERROR("CREATE_COMMAND failed");
            status = SA_STATUS_INTERNAL_ERROR;
            break;
        }

        get_version->api_version = API_VERSION;
        get_version->version = *version;

        // clang-format off
        ta_param_type param_types[NUM_TA_PARAMS] = {TA_PARAM_INOUT, TA_PARAM_NULL, TA_PARAM_NULL, TA_PARAM_NULL};
        ta_param params[NUM_TA_PARAMS] = {{get_version, sizeof(sa_get_version_s)},
                                          {NULL, 0},
                                          {NULL, 0},
                                          {NULL, 0}};
        // clang-format on
        status = ta_invoke_command(session, SA_GET_VERSION, param_types, params);
        if (status != SA_STATUS_OK) {
            ERROR("ta_invoke_command failed: %d", status);
        }

        *version = get_version->version;
    } while (false);

    RELEASE_COMMAND(get_version);
    return status;
}
