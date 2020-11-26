#include "mbed.h"
#include <sstream>
#include <string>

const unsigned char msg_frame[] = {0x7E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00};
const unsigned int chksm_1 = 0x20D;

unsigned int chksm = 0x00;

static char transmit_msg[128];
std::string packet;


void transmit(char msg[])
{
    xbee.putc(msg_frame[0]);
    xbee.putc(0x00);
    xbee.putc(14+strlen(msg));
    for (int x=1; x<15 ; x++) {
        xbee.putc(msg_frame[x]);
    }

    xbee.printf(msg);
    // Calculate checksum for message data
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
    //transmit_msg = tempstr.c_str();
    std::strcpy (transmit_msg, tempstr.c_str());
}

void clear_msg(void)
{
    memset(transmit_msg,0, strlen(transmit_msg));
    packet.clear();
}
