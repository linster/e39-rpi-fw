/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_CA_STEFANM_E39_PROTO_CONFIGPROTO_PB_H_INCLUDED
#define PB_CA_STEFANM_E39_PROTO_CONFIGPROTO_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _ca_stefanm_e39_proto_ConfigProto_LoggingLevels {
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels_DEBUG = 0,
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels_INFO = 1,
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WARN = 2,
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels_ERROR = 3,
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WTF = 4
} ca_stefanm_e39_proto_ConfigProto_LoggingLevels;

typedef enum _ca_stefanm_e39_proto_ConfigProto_ScanProgram {
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_NONE = 0,
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_CLOCK = 1,
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_LINSTEROS_BOOTSPLASH = 2,
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_GOOSE_BOOTSPLASH = 3,
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_BMW_BOOTSPLASH = 4,
    ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU = 5
} ca_stefanm_e39_proto_ConfigProto_ScanProgram;

typedef enum _ca_stefanm_e39_proto_ConfigProto_VideoSource {
    ca_stefanm_e39_proto_ConfigProto_VideoSource_RVC = 0,
    ca_stefanm_e39_proto_ConfigProto_VideoSource_UPSTREAM = 1,
    ca_stefanm_e39_proto_ConfigProto_VideoSource_PICO = 2,
    ca_stefanm_e39_proto_ConfigProto_VideoSource_PI = 3
} ca_stefanm_e39_proto_ConfigProto_VideoSource;

typedef enum _ca_stefanm_e39_proto_ConfigProto_VideoEncoding {
    ca_stefanm_e39_proto_ConfigProto_VideoEncoding_NTSC = 0,
    ca_stefanm_e39_proto_ConfigProto_VideoEncoding_PAL = 1
} ca_stefanm_e39_proto_ConfigProto_VideoEncoding;

typedef enum _ca_stefanm_e39_proto_ConfigProto_AspectRatio {
    ca_stefanm_e39_proto_ConfigProto_AspectRatio_FourThree = 0,
    ca_stefanm_e39_proto_ConfigProto_AspectRatio_SixteenNine = 1,
    ca_stefanm_e39_proto_ConfigProto_AspectRatio_Zoom = 2
} ca_stefanm_e39_proto_ConfigProto_AspectRatio;

/* Struct definitions */
typedef struct _ca_stefanm_e39_proto_ConfigProto {
    pb_callback_t str;
    bool isIbusLogOutputEnabled;
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels enabledMaxLogLevelForIbusLog;
    ca_stefanm_e39_proto_ConfigProto_LoggingLevels enabledMaxLogLevelForPrintfLog;
    /* If enabled, always turn on the Pi4 power supply when we start up. */
    bool alwaysTurnOnRpiOnStatup;
    /* If enabled, send a packet to turn on BMBT screen */
    bool alwaysTurnOnScreenOnIbusActivity;
    ca_stefanm_e39_proto_ConfigProto_ScanProgram scanProgramOnBoot;
    ca_stefanm_e39_proto_ConfigProto_VideoSource videoSourceOnBoot;
    /* https://github.com/piersholt/wilhelm-docs/blob/master/bmbt/4f.md */
    bool sendBMBTEncodingPacketOnBootup;
    ca_stefanm_e39_proto_ConfigProto_VideoEncoding videoEncoding;
    ca_stefanm_e39_proto_ConfigProto_AspectRatio aspectRatio;
} ca_stefanm_e39_proto_ConfigProto;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MIN ca_stefanm_e39_proto_ConfigProto_LoggingLevels_DEBUG
#define _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MAX ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WTF
#define _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_ARRAYSIZE ((ca_stefanm_e39_proto_ConfigProto_LoggingLevels)(ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WTF+1))

#define _ca_stefanm_e39_proto_ConfigProto_ScanProgram_MIN ca_stefanm_e39_proto_ConfigProto_ScanProgram_NONE
#define _ca_stefanm_e39_proto_ConfigProto_ScanProgram_MAX ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU
#define _ca_stefanm_e39_proto_ConfigProto_ScanProgram_ARRAYSIZE ((ca_stefanm_e39_proto_ConfigProto_ScanProgram)(ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU+1))

#define _ca_stefanm_e39_proto_ConfigProto_VideoSource_MIN ca_stefanm_e39_proto_ConfigProto_VideoSource_RVC
#define _ca_stefanm_e39_proto_ConfigProto_VideoSource_MAX ca_stefanm_e39_proto_ConfigProto_VideoSource_PI
#define _ca_stefanm_e39_proto_ConfigProto_VideoSource_ARRAYSIZE ((ca_stefanm_e39_proto_ConfigProto_VideoSource)(ca_stefanm_e39_proto_ConfigProto_VideoSource_PI+1))

#define _ca_stefanm_e39_proto_ConfigProto_VideoEncoding_MIN ca_stefanm_e39_proto_ConfigProto_VideoEncoding_NTSC
#define _ca_stefanm_e39_proto_ConfigProto_VideoEncoding_MAX ca_stefanm_e39_proto_ConfigProto_VideoEncoding_PAL
#define _ca_stefanm_e39_proto_ConfigProto_VideoEncoding_ARRAYSIZE ((ca_stefanm_e39_proto_ConfigProto_VideoEncoding)(ca_stefanm_e39_proto_ConfigProto_VideoEncoding_PAL+1))

#define _ca_stefanm_e39_proto_ConfigProto_AspectRatio_MIN ca_stefanm_e39_proto_ConfigProto_AspectRatio_FourThree
#define _ca_stefanm_e39_proto_ConfigProto_AspectRatio_MAX ca_stefanm_e39_proto_ConfigProto_AspectRatio_Zoom
#define _ca_stefanm_e39_proto_ConfigProto_AspectRatio_ARRAYSIZE ((ca_stefanm_e39_proto_ConfigProto_AspectRatio)(ca_stefanm_e39_proto_ConfigProto_AspectRatio_Zoom+1))

#define ca_stefanm_e39_proto_ConfigProto_enabledMaxLogLevelForIbusLog_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_LoggingLevels
#define ca_stefanm_e39_proto_ConfigProto_enabledMaxLogLevelForPrintfLog_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_LoggingLevels
#define ca_stefanm_e39_proto_ConfigProto_scanProgramOnBoot_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_ScanProgram
#define ca_stefanm_e39_proto_ConfigProto_videoSourceOnBoot_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_VideoSource
#define ca_stefanm_e39_proto_ConfigProto_videoEncoding_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_VideoEncoding
#define ca_stefanm_e39_proto_ConfigProto_aspectRatio_ENUMTYPE ca_stefanm_e39_proto_ConfigProto_AspectRatio


/* Initializer values for message structs */
#define ca_stefanm_e39_proto_ConfigProto_init_default {{{NULL}, NULL}, 0, _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MIN, _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MIN, 0, 0, _ca_stefanm_e39_proto_ConfigProto_ScanProgram_MIN, _ca_stefanm_e39_proto_ConfigProto_VideoSource_MIN, 0, _ca_stefanm_e39_proto_ConfigProto_VideoEncoding_MIN, _ca_stefanm_e39_proto_ConfigProto_AspectRatio_MIN}
#define ca_stefanm_e39_proto_ConfigProto_init_zero {{{NULL}, NULL}, 0, _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MIN, _ca_stefanm_e39_proto_ConfigProto_LoggingLevels_MIN, 0, 0, _ca_stefanm_e39_proto_ConfigProto_ScanProgram_MIN, _ca_stefanm_e39_proto_ConfigProto_VideoSource_MIN, 0, _ca_stefanm_e39_proto_ConfigProto_VideoEncoding_MIN, _ca_stefanm_e39_proto_ConfigProto_AspectRatio_MIN}

/* Field tags (for use in manual encoding/decoding) */
#define ca_stefanm_e39_proto_ConfigProto_str_tag 1
#define ca_stefanm_e39_proto_ConfigProto_isIbusLogOutputEnabled_tag 2
#define ca_stefanm_e39_proto_ConfigProto_enabledMaxLogLevelForIbusLog_tag 3
#define ca_stefanm_e39_proto_ConfigProto_enabledMaxLogLevelForPrintfLog_tag 4
#define ca_stefanm_e39_proto_ConfigProto_alwaysTurnOnRpiOnStatup_tag 5
#define ca_stefanm_e39_proto_ConfigProto_alwaysTurnOnScreenOnIbusActivity_tag 6
#define ca_stefanm_e39_proto_ConfigProto_scanProgramOnBoot_tag 7
#define ca_stefanm_e39_proto_ConfigProto_videoSourceOnBoot_tag 8
#define ca_stefanm_e39_proto_ConfigProto_sendBMBTEncodingPacketOnBootup_tag 9
#define ca_stefanm_e39_proto_ConfigProto_videoEncoding_tag 10
#define ca_stefanm_e39_proto_ConfigProto_aspectRatio_tag 11

/* Struct field encoding specification for nanopb */
#define ca_stefanm_e39_proto_ConfigProto_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   str,               1) \
X(a, STATIC,   SINGULAR, BOOL,     isIbusLogOutputEnabled,   2) \
X(a, STATIC,   SINGULAR, UENUM,    enabledMaxLogLevelForIbusLog,   3) \
X(a, STATIC,   SINGULAR, UENUM,    enabledMaxLogLevelForPrintfLog,   4) \
X(a, STATIC,   SINGULAR, BOOL,     alwaysTurnOnRpiOnStatup,   5) \
X(a, STATIC,   SINGULAR, BOOL,     alwaysTurnOnScreenOnIbusActivity,   6) \
X(a, STATIC,   SINGULAR, UENUM,    scanProgramOnBoot,   7) \
X(a, STATIC,   SINGULAR, UENUM,    videoSourceOnBoot,   8) \
X(a, STATIC,   SINGULAR, BOOL,     sendBMBTEncodingPacketOnBootup,   9) \
X(a, STATIC,   SINGULAR, UENUM,    videoEncoding,    10) \
X(a, STATIC,   SINGULAR, UENUM,    aspectRatio,      11)
#define ca_stefanm_e39_proto_ConfigProto_CALLBACK pb_default_field_callback
#define ca_stefanm_e39_proto_ConfigProto_DEFAULT NULL

extern const pb_msgdesc_t ca_stefanm_e39_proto_ConfigProto_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ca_stefanm_e39_proto_ConfigProto_fields &ca_stefanm_e39_proto_ConfigProto_msg

/* Maximum encoded size of messages (where known) */
/* ca_stefanm_e39_proto_ConfigProto_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
