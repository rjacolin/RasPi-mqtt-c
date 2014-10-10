/*
 * json.h
 *
 *  Created on: Jun 9, 2014
 *      Author: rjacolin
 */

#ifndef JSON_H_
#define JSON_H_

#include "jsmn.h"
#include <stdbool.h>

jsmntok_t * json_tokenise(const char *js);
bool json_token_streq(const char *js, jsmntok_t *t, char *s);
char * json_token_tostr(char *js, jsmntok_t *t);


#endif /* JSON_H_ */
