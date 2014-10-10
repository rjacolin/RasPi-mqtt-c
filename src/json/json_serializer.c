/*
 * json_serializer.c
 *
 *  Created on: Apr 24, 2014
 *      Author: rjacolin
 */

#define BS_MEM_ALLOC( size) malloc( size)

char* json_valueList(char* buffer, int*timestamps, char**values, int nbOfValues)
{
	buffer = strcpy(buffer, "[");

	int i = 0;
	for(i = 0; i < nbOfValues; i++)
	{
		sprintf(buffer, "%s {\"timestamp\" : %ld000, \"value\" : \"%s\"}", buffer, timestamps[i], values[i]);
		if(i < nbOfValues - 1)
			buffer = strcat(buffer, ", ");
	}

	buffer = strncat(buffer, "]", 2);
	return buffer;
}


char* json_data(char* buffer, char* name, int*timestamps, char**values, int nbOfValues)
{
	char payload[1024];
	json_valueList(payload, timestamps, values, nbOfValues);
	sprintf(buffer, "{\"%s\": %s}", name, payload);
	return buffer;
}

char* json_encapsulate(char* buffer)
{
	char* payload = BS_MEM_ALLOC(strlen(buffer) + 2);
	sprintf(payload, "[%s]", buffer);
	return payload;
}
