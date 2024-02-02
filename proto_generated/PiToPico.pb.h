/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_CA_STEFANM_E39_PROTO_PITOPICO_PB_H_INCLUDED
#define PB_CA_STEFANM_E39_PROTO_PITOPICO_PB_H_INCLUDED
#include <pb.h>
#include "ConfigProto.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* TODO Someday this could be a union proto type, but not this release. */
typedef enum _ca_stefanm_e39_proto_PiToPico_MessageType {
    ca_stefanm_e39_proto_PiToPico_MessageType_EmptyMessage = 0,
    ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatRequest = 1,
    ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatResponse = 2,
    ca_stefanm_e39_proto_PiToPico_MessageType_ConfigStatusRequest = 3, /* Instruct the pico to tell us what it's config object is */
    ca_stefanm_e39_proto_PiToPico_MessageType_ConfigPush = 4, /* Tell the pico what it's new config object should be */
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestUpstream = 5, /* Ask the pico to show upstream (for Back to BMW function) */
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestPico = 6, /* Ask the pico to show the debug menu */
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRpi = 7, /* Ask the pico to show the RPi. */
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRVC = 8,
    /* For test purposes to toggle the power switch for the RPI power supply. */
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOn = 9,
    ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOff = 10,
    /* Allow us to simulate ignition events without a GM or LCM or ignition switch */
    ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition0 = 11,
    ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition1 = 12,
    ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition2 = 13,
    ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition3 = 14
} ca_stefanm_e39_proto_PiToPico_MessageType;

/* Struct definitions */
typedef struct _ca_stefanm_e39_proto_PiToPico {
    ca_stefanm_e39_proto_PiToPico_MessageType messageType;
    bool has_newConfig;
    ca_stefanm_e39_proto_ConfigProto newConfig;
} ca_stefanm_e39_proto_PiToPico;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _ca_stefanm_e39_proto_PiToPico_MessageType_MIN ca_stefanm_e39_proto_PiToPico_MessageType_EmptyMessage
#define _ca_stefanm_e39_proto_PiToPico_MessageType_MAX ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition3
#define _ca_stefanm_e39_proto_PiToPico_MessageType_ARRAYSIZE ((ca_stefanm_e39_proto_PiToPico_MessageType)(ca_stefanm_e39_proto_PiToPico_MessageType_SimulatedIgnitionPosition3+1))

#define ca_stefanm_e39_proto_PiToPico_messageType_ENUMTYPE ca_stefanm_e39_proto_PiToPico_MessageType


/* Initializer values for message structs */
#define ca_stefanm_e39_proto_PiToPico_init_default {_ca_stefanm_e39_proto_PiToPico_MessageType_MIN, false, ca_stefanm_e39_proto_ConfigProto_init_default}
#define ca_stefanm_e39_proto_PiToPico_init_zero  {_ca_stefanm_e39_proto_PiToPico_MessageType_MIN, false, ca_stefanm_e39_proto_ConfigProto_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define ca_stefanm_e39_proto_PiToPico_messageType_tag 1
#define ca_stefanm_e39_proto_PiToPico_newConfig_tag 2

/* Struct field encoding specification for nanopb */
#define ca_stefanm_e39_proto_PiToPico_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    messageType,       1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  newConfig,         2)
#define ca_stefanm_e39_proto_PiToPico_CALLBACK NULL
#define ca_stefanm_e39_proto_PiToPico_DEFAULT NULL
#define ca_stefanm_e39_proto_PiToPico_newConfig_MSGTYPE ca_stefanm_e39_proto_ConfigProto

extern const pb_msgdesc_t ca_stefanm_e39_proto_PiToPico_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ca_stefanm_e39_proto_PiToPico_fields &ca_stefanm_e39_proto_PiToPico_msg

/* Maximum encoded size of messages (where known) */
#if defined(ca_stefanm_e39_proto_ConfigProto_size)
#define ca_stefanm_e39_proto_PiToPico_size       (8 + ca_stefanm_e39_proto_ConfigProto_size)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
