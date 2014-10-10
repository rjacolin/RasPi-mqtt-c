/*
 * json_serializer.h
 *
 *  Created on: Apr 24, 2014
 *      Author: rjacolin
 */

#ifndef JSON_SERIALIZER_H_
#define JSON_SERIALIZER_H_

char* json_valueList(char* buffer, int*timestamps, char**values, int nbOfValues);
char* json_data(char* buffer, char* name, int*timestamps, char**values, int nbOfValues);
char* json_encapsulate(char* payload);

#endif /* JSON_SERIALIZER_H_ */
