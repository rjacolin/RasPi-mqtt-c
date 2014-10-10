/*
 * json.c
 *
 *  Created on: Jun 9, 2014
 *      Author: rjacolin
 */

#include "json/json.h"
#include "json/log.h"

#include <string.h>

#define BUFFER_SIZE 32768
#define JSON_TOKENS 256

jsmntok_t * json_tokenise(const char *js)
{
    jsmn_parser parser;
    jsmn_init(&parser);

    unsigned int n = JSON_TOKENS;
    jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * n);
    log_null(tokens);

    int ret = jsmn_parse(&parser, js, strlen(js), tokens, n);

    while (ret == JSMN_ERROR_NOMEM)
    {
        n = n * 2 + 1;
        tokens = realloc(tokens, sizeof(jsmntok_t) * n);
        log_null(tokens);
        ret = jsmn_parse(&parser, js, strlen(js), tokens, n);
    }

    if (ret == JSMN_ERROR_INVAL)
        log_die("jsmn_parse: invalid JSON string");
    if (ret == JSMN_ERROR_PART)
        log_die("jsmn_parse: truncated JSON string");

    return tokens;
}

bool json_token_streq(const char *js, jsmntok_t *t, char *s)
{
    return (strncmp(js + t->start, s, t->end - t->start) == 0
            && strlen(s) == (size_t) (t->end - t->start));
}

char * json_token_tostr(char *js, jsmntok_t *t)
{
    js[t->end] = '\0';
    return js + t->start;
}


