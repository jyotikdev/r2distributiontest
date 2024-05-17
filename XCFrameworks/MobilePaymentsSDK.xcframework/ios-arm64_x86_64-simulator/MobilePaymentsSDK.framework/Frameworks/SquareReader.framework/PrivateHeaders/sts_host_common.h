#pragma once

struct cr_sts_feature_crypto_api_t;
struct crs_securesession_comms_api_t;

/// STS host type
typedef enum
{
    STS_HOST_TYPE_DEV,   /// For development use (e.g. crtpy)
    STS_HOST_TYPE_SQUID, /// For SQUID devices
} sts_host_type_e;
