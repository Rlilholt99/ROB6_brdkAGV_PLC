#include <ros_topics.h>

#define EXOS_ASSERT_LOG &handle->logger
#define EXOS_ASSERT_CALLBACK inst->_state = 255;
#include "exos_log.h"
#include "exos_ros_topics_typ.h"
#include <string.h>

#define SUCCESS(_format_, ...) exos_log_success(&handle->logger, EXOS_LOG_TYPE_USER, _format_, ##__VA_ARGS__);
#define INFO(_format_, ...) exos_log_info(&handle->logger, EXOS_LOG_TYPE_USER, _format_, ##__VA_ARGS__);
#define VERBOSE(_format_, ...) exos_log_debug(&handle->logger, EXOS_LOG_TYPE_USER + EXOS_LOG_TYPE_VERBOSE, _format_, ##__VA_ARGS__);
#define ERROR(_format_, ...) exos_log_error(&handle->logger, _format_, ##__VA_ARGS__);

int test;
int test2;

typedef struct
{
    void *self;
    exos_log_handle_t logger;
    ros_topics_typ data;

    exos_datamodel_handle_t ros_topics_typ_datamodel;

    exos_dataset_handle_t odemetry_dataset;
    exos_dataset_handle_t twist_dataset;
    exos_dataset_handle_t config_dataset;
    exos_dataset_handle_t encoder_dataset;
    exos_dataset_handle_t vaccumtopic;
    exos_dataset_handle_t linefollow;
    exos_dataset_handle_t linestatus;
} ros_topics_typHandle_t;

static void datasetEvent(exos_dataset_handle_t *dataset, EXOS_DATASET_EVENT_TYPE event_type, void *info)
{
    struct ros_topics_typCyclic *inst = (struct ros_topics_typCyclic *)dataset->datamodel->user_context;
    ros_topics_typHandle_t *handle = (ros_topics_typHandle_t *)inst->Handle;

    switch (event_type)
    {
    case EXOS_DATASET_EVENT_UPDATED:
        VERBOSE("dataset %s updated! latency (us):%i", dataset->name, (exos_datamodel_get_nettime(dataset->datamodel) - dataset->nettime));
        //handle each subscription dataset separately
        if(0 == strcmp(dataset->name, "twist"))
        {
            memcpy(&inst->pros_topics_typ->twist, dataset->data, dataset->size);
        }
        else if(0 == strcmp(dataset->name, "config"))
        {
            memcpy(&inst->pros_topics_typ->config, dataset->data, dataset->size);
        }
        else if(0 == strcmp(dataset->name, "vaccumTopic"))
        {
            memcpy(&inst->pros_topics_typ->vaccumTopic, dataset->data, dataset->size);
        }
        else if(0 == strcmp(dataset->name, "lineFollow"))
        {
            memcpy(&inst->pros_topics_typ->lineFollow, dataset->data, dataset->size);
        }
        break;

    case EXOS_DATASET_EVENT_PUBLISHED:
        VERBOSE("dataset %s published to local server for distribution! send buffer free:%i", dataset->name, dataset->send_buffer.free);
        //handle each published dataset separately
        if(0 == strcmp(dataset->name, "odemetry"))
        {
            // ros_topic_odemety_typ *odemetry_dataset = (ros_topic_odemety_typ *)dataset->data;
        }
        else if(0 == strcmp(dataset->name, "encoder"))
        {
            // ros_encoder *encoder_dataset = (ros_encoder *)dataset->data;
        }
        else if(0 == strcmp(dataset->name, "lineStatus"))
        {
            // ros_lineStatus *linestatus = (ros_lineStatus *)dataset->data;
        }
        break;

    case EXOS_DATASET_EVENT_DELIVERED:
        VERBOSE("dataset %s delivered to remote server for distribution! send buffer free:%i", dataset->name, dataset->send_buffer.free);
        //handle each published dataset separately
        if(0 == strcmp(dataset->name, "odemetry"))
        {
            // ros_topic_odemety_typ *odemetry_dataset = (ros_topic_odemety_typ *)dataset->data;
        }
        else if(0 == strcmp(dataset->name, "encoder"))
        {
            // ros_encoder *encoder_dataset = (ros_encoder *)dataset->data;
        }
        else if(0 == strcmp(dataset->name, "lineStatus"))
        {
            // ros_lineStatus *linestatus = (ros_lineStatus *)dataset->data;
        }
        break;

    case EXOS_DATASET_EVENT_CONNECTION_CHANGED:
        INFO("dataset %s changed state to %s", dataset->name, exos_get_state_string(dataset->connection_state));

        switch (dataset->connection_state)
        {
        case EXOS_STATE_DISCONNECTED:
            break;
        case EXOS_STATE_CONNECTED:
            //call the dataset changed event to update the dataset when connected
            //datasetEvent(dataset,EXOS_DATASET_UPDATED,info);
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

static void datamodelEvent(exos_datamodel_handle_t *datamodel, const EXOS_DATAMODEL_EVENT_TYPE event_type, void *info)
{
    struct ros_topics_typCyclic *inst = (struct ros_topics_typCyclic *)datamodel->user_context;
    ros_topics_typHandle_t *handle = (ros_topics_typHandle_t *)inst->Handle;

    switch (event_type)
    {
    case EXOS_DATAMODEL_EVENT_CONNECTION_CHANGED:
        INFO("application changed state to %s", exos_get_state_string(datamodel->connection_state));

        inst->Disconnected = 0;
        inst->Connected = 0;
        inst->Operational = 0;
        inst->Aborted = 0;

        switch (datamodel->connection_state)
        {
        case EXOS_STATE_DISCONNECTED:
            inst->Disconnected = 1;
            inst->_state = 255;
            break;
        case EXOS_STATE_CONNECTED:
            inst->Connected = 1;
            break;
        case EXOS_STATE_OPERATIONAL:
            SUCCESS("ros_topics_typ operational!");
            inst->Operational = 1;
            break;
        case EXOS_STATE_ABORTED:
            ERROR("application error %d (%s) occured", datamodel->error, exos_get_error_string(datamodel->error));
            inst->_state = 255;
            inst->Aborted = 1;
            break;
        }
        break;
    case EXOS_DATAMODEL_EVENT_SYNC_STATE_CHANGED:
        break;

    default:
        break;

    }

}

_BUR_PUBLIC void ros_topics_typInit(struct ros_topics_typInit *inst)
{
    ros_topics_typHandle_t *handle;
    TMP_alloc(sizeof(ros_topics_typHandle_t), (void **)&handle);
    if (NULL == handle)
    {
        inst->Handle = 0;
        return;
    }

    memset(&handle->data, 0, sizeof(handle->data));
    handle->self = handle;

    exos_log_init(&handle->logger, "gros_topics_typ_0");

    
    
    exos_datamodel_handle_t *ros_topics_typ_datamodel = &handle->ros_topics_typ_datamodel;
    exos_dataset_handle_t *odemetry_dataset = &handle->odemetry_dataset;
    exos_dataset_handle_t *twist_dataset = &handle->twist_dataset;
    exos_dataset_handle_t *config_dataset = &handle->config_dataset;
    exos_dataset_handle_t *encoder_dataset = &handle->encoder_dataset;
    exos_dataset_handle_t *vaccumtopic = &handle->vaccumtopic;
    exos_dataset_handle_t *linefollow = &handle->linefollow;
    exos_dataset_handle_t *linestatus = &handle->linestatus;
    EXOS_ASSERT_OK(exos_datamodel_init(ros_topics_typ_datamodel, "ros_topics_typ_0", "gros_topics_typ_0"));

    EXOS_ASSERT_OK(exos_dataset_init(odemetry_dataset, ros_topics_typ_datamodel, "odemetry", &handle->data.odemetry, sizeof(handle->data.odemetry)));
    EXOS_ASSERT_OK(exos_dataset_init(twist_dataset, ros_topics_typ_datamodel, "twist", &handle->data.twist, sizeof(handle->data.twist)));
    EXOS_ASSERT_OK(exos_dataset_init(config_dataset, ros_topics_typ_datamodel, "config", &handle->data.config, sizeof(handle->data.config)));
    EXOS_ASSERT_OK(exos_dataset_init(encoder_dataset, ros_topics_typ_datamodel, "encoder", &handle->data.encoder, sizeof(handle->data.encoder)));
    EXOS_ASSERT_OK(exos_dataset_init(vaccumtopic, ros_topics_typ_datamodel, "vaccumTopic", &handle->data.vaccumTopic, sizeof(handle->data.vaccumTopic)));
    EXOS_ASSERT_OK(exos_dataset_init(linefollow, ros_topics_typ_datamodel, "lineFollow", &handle->data.lineFollow, sizeof(handle->data.lineFollow)));
    EXOS_ASSERT_OK(exos_dataset_init(linestatus, ros_topics_typ_datamodel, "lineStatus", &handle->data.lineStatus, sizeof(handle->data.lineStatus)));
    
    inst->Handle = (UDINT)handle;
}

_BUR_PUBLIC void ros_topics_typCyclic(struct ros_topics_typCyclic *inst)
{
    ros_topics_typHandle_t *handle = (ros_topics_typHandle_t *)inst->Handle;

    inst->Error = false;
    if (NULL == handle || NULL == inst->pros_topics_typ)
    {
        inst->Error = true;
        return;
    }
    if ((void *)handle != handle->self)
    {
        inst->Error = true;
        return;
    }

    ros_topics_typ *data = &handle->data;
    exos_datamodel_handle_t *ros_topics_typ_datamodel = &handle->ros_topics_typ_datamodel;
    //the user context of the datamodel points to the ros_topics_typCyclic instance
    ros_topics_typ_datamodel->user_context = inst; //set it cyclically in case the program using the FUB is retransferred
    ros_topics_typ_datamodel->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != ros_topics_typ_datamodel->datamodel_event_callback && ros_topics_typ_datamodel->datamodel_event_callback != datamodelEvent)
    {
        ros_topics_typ_datamodel->datamodel_event_callback = datamodelEvent;
        exos_log_delete(&handle->logger);
        exos_log_init(&handle->logger, "gros_topics_typ_0");
    }

    exos_dataset_handle_t *odemetry_dataset = &handle->odemetry_dataset;
    odemetry_dataset->user_context = NULL; //user defined
    odemetry_dataset->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != odemetry_dataset->dataset_event_callback && odemetry_dataset->dataset_event_callback != datasetEvent)
    {
        odemetry_dataset->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *twist_dataset = &handle->twist_dataset;
    twist_dataset->user_context = NULL; //user defined
    twist_dataset->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != twist_dataset->dataset_event_callback && twist_dataset->dataset_event_callback != datasetEvent)
    {
        twist_dataset->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *config_dataset = &handle->config_dataset;
    config_dataset->user_context = NULL; //user defined
    config_dataset->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != config_dataset->dataset_event_callback && config_dataset->dataset_event_callback != datasetEvent)
    {
        config_dataset->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *encoder_dataset = &handle->encoder_dataset;
    encoder_dataset->user_context = NULL; //user defined
    encoder_dataset->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != encoder_dataset->dataset_event_callback && encoder_dataset->dataset_event_callback != datasetEvent)
    {
        encoder_dataset->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *vaccumtopic = &handle->vaccumtopic;
    vaccumtopic->user_context = NULL; //user defined
    vaccumtopic->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != vaccumtopic->dataset_event_callback && vaccumtopic->dataset_event_callback != datasetEvent)
    {
        vaccumtopic->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *linefollow = &handle->linefollow;
    linefollow->user_context = NULL; //user defined
    linefollow->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != linefollow->dataset_event_callback && linefollow->dataset_event_callback != datasetEvent)
    {
        linefollow->dataset_event_callback = datasetEvent;
    }

    exos_dataset_handle_t *linestatus = &handle->linestatus;
    linestatus->user_context = NULL; //user defined
    linestatus->user_tag = 0; //user defined
    //handle online download of the library
    if(NULL != linestatus->dataset_event_callback && linestatus->dataset_event_callback != datasetEvent)
    {
        linestatus->dataset_event_callback = datasetEvent;
    }

    //unregister on disable
    if (inst->_state && !inst->Enable)
    {
        inst->_state = 255;
    }

    switch (inst->_state)
    {
    case 0:
        inst->Disconnected = 1;
        inst->Connected = 0;
        inst->Operational = 0;
        inst->Aborted = 0;

        if (inst->Enable)
        {
            inst->_state = 10;
        }
        break;

    case 10:
        inst->_state = 100;

        SUCCESS("starting ros_topics_typ application..");

        //connect the datamodel, then the datasets
        EXOS_ASSERT_OK(exos_datamodel_connect_ros_topics_typ(ros_topics_typ_datamodel, datamodelEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(odemetry_dataset, EXOS_DATASET_PUBLISH, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(twist_dataset, EXOS_DATASET_SUBSCRIBE, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(config_dataset, EXOS_DATASET_SUBSCRIBE, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(encoder_dataset, EXOS_DATASET_PUBLISH, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(vaccumtopic, EXOS_DATASET_SUBSCRIBE, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(linefollow, EXOS_DATASET_SUBSCRIBE, datasetEvent));
        EXOS_ASSERT_OK(exos_dataset_connect(linestatus, EXOS_DATASET_PUBLISH, datasetEvent));

        inst->Active = true;
        break;

    case 100:
    case 101:
        if (inst->Start)
        {
            if (inst->_state == 100)
            {
                EXOS_ASSERT_OK(exos_datamodel_set_operational(ros_topics_typ_datamodel));
                inst->_state = 101;
            }
        }
        else
        {
            inst->_state = 100;
        }

        EXOS_ASSERT_OK(exos_datamodel_process(ros_topics_typ_datamodel));
        //put your cyclic code here!

         if (exos_datamodel_get_nettime(odemetry_dataset->datamodel)- test > 28500 || exos_datamodel_get_nettime(odemetry_dataset->datamodel)- test < -28500  )
        {
            
            test = exos_datamodel_get_nettime(odemetry_dataset->datamodel);
            memcpy(&data->odemetry, &inst->pros_topics_typ->odemetry, sizeof(data->odemetry));
            exos_dataset_publish(odemetry_dataset);

            
            
        }

        if (exos_datamodel_get_nettime(odemetry_dataset->datamodel)- test2 > 1000000 || exos_datamodel_get_nettime(odemetry_dataset->datamodel)- test2 < -1000000  )
        {
            
            test2 = exos_datamodel_get_nettime(odemetry_dataset->datamodel);
            

            if (0 != memcmp(&inst->pros_topics_typ->lineStatus, &data->lineStatus, sizeof(data->lineStatus)))
            {
            memcpy(&data->lineStatus, &inst->pros_topics_typ->lineStatus, sizeof(data->lineStatus));
            exos_dataset_publish(linestatus);
            }
            
        }
         
        //}
        //}
        
        //publish the encoder_dataset dataset as soon as there are changes
        
        if (0 != memcmp(&inst->pros_topics_typ->encoder, &data->encoder, sizeof(data->encoder)))
        {
            memcpy(&data->encoder, &inst->pros_topics_typ->encoder, sizeof(data->encoder));
            exos_dataset_publish(encoder_dataset);
        }
        //publish the linestatus dataset as soon as there are changes
       

        break;

    case 255:
        //disconnect the datamodel
        EXOS_ASSERT_OK(exos_datamodel_disconnect(ros_topics_typ_datamodel));

        inst->Active = false;
        inst->_state = 254;
        //no break

    case 254:
        if (!inst->Enable)
            inst->_state = 0;
        break;
    }

    exos_log_process(&handle->logger);

}

_BUR_PUBLIC void ros_topics_typExit(struct ros_topics_typExit *inst)
{
    ros_topics_typHandle_t *handle = (ros_topics_typHandle_t *)inst->Handle;

    if (NULL == handle)
    {
        ERROR("ros_topics_typExit: NULL handle, cannot delete resources");
        return;
    }
    if ((void *)handle != handle->self)
    {
        ERROR("ros_topics_typExit: invalid handle, cannot delete resources");
        return;
    }

    exos_datamodel_handle_t *ros_topics_typ_datamodel = &handle->ros_topics_typ_datamodel;

    EXOS_ASSERT_OK(exos_datamodel_delete(ros_topics_typ_datamodel));

    //finish with deleting the log
    exos_log_delete(&handle->logger);
    //free the allocated handle
    TMP_free(sizeof(ros_topics_typHandle_t), (void *)handle);
}

