CC=gcc
CFLAGS=-c -Wall -Iinc -Iinc/mqtt
LDFLAGS=-lpthread
SOURCES=src/environment.c src/stdoutsub.c src/MQTTClient.c src/mqtt/MQTTConnectClient.c \
src/mqtt/MQTTConnectServer.c src/mqtt/MQTTUnsubscribeClient.c src/MQTTLinux.c \
src/mqtt/MQTTUnsubscribeServer.c src/mqtt/MQTTSerializePublish.c src/mqtt/MQTTSubscribeClient.c \
src/mqtt/MQTTDeserializePublish.c src/mqtt/MQTTSubscribeServer.c src/mqtt/MQTTPacket.c \
src/json/json.c src/json/json_serializer.c src/json/json_deserializer.c src/json/log.c \
src/json/jsmn.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=greenhouse

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@