#include "mbed.h"
#include <string>

  //PAN ID: 416772694E6574

const unsigned char msg_frame[] = {0x7E, 0x10, 0x00, 0x00, 0x00};
const unsigned char node_1[] = {0x00, 0x13, 0xA2, 0x00, 0x41, 0xC0, 0x2D, 0x7C, 0x23, 0xFF};
const unsigned char node_2[] = {0x00, 0x13, 0xA2, 0x00, 0x41, 0xC0, 0x29, 0x31, 0x84, 0x19};
const unsigned int chksm_1 = 0x10;

unsigned int chksm = 0x00;

static char transmit_msg[50];
std::string packet;


void transmit(char msg[], int node)
{
    xbee.putc(msg_frame[0]);
    xbee.putc(0x00);
    xbee.putc(14+strlen(msg));
    for (int x=1; x<3 ; x++) {
        xbee.putc(msg_frame[x]);
    }
    switch(node)
    {
    case 1:
        for (int x=0; x<10 ; x++) {
        xbee.putc(node_1[x]);
        chksm+= node_1[x];
        }   
    break;
    
    case 2:
        for (int x=0; x<10 ; x++) {
        xbee.putc(node_2[x]);
        chksm+= node_2[x];
        }   
    break;
    
    default:
    
    break;
    }
    
    for (int x=3; x<5 ; x++) {
        xbee.putc(msg_frame[x]);
        chksm+= msg_frame[x];
    }
    
    xbee.printf(msg);

    for (int x=0; x<strlen(msg); x++) {
        chksm += msg[x];
    }
    chksm += chksm_1;
    chksm = 0xFF&chksm;
    chksm = 0xFF - chksm;

    xbee.putc(chksm);
    chksm = 0;
}


char *translate(char *p)
{

    static char message[max_msg];
    memset(message, 0, max_msg);
    memcpy(message, (p+16), max_msg); //+
    message[max_msg-1] = '\0';
    return message;


}

template <typename T>
void add_msg(char type[], T data, bool end = false)
{
    std::string tempstr;
    tempstr += transmit_msg ;
    tempstr += type;
    std::stringstream val;
    val << data;
    tempstr += val.str() ;
    tempstr += " " ;
    if (end) {
        tempstr = tempstr + "\r\n";
    }
    std::strcpy (transmit_msg, tempstr.c_str());
}

void clear_msg(void)
{
    memset(transmit_msg,0, strlen(transmit_msg));
    packet.clear();
}
