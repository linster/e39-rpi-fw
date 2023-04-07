/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_CA_STEFANM_E39_PROTO_PICOTOPI_PB_H_INCLUDED
#define PB_CA_STEFANM_E39_PROTO_PICOTOPI_PB_H_INCLUDED
#include "../../../libs/nanopb/pb.h"
#include "ConfigProto.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _ca_stefanm_e39_proto_PicoToPi_MessageType {
    ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatRequest = 0,
    ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatResponse = 1,
    /* For the logger on the pico to shuttle to Rpi. */
    ca_stefanm_e39_proto_PicoToPi_MessageType_LogStatement = 2,
    /* Dump the config object */
    ca_stefanm_e39_proto_PicoToPi_MessageType_ConfigStatusResponse = 3,
    /* TODO we could have a mechanism where the pi sends to the pico
TODO the number of ibus events it processed (knob turn messages?)
TODO and if the pico isn't receiveng this heartbeat often enough,
TODO it could auto-restart the PI. */
    ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartX = 4, /* Ask the RPI to restart the X server */
    ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi = 5 /* Ask the RPI to legit restart */
} ca_stefanm_e39_proto_PicoToPi_MessageType;

/* Struct definitions */
typedef struct _ca_stefanm_e39_proto_PicoToPi {
    ca_stefanm_e39_proto_PicoToPi_MessageType messageType;
    bool has_existingConfig;
    ca_stefanm_e39_proto_ConfigProto existingConfig;
    pb_callback_t loggerStatement;
} ca_stefanm_e39_proto_PicoToPi;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _ca_stefanm_e39_proto_PicoToPi_MessageType_MIN ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatRequest
#define _ca_stefanm_e39_proto_PicoToPi_MessageType_MAX ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi
#define _ca_stefanm_e39_proto_PicoToPi_MessageType_ARRAYSIZE ((ca_stefanm_e39_proto_PicoToPi_MessageType)(ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi+1))

#define ca_stefanm_e39_proto_PicoToPi_messageType_ENUMTYPE ca_stefanm_e39_proto_PicoToPi_MessageType


/* Initializer values for message structs */
#define ca_stefanm_e39_proto_PicoToPi_init_default {_ca_stefanm_e39_proto_PicoToPi_MessageType_MIN, false, ca_stefanm_e39_proto_ConfigProto_init_default, {{NULL}, NULL}}
#define ca_stefanm_e39_proto_PicoToPi_init_zero  {_ca_stefanm_e39_proto_PicoToPi_MessageType_MIN, false, ca_stefanm_e39_proto_ConfigProto_init_zero, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define ca_stefanm_e39_proto_PicoToPi_messageType_tag 1
#define ca_stefanm_e39_proto_PicoToPi_existingConfig_tag 2
#define ca_stefanm_e39_proto_PicoToPi_loggerStatement_tag 3

/* Struct field encoding specification for nanopb */
#define ca_stefanm_e39_proto_PicoToPi_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    messageType,       1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  existingConfig,    2) \
X(a, CALLBACK, OPTIONAL, STRING,   loggerStatement,   3)
#define ca_stefanm_e39_proto_PicoToPi_CALLBACK pb_default_field_callback
#define ca_stefanm_e39_proto_PicoToPi_DEFAULT NULL
#define ca_stefanm_e39_proto_PicoToPi_existingConfig_MSGTYPE ca_stefanm_e39_proto_ConfigProto

extern const pb_msgdesc_t ca_stefanm_e39_proto_PicoToPi_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ca_stefanm_e39_proto_PicoToPi_fields &ca_stefanm_e39_proto_PicoToPi_msg

/* Maximum encoded size of messages (where known) */
/* ca_stefanm_e39_proto_PicoToPi_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
