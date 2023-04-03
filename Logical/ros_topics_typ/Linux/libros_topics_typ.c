#include <string.h>
#define EXOS_ASSERT_LOG &logger
#include "exos_log.h"
#include "libros_topics_typ.h"

#define SUCCESS(_format_, ...) exos_log_success(&logger, EXOS_LOG_TYPE_USER, _format_, ##__VA_ARGS__);
#define INFO(_format_, ...) exos_log_info(&logger, EXOS_LOG_TYPE_USER, _format_, ##__VA_ARGS__);
#define VERBOSE(_format_, ...) exos_log_debug(&logger, EXOS_LOG_TYPE_USER + EXOS_LOG_TYPE_VERBOSE, _format_, ##__VA_ARGS__);
#define ERROR(_format_, ...) exos_log_error(&logger, _format_, ##__VA_ARGS__);

static exos_log_handle_t logger;

typedef struct libros_topics_typHandle
{
    libros_topics_typ_t ext_ros_topics_typ_datamodel;
    exos_datamodel_handle_t ros_topics_typ_datamodel;

    exos_dataset_handle_t odemetry_dataset;
    exos_dataset_handle_t twist_dataset;
    exos_dataset_handle_t config_dataset;
    exos_dataset_handle_t encoder_dataset;
    exos_dataset_handle_t vaccumtopic;
    exos_dataset_handle_t linefollow;
    exos_dataset_handle_t linestatus;
} libros_topics_typHandle_t;

static libros_topics_typHandle_t h_ros_topics_typ;

static void libros_topics_typ_datasetEvent(exos_dataset_handle_t *dataset, EXOS_DATASET_EVENT_TYPE event_type, void *info)
{
    switch (event_type)
    {
    case EXOS_DATASET_EVENT_UPDATED:
        VERBOSE("dataset %s updated! latency (us):%i", dataset->name, (exos_datamodel_get_nettime(dataset->datamodel) - dataset->nettime));
        //handle each subscription dataset separately
        if (0 == strcmp(dataset->name, "odemetry"))
        {
            //update the nettime
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.odemetry.nettime = dataset->nettime;

            //trigger the callback if assigned
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.odemetry.on_change)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.odemetry.on_change();
            }
        }
        else if (0 == strcmp(dataset->name, "encoder"))
        {
            //update the nettime
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.encoder.nettime = dataset->nettime;

            //trigger the callback if assigned
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.encoder.on_change)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.encoder.on_change();
            }
        }
        else if (0 == strcmp(dataset->name, "lineStatus"))
        {
            //update the nettime
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineStatus.nettime = dataset->nettime;

            //trigger the callback if assigned
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineStatus.on_change)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineStatus.on_change();
            }
        }
        break;

    case EXOS_DATASET_EVENT_PUBLISHED:
        break;
    case EXOS_DATASET_EVENT_DELIVERED:
        break;
    case EXOS_DATASET_EVENT_CONNECTION_CHANGED:
        INFO("dataset %s changed state to %s", dataset->name, exos_get_state_string(dataset->connection_state));

        switch (dataset->connection_state)
        {
        case EXOS_STATE_DISCONNECTED:
            break;
        case EXOS_STATE_CONNECTED:
            break;
        case EXOS_STATE_OPERATIONAL:
            break;
        case EXOS_STATE_ABORTED:
            ERROR("dataset %s error %d (%s) occured", dataset->name, dataset->error, exos_get_error_string(dataset->error));
            break;
        }
        break;
    }
}

static void libros_topics_typ_datamodelEvent(exos_datamodel_handle_t *datamodel, const EXOS_DATAMODEL_EVENT_TYPE event_type, void *info)
{
    switch (event_type)
    {
    case EXOS_DATAMODEL_EVENT_CONNECTION_CHANGED:
        INFO("application changed state to %s", exos_get_state_string(datamodel->connection_state));

        h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_connected = false;
        h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_operational = false;
        switch (datamodel->connection_state)
        {
        case EXOS_STATE_DISCONNECTED:
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_disconnected)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_disconnected();
            }
            break;
        case EXOS_STATE_CONNECTED:
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_connected = true;
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_connected)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_connected();
            }
            break;
        case EXOS_STATE_OPERATIONAL:
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_connected = true;
            h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_operational = true;
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_operational)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_operational();
            }
            SUCCESS("ros_topics_typ operational!");
            break;
        case EXOS_STATE_ABORTED:
            if (NULL != h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_disconnected)
            {
                h_ros_topics_typ.ext_ros_topics_typ_datamodel.on_disconnected();
            }
            ERROR("application error %d (%s) occured", datamodel->error, exos_get_error_string(datamodel->error));
            break;
        }
        break;
    case EXOS_DATAMODEL_EVENT_SYNC_STATE_CHANGED:
        break;

    default:
        break;

    }
}

static void libros_topics_typ_publish_twist_dataset(void)
{
    EXOS_ASSERT_OK(exos_dataset_publish(&h_ros_topics_typ.twist_dataset));
}
static void libros_topics_typ_publish_config_dataset(void)
{
    EXOS_ASSERT_OK(exos_dataset_publish(&h_ros_topics_typ.config_dataset));
}
static void libros_topics_typ_publish_vaccumtopic(void)
{
    EXOS_ASSERT_OK(exos_dataset_publish(&h_ros_topics_typ.vaccumtopic));
}
static void libros_topics_typ_publish_linefollow(void)
{
    EXOS_ASSERT_OK(exos_dataset_publish(&h_ros_topics_typ.linefollow));
}

static void libros_topics_typ_connect(void)
{
    //connect the datamodel
    EXOS_ASSERT_OK(exos_datamodel_connect_ros_topics_typ(&(h_ros_topics_typ.ros_topics_typ_datamodel), libros_topics_typ_datamodelEvent));
    
    //connect datasets
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.odemetry_dataset), EXOS_DATASET_SUBSCRIBE, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.twist_dataset), EXOS_DATASET_PUBLISH, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.config_dataset), EXOS_DATASET_PUBLISH, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.encoder_dataset), EXOS_DATASET_SUBSCRIBE, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.vaccumtopic), EXOS_DATASET_PUBLISH, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.linefollow), EXOS_DATASET_PUBLISH, libros_topics_typ_datasetEvent));
    EXOS_ASSERT_OK(exos_dataset_connect(&(h_ros_topics_typ.linestatus), EXOS_DATASET_SUBSCRIBE, libros_topics_typ_datasetEvent));
}
static void libros_topics_typ_disconnect(void)
{
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_connected = false;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_operational = false;

    EXOS_ASSERT_OK(exos_datamodel_disconnect(&(h_ros_topics_typ.ros_topics_typ_datamodel)));
}

static void libros_topics_typ_set_operational(void)
{
    EXOS_ASSERT_OK(exos_datamodel_set_operational(&(h_ros_topics_typ.ros_topics_typ_datamodel)));
}

static void libros_topics_typ_process(void)
{
    EXOS_ASSERT_OK(exos_datamodel_process(&(h_ros_topics_typ.ros_topics_typ_datamodel)));
    exos_log_process(&logger);
}

static void libros_topics_typ_dispose(void)
{
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_connected = false;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.is_operational = false;

    EXOS_ASSERT_OK(exos_datamodel_delete(&(h_ros_topics_typ.ros_topics_typ_datamodel)));
    exos_log_delete(&logger);
}

static int32_t libros_topics_typ_get_nettime(void)
{
    return exos_datamodel_get_nettime(&(h_ros_topics_typ.ros_topics_typ_datamodel));
}

static void libros_topics_typ_log_error(char* log_entry)
{
    exos_log_error(&logger, log_entry);
}

static void libros_topics_typ_log_warning(char* log_entry)
{
    exos_log_warning(&logger, EXOS_LOG_TYPE_USER, log_entry);
}

static void libros_topics_typ_log_success(char* log_entry)
{
    exos_log_success(&logger, EXOS_LOG_TYPE_USER, log_entry);
}

static void libros_topics_typ_log_info(char* log_entry)
{
    exos_log_info(&logger, EXOS_LOG_TYPE_USER, log_entry);
}

static void libros_topics_typ_log_debug(char* log_entry)
{
    exos_log_debug(&logger, EXOS_LOG_TYPE_USER, log_entry);
}

static void libros_topics_typ_log_verbose(char* log_entry)
{
    exos_log_warning(&logger, EXOS_LOG_TYPE_USER + EXOS_LOG_TYPE_VERBOSE, log_entry);
}

libros_topics_typ_t *libros_topics_typ_init(void)
{
    memset(&h_ros_topics_typ, 0, sizeof(h_ros_topics_typ));

    h_ros_topics_typ.ext_ros_topics_typ_datamodel.twist.publish = libros_topics_typ_publish_twist_dataset;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.config.publish = libros_topics_typ_publish_config_dataset;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.vaccumTopic.publish = libros_topics_typ_publish_vaccumtopic;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineFollow.publish = libros_topics_typ_publish_linefollow;
    
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.connect = libros_topics_typ_connect;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.disconnect = libros_topics_typ_disconnect;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.process = libros_topics_typ_process;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.set_operational = libros_topics_typ_set_operational;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.dispose = libros_topics_typ_dispose;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.get_nettime = libros_topics_typ_get_nettime;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.error = libros_topics_typ_log_error;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.warning = libros_topics_typ_log_warning;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.success = libros_topics_typ_log_success;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.info = libros_topics_typ_log_info;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.debug = libros_topics_typ_log_debug;
    h_ros_topics_typ.ext_ros_topics_typ_datamodel.log.verbose = libros_topics_typ_log_verbose;
    
    exos_log_init(&logger, "gros_topics_typ_0");

    SUCCESS("starting gros_topics_typ_0 application..");

    EXOS_ASSERT_OK(exos_datamodel_init(&h_ros_topics_typ.ros_topics_typ_datamodel, "ros_topics_typ_0", "gros_topics_typ_0"));

    //set the user_context to access custom data in the callbacks
    h_ros_topics_typ.ros_topics_typ_datamodel.user_context = NULL; //not used
    h_ros_topics_typ.ros_topics_typ_datamodel.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.odemetry_dataset, &h_ros_topics_typ.ros_topics_typ_datamodel, "odemetry", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.odemetry.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.odemetry.value)));
    h_ros_topics_typ.odemetry_dataset.user_context = NULL; //not used
    h_ros_topics_typ.odemetry_dataset.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.twist_dataset, &h_ros_topics_typ.ros_topics_typ_datamodel, "twist", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.twist.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.twist.value)));
    h_ros_topics_typ.twist_dataset.user_context = NULL; //not used
    h_ros_topics_typ.twist_dataset.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.config_dataset, &h_ros_topics_typ.ros_topics_typ_datamodel, "config", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.config.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.config.value)));
    h_ros_topics_typ.config_dataset.user_context = NULL; //not used
    h_ros_topics_typ.config_dataset.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.encoder_dataset, &h_ros_topics_typ.ros_topics_typ_datamodel, "encoder", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.encoder.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.encoder.value)));
    h_ros_topics_typ.encoder_dataset.user_context = NULL; //not used
    h_ros_topics_typ.encoder_dataset.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.vaccumtopic, &h_ros_topics_typ.ros_topics_typ_datamodel, "vaccumTopic", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.vaccumTopic.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.vaccumTopic.value)));
    h_ros_topics_typ.vaccumtopic.user_context = NULL; //not used
    h_ros_topics_typ.vaccumtopic.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.linefollow, &h_ros_topics_typ.ros_topics_typ_datamodel, "lineFollow", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineFollow.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineFollow.value)));
    h_ros_topics_typ.linefollow.user_context = NULL; //not used
    h_ros_topics_typ.linefollow.user_tag = 0; //not used

    EXOS_ASSERT_OK(exos_dataset_init(&h_ros_topics_typ.linestatus, &h_ros_topics_typ.ros_topics_typ_datamodel, "lineStatus", &h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineStatus.value, sizeof(h_ros_topics_typ.ext_ros_topics_typ_datamodel.lineStatus.value)));
    h_ros_topics_typ.linestatus.user_context = NULL; //not used
    h_ros_topics_typ.linestatus.user_tag = 0; //not used

    return &(h_ros_topics_typ.ext_ros_topics_typ_datamodel);
}
