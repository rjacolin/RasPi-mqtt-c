/*
 * json_deserializer.h
 *
 *  Created on: Jun 9, 2014
 *      Author: rjacolin
 */

#ifndef JSON_DESERIALIZER_H_
#define JSON_DESERIALIZER_H_

#include <stdint.h>
#include <stdbool.h>

#define BS_MEM_ALLOC( size) malloc( size)

typedef enum bsd_status_t {
  BSD_EOK = 0,
  BSD_ENOTIMPL = -1,    /* not implemented */
  BSD_EINVALID = -2,    /* invalid stream (syntax error, ...) */
  BSD_EBADCONTEXT = -3, /* Decoding context is unknown/invalid */
  BSD_EINVOPCODE = -4,  /* used an unknown opcode */
  BSD_EBADCLASSID = -5, /* used an unknown class identifier */
  BSD_ETOODEEP = -6,    /* too many nested conainers */
  BSD_EMEMORY = -7,     /* out of memory */
  BSD_EINTERNAL = -100, /* internal error, should not happen */
} bsd_status_t;

typedef struct bsd_data_t {
  enum bsd_data_type_t {
    BSD_ERROR, // 0
    BSD_CLOSE,
    BSD_NULL,
    BSD_INT,
    BSD_BOOL,
    BSD_DOUBLE, // 5
    BSD_STRING,
    BSD_CHUNKED_STRING,
    BSD_CHUNK,
    BSD_LIST,
    BSD_ZLIST, // 10
    BSD_MAP,
    BSD_ZMAP,
    BSD_OBJECT,
    BSD_CLASSDEF,
  } kind;

  const char *fieldname;
  union bsd_data_content_t {
    int64_t i;
    bool boolean;
    double d;
    size_t length;
    enum bsd_data_type_t cont_type;
    bsd_status_t error;
    struct {
      size_t length;
      const char *data;
    } string, chunk;
  } content;
} bsd_data_t;

/**
 * sdb_sendCallbackInfo_t structure received by user.
 *
 * @parm ticketId   : acknowledgment number for previous messages. 0 means no ACK expected by the server.
 * @param path       : root path of message
 * @param nbofvalues : number of values stored in the arrays below
 * @param keys		 : arrays of key value
 * @param values     : arrays of data value. The deserialized data is stored in the bsd_data_t structure
 */
typedef struct mqtt_callBackData_t {  ///<  for DATA
	int ticketId;
	char *path;
	int nbofvalues;
	char **keys;
	struct bsd_data_t *values[];
} mqtt_data_t;

mqtt_data_t json_deserialize(char* payload);
void json_free(mqtt_data_t);

#endif /* JSON_DESERIALIZER_H_ */
