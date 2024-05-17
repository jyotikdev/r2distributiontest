// Copyright 2016 Square, Inc.

#pragma once

#include "cr_comms_driver_api.h"
#include <cardreader/cr_comms_backend_api.h>
#include <cardreader_shared/crs_timer.h>

typedef struct cr_comms_driver_t cr_comms_driver_t;

typedef struct cr_comms_driver_cfg_t {
    cr_comms_backend_api_t backend;
    crs_timer_api_t timer_api;
} cr_comms_driver_cfg_t;

cr_comms_driver_t *cr_comms_driver_create(cr_comms_driver_cfg_t const *cfg);

cr_comms_driver_api_t cr_comms_driver_build_api(cr_comms_driver_t *comms_driver);
