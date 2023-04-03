/*Automatically generated c file from ros_topics_typ.typ*/

#include "exos_ros_topics_typ.h"

const char config_ros_topics_typ[] = "{\"name\":\"struct\",\"attributes\":{\"name\":\"<NAME>\",\"dataType\":\"ros_topics_typ\",\"info\":\"<infoId0>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"odemetry\",\"dataType\":\"ros_topic_odemety_typ\",\"comment\":\"PUB\",\"info\":\"<infoId1>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"pose\",\"dataType\":\"ros_topic_poseCor_typ\",\"info\":\"<infoId2>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"pose\",\"dataType\":\"ros_topic_pose_typ\",\"info\":\"<infoId3>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"position\",\"dataType\":\"ros_topic_point_typ\",\"info\":\"<infoId4>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId5>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId6>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId7>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"orientation\",\"dataType\":\"ros_topic_quaternion_typ\",\"info\":\"<infoId8>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId9>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId10>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"w\",\"dataType\":\"LREAL\",\"info\":\"<infoId11>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId12>\"}}]}]}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"twist\",\"dataType\":\"ros_topic_twistCor_typ\",\"info\":\"<infoId13>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"twist\",\"dataType\":\"ros_topic_twist_typ\",\"info\":\"<infoId14>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"angular\",\"dataType\":\"ros_topic_vector3_typ\",\"info\":\"<infoId15>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId16>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId17>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId18>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"linear\",\"dataType\":\"ros_topic_vector3_typ\",\"info\":\"<infoId19>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId20>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId21>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId22>\"}}]}]}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"header\",\"dataType\":\"ros_topic_header_typ\",\"info\":\"<infoId23>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"stamp\",\"dataType\":\"ros_topic_time_typ\",\"info\":\"<infoId24>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"nsec\",\"dataType\":\"UDINT\",\"info\":\"<infoId25>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"sec\",\"dataType\":\"UDINT\",\"info\":\"<infoId26>\"}}]},{\"name\":\"variable\",\"attributes\":{\"name\":\"seq\",\"dataType\":\"UDINT\",\"info\":\"<infoId27>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"frame_id\",\"dataType\":\"STRING\",\"stringLength\":21,\"info\":\"<infoId28>\"}}]}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"twist\",\"dataType\":\"ros_topic_twist_typ\",\"comment\":\"SUB\",\"info\":\"<infoId29>\"},\"children\":[{\"name\":\"struct\",\"attributes\":{\"name\":\"angular\",\"dataType\":\"ros_topic_vector3_typ\",\"info\":\"<infoId30>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId31>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId32>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId33>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"linear\",\"dataType\":\"ros_topic_vector3_typ\",\"info\":\"<infoId34>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"y\",\"dataType\":\"LREAL\",\"info\":\"<infoId35>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"z\",\"dataType\":\"LREAL\",\"info\":\"<infoId36>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"x\",\"dataType\":\"LREAL\",\"info\":\"<infoId37>\"}}]}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"config\",\"dataType\":\"ros_config_typ\",\"comment\":\"SUB\",\"info\":\"<infoId38>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"maxSpeed\",\"dataType\":\"LREAL\",\"info\":\"<infoId39>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"minSpeed\",\"dataType\":\"LREAL\",\"info\":\"<infoId40>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"baseWidth\",\"dataType\":\"LREAL\",\"info\":\"<infoId41>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"encoder\",\"dataType\":\"ros_encoder\",\"comment\":\"PUB\",\"info\":\"<infoId42>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"encoder1\",\"dataType\":\"INT\",\"info\":\"<infoId43>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"encoder2\",\"dataType\":\"INT\",\"info\":\"<infoId44>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"vaccumTopic\",\"dataType\":\"ros_armBools\",\"comment\":\"SUB\",\"info\":\"<infoId45>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"vaccumValve\",\"dataType\":\"BOOL\",\"info\":\"<infoId46>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"vaccumMotor\",\"dataType\":\"BOOL\",\"info\":\"<infoId47>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"lineFollow\",\"dataType\":\"ros_lineMode\",\"comment\":\"SUB\",\"info\":\"<infoId48>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"lineMode\",\"dataType\":\"BOOL\",\"info\":\"<infoId49>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"lineSharpTurn\",\"dataType\":\"BOOL\",\"info\":\"<infoId50>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"lineHoldRight\",\"dataType\":\"BOOL\",\"info\":\"<infoId51>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"lineForward\",\"dataType\":\"BOOL\",\"info\":\"<infoId52>\"}},{\"name\":\"variable\",\"attributes\":{\"name\":\"lineInterSection\",\"dataType\":\"BOOL\",\"info\":\"<infoId53>\"}}]},{\"name\":\"struct\",\"attributes\":{\"name\":\"lineStatus\",\"dataType\":\"ros_lineStatus\",\"comment\":\"PUB\",\"info\":\"<infoId54>\"},\"children\":[{\"name\":\"variable\",\"attributes\":{\"name\":\"lineStatusCode\",\"dataType\":\"INT\",\"info\":\"<infoId55>\"}}]}]}";

/*Connect the ros_topics_typ datamodel to the server*/
EXOS_ERROR_CODE exos_datamodel_connect_ros_topics_typ(exos_datamodel_handle_t *datamodel, exos_datamodel_event_cb datamodel_event_callback)
{
    ros_topics_typ data;
    exos_dataset_info_t datasets[] = {
        {EXOS_DATASET_BROWSE_NAME_INIT,{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.position),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.position.y),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.position.z),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.position.x),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.orientation),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.orientation.y),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.orientation.z),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.orientation.w),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.pose.pose.orientation.x),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.angular),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.angular.y),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.angular.z),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.angular.x),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.linear),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.linear.y),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.linear.z),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.twist.twist.linear.x),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header.stamp),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header.stamp.nsec),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header.stamp.sec),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header.seq),{}},
        {EXOS_DATASET_BROWSE_NAME(odemetry.header.frame_id),{}},
        {EXOS_DATASET_BROWSE_NAME(twist),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.angular),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.angular.y),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.angular.z),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.angular.x),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.linear),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.linear.y),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.linear.z),{}},
        {EXOS_DATASET_BROWSE_NAME(twist.linear.x),{}},
        {EXOS_DATASET_BROWSE_NAME(config),{}},
        {EXOS_DATASET_BROWSE_NAME(config.maxSpeed),{}},
        {EXOS_DATASET_BROWSE_NAME(config.minSpeed),{}},
        {EXOS_DATASET_BROWSE_NAME(config.baseWidth),{}},
        {EXOS_DATASET_BROWSE_NAME(encoder),{}},
        {EXOS_DATASET_BROWSE_NAME(encoder.encoder1),{}},
        {EXOS_DATASET_BROWSE_NAME(encoder.encoder2),{}},
        {EXOS_DATASET_BROWSE_NAME(vaccumTopic),{}},
        {EXOS_DATASET_BROWSE_NAME(vaccumTopic.vaccumValve),{}},
        {EXOS_DATASET_BROWSE_NAME(vaccumTopic.vaccumMotor),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow.lineMode),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow.lineSharpTurn),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow.lineHoldRight),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow.lineForward),{}},
        {EXOS_DATASET_BROWSE_NAME(lineFollow.lineInterSection),{}},
        {EXOS_DATASET_BROWSE_NAME(lineStatus),{}},
        {EXOS_DATASET_BROWSE_NAME(lineStatus.lineStatusCode),{}}
    };

    exos_datamodel_calc_dataset_info(datasets, sizeof(datasets));

    return exos_datamodel_connect(datamodel, config_ros_topics_typ, datasets, sizeof(datasets), datamodel_event_callback);
}
