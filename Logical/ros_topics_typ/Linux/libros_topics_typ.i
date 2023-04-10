%module(directors="1") libros_topics_typ
%{
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "exos_ros_topics_typ.h"
#include "libros_topics_typ.h"
%}

%include "typemaps.i"
%include "std_except.i"

%feature("director") ros_topics_typEventHandler;
%inline %{
struct ros_topics_typEventHandler
{
    virtual void on_connected(void) {}
    virtual void on_disconnected(void) {}
    virtual void on_operational(void) {}

    virtual void on_change_odemetry() {}
    virtual void on_change_encoder() {}
    virtual void on_change_lineStatus() {}

    virtual ~ros_topics_typEventHandler() {}
    libros_topics_typ_t *ros_topics_typ_datamodel;
};
%}

%{
static ros_topics_typEventHandler *pros_topics_typEventHandler = NULL;

static void libros_topics_typ_on_connected()
{
    pros_topics_typEventHandler->on_connected();
}

static void libros_topics_typ_on_disconnected()
{
    pros_topics_typEventHandler->on_disconnected();
}

static void libros_topics_typ_on_operational()
{
    pros_topics_typEventHandler->on_operational();
}

static void libros_topics_typ_on_change_odemetry()
{
    pros_topics_typEventHandler->on_change_odemetry();
}
static void libros_topics_typ_on_change_encoder()
{
    pros_topics_typEventHandler->on_change_encoder();
}
static void libros_topics_typ_on_change_lineStatus()
{
    pros_topics_typEventHandler->on_change_lineStatus();
}
%}

%inline %{
void add_event_handler(libros_topics_typ_t *ros_topics_typ_datamodel, ros_topics_typEventHandler *handler)
{
    pros_topics_typEventHandler = handler;

    ros_topics_typ_datamodel->on_connected = &libros_topics_typ_on_connected;
    ros_topics_typ_datamodel->on_disconnected = &libros_topics_typ_on_disconnected;
    ros_topics_typ_datamodel->on_operational = &libros_topics_typ_on_operational;
    
    ros_topics_typ_datamodel->odemetry.on_change = &libros_topics_typ_on_change_odemetry;
    ros_topics_typ_datamodel->encoder.on_change = &libros_topics_typ_on_change_encoder;
    ros_topics_typ_datamodel->lineStatus.on_change = &libros_topics_typ_on_change_lineStatus;
    
    pros_topics_typEventHandler->ros_topics_typ_datamodel = ros_topics_typ_datamodel;
    handler = NULL;
}
%}

%include "stdint.i"

/* Handle arrays in substructures, structs could be exposed using these two lines:
     %include "exos_ros_topics_typ.h"
   But we need to disable the array members and add them again with the wrapped_array
*/
typedef struct ros_lineStatus
{
    int16_t lineStatusCode;

} ros_lineStatus;

typedef struct ros_lineMode
{
    bool lineMode;
    bool lineSharpTurn;
    bool lineHoldRight;
    bool lineForward;
    bool lineInterSection;
    bool lineDecision;
    bool lineFast;
    bool lineSlow;

} ros_lineMode;

typedef struct ros_armBools
{
    bool vaccumValve;
    bool vaccumMotor;

} ros_armBools;

typedef struct ros_encoder
{
    int16_t encoder1;
    int16_t encoder2;

} ros_encoder;

typedef struct ros_config_typ
{
    double maxSpeed;
    double minSpeed;
    double baseWidth;

} ros_config_typ;

//this expresses velocity in free space broken into its linear and angular parts.
typedef struct ros_topic_vector3_typ
{
    double y;
    double z;
    double x;

} ros_topic_vector3_typ;

//This expresses velocity in free space broken into its linear and angular parts.
typedef struct ros_topic_twist_typ
{
    struct ros_topic_vector3_typ angular;
    struct ros_topic_vector3_typ linear;

} ros_topic_twist_typ;

typedef struct ros_topic_time_typ
{
    uint32_t nsec;
    uint32_t sec;

} ros_topic_time_typ;

typedef struct ros_topic_header_typ
{
    struct ros_topic_time_typ stamp;
    uint32_t seq;
    char frame_id[21];

} ros_topic_header_typ;

//This expresses velocity in free space with uncertainty.
typedef struct ros_topic_twistCor_typ
{
    struct ros_topic_twist_typ twist;

} ros_topic_twistCor_typ;

//This represents an orientation in free space in quaternion form.
typedef struct ros_topic_quaternion_typ
{
    double y;
    double z;
    double w;
    double x;

} ros_topic_quaternion_typ;

//this expresses velocity in free space broken into its linear and angular parts.
typedef struct ros_topic_point_typ
{
    double y;
    double z;
    double x;

} ros_topic_point_typ;

//A representation of pose in free space, composed of position and orientation. 
typedef struct ros_topic_pose_typ
{
    struct ros_topic_point_typ position;
    struct ros_topic_quaternion_typ orientation;

} ros_topic_pose_typ;

//This represents a pose in free space with uncertainty.
typedef struct ros_topic_poseCor_typ
{
    struct ros_topic_pose_typ pose;

} ros_topic_poseCor_typ;

typedef struct ros_topic_odemety_typ
{
    struct ros_topic_poseCor_typ pose;
    struct ros_topic_twistCor_typ twist;
    struct ros_topic_header_typ header;

} ros_topic_odemety_typ;

typedef struct libros_topics_typodemetry
{
    void on_change(void);
    int32_t nettime;
    ros_topic_odemety_typ value;
} libros_topics_typodemetry_t;

typedef struct libros_topics_typtwist
{
    void publish(void);
    ros_topic_twist_typ value;
} libros_topics_typtwist_t;

typedef struct libros_topics_typconfig
{
    void publish(void);
    ros_config_typ value;
} libros_topics_typconfig_t;

typedef struct libros_topics_typencoder
{
    void on_change(void);
    int32_t nettime;
    ros_encoder value;
} libros_topics_typencoder_t;

typedef struct libros_topics_typvaccumTopic
{
    void publish(void);
    ros_armBools value;
} libros_topics_typvaccumTopic_t;

typedef struct libros_topics_typlineFollow
{
    void publish(void);
    ros_lineMode value;
} libros_topics_typlineFollow_t;

typedef struct libros_topics_typlineStatus
{
    void on_change(void);
    int32_t nettime;
    ros_lineStatus value;
} libros_topics_typlineStatus_t;

typedef struct libros_topics_typ_log
{
    void error(char *log_entry);
    void warning(char *log_entry);
    void success(char *log_entry);
    void info(char *log_entry);
    void debug(char *log_entry);
    void verbose(char *log_entry);
} libros_topics_typ_log_t;

typedef struct libros_topics_typ
{
    void connect(void);
    void disconnect(void);
    void process(void);
    void set_operational(void);
    void dispose(void);
    int32_t get_nettime(void);
    libros_topics_typ_log_t log;
    void on_connected(void);
    void on_disconnected(void);
    void on_operational(void);
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

libros_topics_typ_t *libros_topics_typ_init(void);
