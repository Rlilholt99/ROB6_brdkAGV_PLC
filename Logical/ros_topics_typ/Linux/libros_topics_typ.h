#ifndef _LIBROS_TOPICS_TYP_H_
#define _LIBROS_TOPICS_TYP_H_

#include "exos_ros_topics_typ.h"

typedef void (*libros_topics_typ_event_cb)(void);
typedef void (*libros_topics_typ_method_fn)(void);
typedef int32_t (*libros_topics_typ_get_nettime_fn)(void);
typedef void (*libros_topics_typ_log_fn)(char *log_entry);

typedef struct libros_topics_typodemetry
{
    libros_topics_typ_event_cb on_change;
    int32_t nettime;
    ros_topic_odemety_typ value;
} libros_topics_typodemetry_t;

typedef struct libros_topics_typtwist
{
    libros_topics_typ_method_fn publish;
    ros_topic_twist_typ value;
} libros_topics_typtwist_t;

typedef struct libros_topics_typconfig
{
    libros_topics_typ_method_fn publish;
    ros_config_typ value;
} libros_topics_typconfig_t;

typedef struct libros_topics_typencoder
{
    libros_topics_typ_event_cb on_change;
    int32_t nettime;
    ros_encoder value;
} libros_topics_typencoder_t;

typedef struct libros_topics_typvaccumTopic
{
    libros_topics_typ_method_fn publish;
    ros_armBools value;
} libros_topics_typvaccumTopic_t;

typedef struct libros_topics_typlineFollow
{
    libros_topics_typ_method_fn publish;
    ros_lineMode value;
} libros_topics_typlineFollow_t;

typedef struct libros_topics_typlineStatus
{
    libros_topics_typ_event_cb on_change;
    int32_t nettime;
    ros_lineStatus value;
} libros_topics_typlineStatus_t;

typedef struct libros_topics_typ_log
{
    libros_topics_typ_log_fn error;
    libros_topics_typ_log_fn warning;
    libros_topics_typ_log_fn success;
    libros_topics_typ_log_fn info;
    libros_topics_typ_log_fn debug;
    libros_topics_typ_log_fn verbose;
} libros_topics_typ_log_t;

typedef struct libros_topics_typ
{
    libros_topics_typ_method_fn connect;
    libros_topics_typ_method_fn disconnect;
    libros_topics_typ_method_fn process;
    libros_topics_typ_method_fn set_operational;
    libros_topics_typ_method_fn dispose;
    libros_topics_typ_get_nettime_fn get_nettime;
    libros_topics_typ_log_t log;
    libros_topics_typ_event_cb on_connected;
    libros_topics_typ_event_cb on_disconnected;
    libros_topics_typ_event_cb on_operational;
    bool is_connected;
    bool is_operational;
    libros_topics_typodemetry_t odemetry;
    libros_topics_typtwist_t twist;
    libros_topics_typconfig_t config;
    libros_topics_typencoder_t encoder;
    libros_topics_typvaccumTopic_t vaccumTopic;
    libros_topics_typlineFollow_t lineFollow;
    libros_topics_typlineStatus_t lineStatus;
} libros_topics_typ_t;

#ifdef __cplusplus
extern "C" {
#endif
libros_topics_typ_t *libros_topics_typ_init(void);
#ifdef __cplusplus
}
#endif
#endif // _LIBROS_TOPICS_TYP_H_
