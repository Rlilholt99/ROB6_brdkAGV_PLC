/*Automatically generated header file from ros_topics_typ.typ*/

#ifndef _EXOS_COMP_ROS_TOPICS_TYP_H_
#define _EXOS_COMP_ROS_TOPICS_TYP_H_

#include "exos_api.h"

#if defined(_SG4)
#include <ros_topics.h>
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef struct ros_topic_vector3_typ
{
    double y;
    double z;
    double x;

} ros_topic_vector3_typ;

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

typedef struct ros_topic_twistCor_typ
{
    struct ros_topic_twist_typ twist;

} ros_topic_twistCor_typ;

typedef struct ros_topic_quaternion_typ
{
    double y;
    double z;
    double w;
    double x;

} ros_topic_quaternion_typ;

typedef struct ros_topic_point_typ
{
    double y;
    double z;
    double x;

} ros_topic_point_typ;

typedef struct ros_topic_pose_typ
{
    struct ros_topic_point_typ position;
    struct ros_topic_quaternion_typ orientation;

} ros_topic_pose_typ;

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

typedef struct ros_topics_typ
{
    struct ros_topic_odemety_typ odemetry; //PUB
    struct ros_topic_twist_typ twist; //SUB
    struct ros_config_typ config; //SUB
    struct ros_encoder encoder; //PUB
    struct ros_armBools vaccumTopic; //SUB
    struct ros_lineMode lineFollow; //SUB
    struct ros_lineStatus lineStatus; //PUB

} ros_topics_typ;

#endif // _SG4

EXOS_ERROR_CODE exos_datamodel_connect_ros_topics_typ(exos_datamodel_handle_t *datamodel, exos_datamodel_event_cb datamodel_event_callback);

#endif // _EXOS_COMP_ROS_TOPICS_TYP_H_
