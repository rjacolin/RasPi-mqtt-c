/*
 * json_deserializer.c
 *
 *  Created on: Jun 9, 2014
 *      Author: rjacolin
 */
#include <malloc.h>
#include <string.h>
#include "json/json_deserializer.h"
#include "json/jsmn.h"
#include "json/log.h"

char *KEYS[] = { "uid", "timestamp", "command", "id", "params" };


mqtt_data_t json_deserialize_command(jsmntok_t *tokens, char* payload, mqtt_data_t data_t);

/**
 *[{"uid":"3863fccf9a1a48af99c8bfcbf01c6b7f","timestamp":1401902872564,"command":{"id":"machine.TurnOn","params":{"Light":false}}}]
 *
 *
 * @return resultcode if negative value or number of tokens
 */
mqtt_data_t json_deserialize(char* payload)
{
	mqtt_data_t data_t;
    jsmntok_t * tokens = json_tokenise(payload);

    char* temp_uid = json_token_tostr(payload, &tokens[2]);
    char* uid = BS_MEM_ALLOC(strlen(temp_uid) + 1);
    memcpy(uid, temp_uid, strlen(temp_uid) + 1);

    char* temp_data = json_token_tostr(payload, &tokens[6]);
    char* data = BS_MEM_ALLOC(strlen(temp_data) + 1);
    memcpy(data, temp_data, strlen(temp_data) + 1);

    if(strcmp(data, "command") == 0)
    {
    	//data_t = json_deserialize_command(tokens, payload, data_t);
    	char* temp_name = json_token_tostr(payload, &tokens[9]);
    		char* name = BS_MEM_ALLOC(strlen(temp_name) + 1);
    		memcpy(name, temp_name, strlen(temp_name) + 1);

    		//data_t->ticketId = uid;

    		int max = (&tokens[11])->size;
    		printf("Command: %s (%d params)\r\n", name, max / 2);
    		int index = 0;

    		char** keys = BS_MEM_ALLOC(max / 2);
    		int key_index = 0;
    		for(index = 0; index < max; index+=2, key_index++)
    		{
    			char* temp_pname = json_token_tostr(payload, &tokens[12 + index]);
    			keys[key_index] = BS_MEM_ALLOC(strlen(temp_pname) + 1);
    			memcpy(keys[key_index], temp_pname, strlen(temp_pname) + 1);

    			char* temp = json_token_tostr(payload, &tokens[12 + index + 1]);
    			char* param_value = BS_MEM_ALLOC(strlen(temp) + 1);
    			memcpy(param_value, temp, strlen(temp) + 1);

    			printf("%s: %s\r\n", keys[key_index], param_value);
    			//x.fieldname = param_name;

    			bsd_data_t x;

    			if(strcmp(param_value, "true") == 0)
    			{
    				printf("param bool @ true\r\n");
    				x.kind = BSD_BOOL;
    				x.content.boolean = true;
    			}
    			else if(strcmp(param_value, "false") == 0)
    			{
    				printf("param bool @ false\r\n");
    				x.kind = BSD_BOOL;
    				x.content.boolean = false;
    			}
    			else
    			{
    				// Not supported
    	//    			x.content.string.length = strlen(param_value);
    	//    			x.content.string.data = param_value;
    			}

    			data_t.values[key_index] = BS_MEM_ALLOC(sizeof(x));
    			memcpy(data_t.values[key_index], &x, sizeof(x));
    		}
    		data_t.path = name;
    		data_t.nbofvalues = max / 2;
    		data_t.keys = keys;
    }

    free(tokens);
    free(uid);
    free(data);
    return data_t;
}

mqtt_data_t json_deserialize_command(jsmntok_t *tokens, char* payload, mqtt_data_t data_t)
{
	char* temp_name = json_token_tostr(payload, &tokens[9]);
	char* name = BS_MEM_ALLOC(strlen(temp_name) + 1);
	memcpy(name, temp_name, strlen(temp_name) + 1);

	//data_t->ticketId = uid;

	int max = (&tokens[11])->size;
	printf("Command: %s (%d params)\r\n", name, max / 2);
	int index = 0;

	char** keys = BS_MEM_ALLOC(max / 2);
	int key_index = 0;
	for(index = 0; index < max; index+=2, key_index++)
	{
		char* temp_pname = json_token_tostr(payload, &tokens[12 + index]);
		keys[key_index] = BS_MEM_ALLOC(strlen(temp_pname) + 1);
		memcpy(keys[key_index], temp_pname, strlen(temp_pname) + 1);

		char* temp = json_token_tostr(payload, &tokens[12 + index + 1]);
		char* param_value = BS_MEM_ALLOC(strlen(temp) + 1);
		memcpy(param_value, temp, strlen(temp) + 1);

		printf("%s: %s\r\n", keys[key_index], param_value);
		//x.fieldname = param_name;

		bsd_data_t x;

		if(strcmp(param_value, "true") == 0)
		{
			printf("param bool @ true\r\n");
			x.kind = BSD_BOOL;
			x.content.boolean = true;
		}
		else if(strcmp(param_value, "false") == 0)
		{
			printf("param bool @ false\r\n");
			x.kind = BSD_BOOL;
			x.content.boolean = false;
		}
		else
		{
			// Not supported
//    			x.content.string.length = strlen(param_value);
//    			x.content.string.data = param_value;
		}

		data_t.values[key_index] = BS_MEM_ALLOC(sizeof(x));
		memcpy(data_t.values[key_index], &x, sizeof(x));
	}
	data_t.path = name;
	data_t.nbofvalues = max / 2;
	data_t.keys = keys;
	return data_t;
}

void json_free(mqtt_data_t data_t)
{
	if(data_t.path != NULL)
		free(data_t.path);
	if(data_t.keys != NULL)
	{
//		int index = 0;
//		for(index = 0; index < data_t.nbofvalues; index++)
//		{
//			if(data_t.keys[index])
//				free(data_t.keys[index]);
//		}
		free(data_t.keys);
	}

}
