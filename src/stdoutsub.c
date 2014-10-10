/*******************************************************************************
 * Copyright (c) 2012, 2013 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *    Ian Craggs - change delimiter option from char to string
 *    Al Stockdill-Mander - Version using the embedded C client
 *******************************************************************************/

/*
 
 Greenhouse demo for RaspberryPi

*/
#include <stdio.h>
#include "MQTTClient.h"
#include "json/json_deserializer.h"
#include "json/json_serializer.h"

#include <stdio.h>
#include <signal.h>
#include <memory.h>

#include <sys/time.h>
#include <pthread.h>

#define READING_MESURES_PERIOD 		10  /* in seconds */
#define UPLOADING_MESURES_PERIOD 	30  /* in seconds */

char payload[1024];
static char deviceId[] = "00000000B6AF4A9D";

pthread_t reading_tid;
pthread_t uploading_tid;

volatile int toStop = 0;

/* events from the environment */
int getTemperature();
double getHumidity();
double getLuminosity();
int getTimeStamp();

/* variables reserve for parsing the messages comming from the server*/
static bool dataFailure = 0; /* Failure to parse a data message */
static bool lightState  = 0; /* Initially off */

int temp_ts[1024];
char* temp_values[1024];
int temp_size = 0;

int hum_ts[1024];
char* hum_values[1024];
int hum_size = 0;


void mqtt_publish(char*, int);

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

int readMeasures()
{
	int temperature   = getTemperature();
	double humidity   = getHumidity();
	double luminosity = getLuminosity() ;

	char str[250];
	sprintf(str," temperature=%d, humidity=%lf, luminosity=%lf",  temperature, humidity, luminosity);
	printf("[Entry_point] Reading measures : %s\n", str);

	/*payload =
		"["
	    "{"
	    "\"machine.temperature\": [{"
	    "  \"timestamp\" : %d, "
	    "  \"value\" : \"%lf\""
	    "}]"
	    "}"
	    "]";*/
	int timestamp = getTimeStamp();
	temp_ts[temp_size] = timestamp;
	temp_values[temp_size] = malloc(10);
	sprintf(temp_values[temp_size], "%d", temperature);
	temp_size++;

	hum_ts[hum_size] = timestamp;
	hum_values[hum_size] = malloc(10);
	sprintf(hum_values[hum_size], "%lf", humidity);
	hum_size++;

	return 0;
}

void* readMeasuresHandler(void *arg)
{
	while(!toStop)
	{
		int r = readMeasures();
		if (r) printf("[entry_point] Error Feed data to the table [status=%d]\n", r);
		sleep(30);	
	}
	return NULL;
}

void* uploadMeasuresHandler(void *arg)
{
	Client* c = (Client*) arg;
	sleep(60);
	while (!toStop)
	{
		char temp_payload[1024];
		// Push values using full data path.
		json_data(temp_payload, "machine.temperature", temp_ts, temp_values, temp_size);
		char hum_payload[1024];
		json_data(hum_payload, "machine.humidity", hum_ts, hum_values, hum_size);
		sprintf(payload, "%s, %s", temp_payload, hum_payload);
		char* buffer = json_encapsulate(payload);
		printf("[SAMPLES] %s\n", buffer);

		MQTTMessage msg;
		msg.qos = QOS0;
		msg.retained = 0;
		msg.dup = 0;
		msg.id = 0;
		msg.payload = buffer;
		msg.payloadlen = strlen(buffer);

		char* topic = malloc(15+strlen(deviceId));
		sprintf(topic, "%s/messages/json", deviceId);
		printf("Publish on %s\n", topic);

		int rc = MQTTPublish(c, topic, &msg);
		if(rc != 0)
			printf("publish error: %d\n", rc);

		free(buffer);
		hum_size = 0;
		temp_size = 0;
		sleep(60);
	}
	return NULL;
}

/****************************************************************/
/*  Object     : Parse the message value coming from the server */
/****************************************************************/
void onIncomingData(char *path, char** keys, bsd_data_t **values, size_t nbofvalues){
	printf("[callBackFct]     PATH = %s (%d) \r\n", path, nbofvalues);
	int i;
	for(i=0; i < nbofvalues; i++)
	{
		if( ! strcmp( keys[i], "machine.TurnOn") )
		{
			if( values[i]->content.boolean != lightState)
				printf("[callBackFct]     Light is now %s \r\n", values[i]->content.boolean == true ? "ON" : "OFF");
		}

		else { /* Data path not handled */
	        	dataFailure = 1;
	    }
	}
}

void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;

	printf("%.*s%s", (int)message->payloadlen, (char*)message->payload, " ");
	char payload[1024];
	sprintf(payload, "%.*s%s", (int)message->payloadlen, (char*)message->payload, " ");

	printf("\r\n[callBackFct] Callback user is called\r\n");
	mqtt_data_t data = json_deserialize(payload);

//	if(data != NULL)
//	{
		onIncomingData( data.path, data.keys, data.values, data.nbofvalues);

		/*acknowledge to the server after receiving data*/
		//onDataAcknowledgment(data.ticketId);
//	}
	json_free(data);
	printf("[callBackFct] End callback user  \r\n\n\n\n");
}


int main(int argc, char** argv)
{
	int rc = 0;
	unsigned char buf[1024];
	unsigned char readbuf[1024];

	Network n;
	Client c;

	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);

	NewNetwork(&n);
	ConnectNetwork(&n, "eu.airvantage.net", 1883);
	MQTTClient(&c, &n, 1000, buf, 1024, readbuf, 1024);
 
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = deviceId;
	data.username.cstring = "00000000B6AF4A9D";
	data.password.cstring = "toto";

	data.keepAliveInterval = 10;
	data.cleansession = 1;
	printf("Connecting to %s %d\n", "tcp://eu.airvantage.net", 1883);
	
	rc = MQTTConnect(&c, &data);
	printf("Connected %d\n", rc);
    

	char* topic = malloc(strlen(deviceId)+13);
	sprintf(topic, "%s/tasks/json", deviceId);

        printf("Subscribing to %s\n", topic);
	rc = MQTTSubscribe(&c, topic, 0, messageArrived);
	printf("Subscribed %d\n", rc);

	int err1 = pthread_create(&reading_tid, NULL, &readMeasuresHandler, (void*)&c);
	int err2 = pthread_create(&uploading_tid, NULL, &uploadMeasuresHandler, (void*) &c);
	if(err1 != 0 || err2 != 0)
		printf("Can't create simulator (%s, %d)\n", strerror(err1), err2);
	else
	{
		while (!toStop)
		{
			MQTTYield(&c, 1000);	
		}
	}
	
	printf("Stopping\n");

	MQTTDisconnect(&c);
	n.disconnect(&n);

	return 0;
}


