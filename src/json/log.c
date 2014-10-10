/*
 * log.c
 *
 *  Created on: Jun 9, 2014
 *      Author: rjacolin
 */

#include <stdarg.h>
#include <stdio.h>
#include "json/log.h"

void log_die(char *msg, ...)
{
    va_list argp;

    log_null(msg);

    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);

    fprintf(stderr, "\n");
    abort();
}
