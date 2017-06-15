#include<stdio.h>
#include<winsock2.h>
#include<time.h>
#pragma comment(lib,"ws2_32.lib")
#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4
// MQTT_VERSION : Pick the version
//#define MQTT_VERSION MQTT_VERSION_3_1
#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1
#endif

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 128
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 15
#endif

// MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT 3
#endif

// MQTT_MAX_TRANSFER_SIZE : limit how much data is passed to the network client
//  in each write call. Needed for the Arduino Wifi Shield. Leave undefined to
//  pass the entire MQTT packet in each write call.
//#define MQTT_MAX_TRANSFER_SIZE 80

// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5

#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)
UINT8 mqtt_message[127]; // byte array to store Mqtt packet
int MqttMessageLength = 0;
#define KEEPALIVE 15000
#define uint8_t UINT8
void delay(unsigned int);
#define uint16_t UINT16
int m=0;
   uint8_t buffer[MQTT_MAX_PACKET_SIZE];
   boolean write(uint8_t header, uint8_t* buf, uint16_t length);
   char* msg;
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
    int writeString(const char*, uint8_t*, uint16_t);


 uint16_t nextMsgId;

int Mqtt_Connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage)
    {

    uint16_t length = 5;
    unsigned int j;
    uint16_t user_length,pass_length,will_length,will_topic_length;
#if MQTT_VERSION == MQTT_VERSION_3_1
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
            for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                buffer[length++] = d[j];
            }
            uint8_t v;

            if (willTopic) {
                v = 0x06|(willQos<<3)|(willRetain<<5);
                will_length=strlen(willMessage);
                will_topic_length=strlen(willTopic);
            } else {
                v = 0x02;
                will_length=0;
                will_topic_length=0;

            }
            if(user != NULL) {
                v = v|0x80;

                if(pass != NULL) {
                    v = v|(0x80>>1);
                }
            }
            buffer[length++] = v;
            buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
            buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);
            length = writeString(id,buffer,length);
            if (willTopic) {
                length = writeString(willTopic,buffer,length);
                length = writeString(willMessage,buffer,length);
            }

            if(user != NULL) {
                length = writeString(user,buffer,length);
                user_length=strlen(user);
                if(pass != NULL) {
                    length = writeString(pass,buffer,length);
                    pass_length=strlen(pass);
                }
                  write(MQTTCONNECT,buffer,length-5);

            //printf("%d",length);
            //buffer[0]=16;
           // m=strlen(id);
           // buffer[1]=MQTT_HEADER_VERSION_LENGTH+5+strlen(id)+user_length+pass_length+will_length+will_topic_length&0xFF;
            //printf("%d id \n %d user \n %d password \n %d will_message\n %d will_topic_len \n",strlen(id),user_length,pass_length,will_length,will_topic_length );
            //length=MQTT_HEADER_VERSION_LENGTH+5+strlen(id)+user_length+pass_length+will_length+will_topic_length;
            //printf("%d",length);

            uint8_t buf_2[length-3];
            printf("[");
            for(m=0;m<length-3;m++)
            {
                    //printf("%c",buffer[m]);
                    buf_2[m]=buffer[m+3];
                     printf("%02X",buf_2[m]);
            }
                 printf("]");
                if( send(s , buf_2 ,length-3 , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");

    return 1;
}

    }
    int mqtt_ping()
    {
        buffer[0]=192;
        buffer[1]=0;
        if( send(s , buffer ,2 , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Send OK");
    }
int MQTT_Publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained)
{
        uint16_t length = 5;
        length = writeString(topic,buffer,length);
        uint16_t i;
        for (i=0;i<plength;i++) {
            buffer[length++] = payload[i];
        }
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
        write(header,buffer,length-5);
        printf("%d",length);
        uint8_t buf_2[length-3];
            printf("[");
            for(m=0;m<length-3;m++)
            {
                    //printf("%c",buffer[m]);
                    buf_2[m]=buffer[m+3];
                     printf("%02X",buffer[m]);
            }
                 printf("]");

    if( send(s , buf_2 ,length-3 , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

}


void sock_sub(const char * topic)
    {
    uint8_t topic_length=strlen(topic);
	MqttMessageLength=topic_length;
	buffer[0] =128;
	buffer[1] =4+topic_length+1;
	buffer[2] =0;
	buffer[3] =0;
	buffer[4] =0;
	buffer[5] =topic_length;
	for(m=0;m<topic_length;m++)
	{
		buffer[6+m]=topic[m];
	}
	buffer[6+topic_length]=0;
	MqttMessageLength=7+topic_length;


 printf("\n[");
    for(m=0;m<MqttMessageLength;m++)
    {

        printf("%02X",buffer[m]);
    }
     printf("]");
       if( send(s , buffer ,MqttMessageLength, 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("\nSend OK");
    }
UINT8 MQTT_SUBS[12]={0x80,0x0A,0x00,0x00,0x00,0x05,0x6B,0x61,0x6E,0x6B,0x61,0x00};
BOOLEAN MQTT_Subscribe(const char* topic, uint8_t qos) {

    uint16_t length = 5;
    if(qos < 0 || qos > 1) {
        return 0;
    }
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, buffer,length);
        buffer[length++] = qos;

        write(MQTTSUBSCRIBE,buffer,length-5);
        printf("%d",length);
        printf("\n[");
        uint8_t buf_2[length-3];

            for(m=0;m<length-3;m++)
            {
                    buf_2[m]=buffer[m+3];
                     printf("%02X",buf_2[m]);
            }
        printf("]");
            if( send(s , buf_2 ,length-3, 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
 printf("\n[");
    for(m=0;m<12;m++)
    {

        printf("%02X",MQTT_SUBS[m]);
    }
     printf("]");
         printf("\n");
}


void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
boolean write(uint8_t header, uint8_t* buf, uint16_t length) {
    int i=0;
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint16_t rc;
    uint16_t len = length;
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0);

    buf[4-llen] = header;
    for (i=0;i<llen;i++) {
        buf[5-llen+i] = lenBuf[i];
    }
}
int writeString(const char* string, uint8_t* buf, uint16_t pos) {
    const char* idp = string;
    uint16_t i = 0;
    pos += 2;
    while (*idp) {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}

int main(int argc , char *argv[])
{

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");


    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(1883);

    //Connect to remote server

    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected");
    delay(2000);
    Mqtt_Connect("kankan01","kankan","10010110",NULL,0,0,NULL);
    delay(2000);

    printf("\n");
    //sock_sub("kankan420");
    MQTT_Subscribe("kankan420",1);
    delay(2000);
   for(m=0;m<50;m++)
   {
    mqtt_ping();
    delay(3000);
   }
    char * payload="kankan45";
    printf("\n");
    MQTT_Publish("hello123",payload,strlen(payload),1);
    delay(1000);
    printf("\n");

    return 0;
}
