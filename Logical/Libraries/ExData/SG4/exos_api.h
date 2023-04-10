/*
Copyright (C) 2021 B&R Danmark
All rights reserved
*/

#ifndef _EXOS_API_H_
#define _EXOS_API_H_
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/**
 * The version of the exos api
 */
#define EXOS_API_VERSION "2.1.0-1"
/*
NUMERIC version
       0 -       9999 build revision
   10000 -      90000 1-9 patch version (limited from AS)
  100000 -    9900000 1-99 minor version
10000000 -  990000000 1-99 major version
*/
#define EXOS_API_NUMERIC_VERSION 20100001

/*Maximum size for a dataset*/
#define EXOS_API_MAX_DATASET_SIZE 200000

#ifdef _SG4
#include <bur/plctypes.h>
#else
#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC __attribute__((visibility("default")))
#endif
#endif

#define EXOS_ARRAY_DEPTH 10

typedef struct exos_dataset_info
{
    const char *name;
    void *adr;
    size_t size;
    long offset;
    uint32_t arrayItems[EXOS_ARRAY_DEPTH];

} exos_dataset_info_t;

#define EXOS_DATASET_BROWSE_NAME_INIT "", &(data), sizeof(data), 0
#define EXOS_DATASET_BROWSE_NAME(_arg_) #_arg_, &(data._arg_), sizeof(data._arg_), 0

typedef enum
{
    EXOS_ERROR_OK = 0,
    EXOS_ERROR_NOT_IMPLEMENTED = 5000,
    EXOS_ERROR_PARAMETER_NULL,
    EXOS_ERROR_BAD_DATAMODEL_HANDLE,
    EXOS_ERROR_BAD_DATASET_HANDLE,
    EXOS_ERROR_BAD_LOG_HANDLE,
    EXOS_ERROR_BAD_SYNC_HANDLE,
    EXOS_ERROR_NOT_ALLOWED,
    EXOS_ERROR_NOT_FOUND,
    EXOS_ERROR_STRING_FORMAT,
    EXOS_ERROR_MESSAGE_FORMAT,
    EXOS_ERROR_NO_DATA,
    EXOS_ERROR_BUFFER_OVERFLOW,
    EXOS_ERROR_TIMEOUT,
    EXOS_ERROR_BAD_DATASET_SIZE,
    EXOS_ERROR_USER,
    EXOS_ERROR_SYSTEM,
    EXOS_ERROR_SYSTEM_SOCKET,
    EXOS_ERROR_SYSTEM_SOCKET_USAGE,
    EXOS_ERROR_SYSTEM_MALLOC,
    EXOS_ERROR_SYSTEM_LXI,
    EXOS_ERROR_DMR_NOT_READY,
    EXOS_ERROR_DMR_SHUTDOWN,
    EXOS_ERROR_BAD_STATE
} EXOS_ERROR_CODE;

/**
 * String representation of an error
 *
 * With the function `exos_get_error_string()`, a textual (ASCII string) representation of an error code can be aquired.
*/
char *exos_get_error_string(EXOS_ERROR_CODE error);

typedef enum
{
    EXOS_STATE_DISCONNECTED,
    EXOS_STATE_CONNECTED,
    EXOS_STATE_OPERATIONAL,
    EXOS_STATE_ABORTED
} EXOS_CONNECTION_STATE;

/**
 * String representation of a state
 * 
 * With the function `exos_get_state_string()`, a textual (ASCII string) representation of a connection state can be aquired.
*/
char *exos_get_state_string(EXOS_CONNECTION_STATE state);

typedef enum
{
    EXOS_DATAMODEL_EVENT_CONNECTION_CHANGED,
    EXOS_DATAMODEL_EVENT_SYNC_STATE_CHANGED
} EXOS_DATAMODEL_EVENT_TYPE;

typedef struct exos_datamodel_handle exos_datamodel_handle_t;

/**
 * Callback function for datamodel events
 * 
 * Currently, the callback only triggers one event_type, `EXOS_DATAMODEL_EVENT_CONNECTION_CHANGE`, which comes at a state change of the dataset.
 * Here, it makes sense to query the `datamodel->connection_state` in order to get the current state of the connection.
*/
typedef void (*exos_datamodel_event_cb)(exos_datamodel_handle_t *datamodel, const EXOS_DATAMODEL_EVENT_TYPE event_type, void *info);

/**
 * Initialize the datamodel handle
 * 
 * This function intializes (resets) a datamodel handle and gives it a `user_alias` via a `datamodel_instance_name`.
 * The datamodel handle is then used for receiving incoming messages using the `exos_datamodel_process()`
*/
EXOS_ERROR_CODE exos_datamodel_init(exos_datamodel_handle_t *datamodel, const char *datamodel_instance_name, const char *user_alias);

/**
 * (internal) Calculate dataset_info_t offsets from the EXOS_DATASET_BROWSE_NAME macros
 * 
 * The offsets of the various datasets of a datamodel are entered in a exos_dataset_info_t list, basically pointing out the start address of the given dataset.
 * This function creates offsets of the absolute addresses by subtracting the first (info[0]) dataset info from the absolute address
 * The function is "internal" meaning it shouldnt be used in an application unless there are specific reasons to do so. Applications should refer to the automatically generated headerfile.
*/
void exos_datamodel_calc_dataset_info(exos_dataset_info_t *info, size_t info_size);

/**
 * (internal) Connect a datamodel to the Dataset Message Router
 * 
 * The connect function used in an application is code generated from a `.typ` file and will have the name of the specified main data structure of the datamodel.
 * 
 * For example, lets say the MyApp datamodel uses a MyApp.typ file, and in that file, the main datamodel structure is called MyApplication.
 * In that case the generated header (`exos_myapplication.h`) will declare the function `exos_datamodel_connect_myapplication()`
 * 
 * This function is the "raw" version, which provides a JSON template (describing the datamodel) and a list of exos_dataset_info_t describing the offsets of the datasets within the datamodel
 * The function is "internal" meaning it shouldnt be used in an application unless there are specific reasons to do so. Applications should refer to the automatically generated headerfile.
*/
EXOS_ERROR_CODE exos_datamodel_connect(exos_datamodel_handle_t *datamodel, const char *config, const exos_dataset_info_t *info, size_t info_size, exos_datamodel_event_cb datamodel_event_callback);

typedef enum
{
    EXOS_DATAMODEL_PROCESS_BLOCKING,
    EXOS_DATAMODEL_PROCESS_NON_BLOCKING
} EXOS_DATAMODEL_PROCESS_MODE;

EXOS_ERROR_CODE exos_datamodel_set_process_mode(exos_datamodel_handle_t *datamodel, EXOS_DATAMODEL_PROCESS_MODE mode);

/**
 * Set the datamodel into OPERATIONAL state
 * 
 * The `EXOS_STATE_OPERATIONAL` is merely to provide a built-in feature to distinguish between an operational and a preoperational state from an application perspective.
 * If there is no need for this distinction, then an application can suffice with using the `EXOS_STATE_CONNECTED` as "active" state. 
 * 
*/
EXOS_ERROR_CODE exos_datamodel_set_operational(exos_datamodel_handle_t *datamodel);

/**
 * Disconnect a datamodel from the Dataset Message Router
 * 
 * Triggers the `EXOS_STATE_DISCONNECTED` event on the datamodel before removing the callback instruction.
 * Here all internal ZMQ sockets are closed and there is no data transfer between the application and the server after this function has been called.
 * Before closing, however, the application sends a disconnection message to the *Dataset Message Router*.
 * 
 * All datasets assigned to the datamodel will be automatically disconnected (and receive a respective `EXOS_STATE_DISCONNECTED` event) 
*/
EXOS_ERROR_CODE exos_datamodel_disconnect(exos_datamodel_handle_t *datamodel);

/**
 * Release all resources from a datamodel (and disconnect from the Dataset Message Router)
 * 
 * This function will free up all allocated memory allocated for this datamodel, including all assigned datasets.
 * If a connection to the *Dataset Message Router* is active, it will be disconnected, independent of the current datamodel state.
*/
EXOS_ERROR_CODE exos_datamodel_delete(exos_datamodel_handle_t *datamodel);

typedef struct exos_datamodel_sync_info
{
    bool in_sync;
    bool _reserved_bool[8];
    uint32_t missed_dmr_cycles;
    uint32_t missed_ar_cycles;
    EXOS_DATAMODEL_PROCESS_MODE process_mode;
    uint32_t _reserved_uint32[7];
} exos_datamodel_sync_info_t;

typedef struct exos_datamodel_private
{
    uint32_t _magic;
    void *_artefact;
    void *_reserved[8];
} exos_datamodel_private_t;

struct exos_datamodel_handle
{
    char *name;
    EXOS_CONNECTION_STATE connection_state;
    EXOS_ERROR_CODE error;
    void *user_context;
    long user_tag;
    char *user_alias;
    exos_datamodel_event_cb datamodel_event_callback;
    exos_datamodel_sync_info_t sync_info;
    bool _reserved_bool[8];
    uint32_t _reserved_uint32[8];
    void *_reserved_void[8];
    exos_datamodel_private_t _private;
};

typedef struct exos_dataset_handle exos_dataset_handle_t;

/**
 * Iterate a datamodel for attached datasets
 * 
 * In some situations it might be necessary to query which datasets are attached to the datamodel, which is where `exos_datamodel_get_dataset()` can be used.
 * It is an iterator function that returns NULL when there are no more datasets.
 * You start the iteration by setting the `previous` dataset to NULL. 
*/
exos_dataset_handle_t *exos_datamodel_get_dataset(exos_datamodel_handle_t *datamodel, exos_dataset_handle_t *previous);

/**
 * Cyclic main function - poll datamodel for incoming messages
 * 
 * The `exos_datamodel_process()` polls incoming messages in a blocking manner in Linux, where it synchronizes with the loop of the *Dataset Message Router*, 
 * which - in a shared memory connection - is synchronized with a configured AR task class. 
 * 
 * If there are many incoming messages, for example a lot of dataset updates, the `exos_datamodel_process()` will read out all pending messages and 
 * call the registered callbacks from the context it is called, until there are no more messages.
*/
EXOS_ERROR_CODE exos_datamodel_process(exos_datamodel_handle_t *datamodel);

/**
 * 
 * Get the current AR NETTIME (synchronized)
 * 
 * This function returns the current AR NETTIME (extrapolated in Linux)
*/
int32_t exos_datamodel_get_nettime(exos_datamodel_handle_t *datamodel);

typedef enum
{
    EXOS_DATASET_EVENT_CONNECTION_CHANGED,
    EXOS_DATASET_EVENT_UPDATED,
    EXOS_DATASET_EVENT_PUBLISHED,
    EXOS_DATASET_EVENT_DELIVERED
    //EXOS_DATASET_RECIEVED
} EXOS_DATASET_EVENT_TYPE;

typedef struct exos_buffer_info
{
    uint32_t size;
    uint32_t free;
    uint32_t used;
} exos_send_buffer_info_t;

typedef void (*exos_dataset_event_cb)(exos_dataset_handle_t *dataset, EXOS_DATASET_EVENT_TYPE event_type, void *info);

typedef enum
{
    EXOS_DATASET_SUBSCRIBE = 1,
    EXOS_DATASET_PUBLISH = 16
} EXOS_DATASET_TYPE;

typedef struct exos_dataset_private
{
    uint32_t _magic;
    void *_value;
    void *_reserved[8];
} exos_dataset_private_t;
struct exos_dataset_handle
{
    char *name;
    EXOS_DATASET_TYPE type;
    exos_datamodel_handle_t *datamodel;
    void *data;
    size_t size;
    EXOS_ERROR_CODE error;
    EXOS_CONNECTION_STATE connection_state;
    exos_send_buffer_info_t send_buffer;
    int32_t nettime;
    int32_t user_tag;
    void *user_context;
    exos_dataset_event_cb dataset_event_callback;
    bool _reserved_bool[8];
    uint32_t _reserved_uint32[8];
    void *_reserved_void[8];
    exos_dataset_private_t _private;
};

/**
 * Initialize a dataset handle and attach it to a datamodel
 * 
 * This function initializes the `exos_dataset_handle_t` structure, meaning it zeroes all members and sets artefact, data and size members.
*/
EXOS_ERROR_CODE exos_dataset_init(exos_dataset_handle_t *dataset, exos_datamodel_handle_t *datamodel, const char *browse_name, const void *data, size_t size);

/**
 * Connect a dataset to the Dataset Message Router and register an event callback
*/
EXOS_ERROR_CODE exos_dataset_connect(exos_dataset_handle_t *dataset, EXOS_DATASET_TYPE type, exos_dataset_event_cb dataset_event_callback);

/**
 * Publish a dataset
 * 
 * Publish (send) a dataset to other subscribers. The dataset needs to be connected to the *Dataset Message Router* as `EXOS_DATASET_PUBLISH` via the `exos_dataset_connect()` function first.
 * 
 * When publishing (sending) a value, the `data` pointer given in `exos_dataset_init()` is copied into a message (using the `size`).
*/
EXOS_ERROR_CODE exos_dataset_publish(exos_dataset_handle_t *dataset);

/**
 * Disconnect a dataset from the Dataset Message Router
 * 
 * This will disconnect the datasetEvent from the dataset (after receiving an `EXOS_STATE_DISCONNECTED` event), and no values can be published using this dataset. 
 * This function should be called in case the datamodel should stay connected, but the individual dataset should be disconnected.
*/
EXOS_ERROR_CODE exos_dataset_disconnect(exos_dataset_handle_t *dataset);

/**
 * Release all resources from a dataset (and disconnect from the Dataset Message Router)
 * 
 * Delete (and disconnect) a dataset and free up all allocated resources.
 * If the dataset is connected, it will be disconnected before being deleted.
*/
EXOS_ERROR_CODE exos_dataset_delete(exos_dataset_handle_t *dataset);

#endif
