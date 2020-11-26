#include <string.h>
#include <ctype.h>
#include "C12832.h"


// Compiler configuration

//#define DEBUG

//#define BaseStation
#define SensorNode

#define Display

const unsigned int max_msg = 128;

// End of configuration

#ifdef SensorNode
#ifdef BaseStation
#undef BaseStation
#endif
#endif

#ifdef DEBUG
Serial usb(USBTX, USBRX);
#else
Serial xbee(p9,p10);
#endif

#ifdef Display
C12832 lcd(p5, p7, p6, p8, p11);
#endif

#ifdef BaseStation
#include "zigbee_c.h"
#endif

#ifdef SensorNode
#include "zigbee_n.h"
#endif


enum agriData { //Enumeration of AgriNet Data options
    ERRR,
    UUID,
    UTCS,
#ifdef BaseStation
    LBAT,
    PWRS,
    STMP,
    ATMP,
    MOIS,
    LIGT,
    PHLV,
    COND,
    HUMD,
    GPSC,
#endif
#ifdef SensorNode
    PING,
    BTRS,
    PDAT,
    PALL,
    LPMD,
    WAKE,
    BRST,
#endif
};



struct agriProtocol { //Structure for storing AgriNet commands
    uint16_t UUID;
    uint32_t UTCS;
#ifdef BaseStation
    bool LBAT;
    uint8_t PWRS;
    double STMP;
    double ATMP;
    double LIGT;
    double COND;
    double MOIS;
    double HUMD;
    double PHLV;
#endif
#ifdef SensorNode
    bool PDAT;
    bool WAKE;
    bool LPMD;
    bool PALL;
    bool PING;
    bool BTRS;
    bool BRST;
#endif
    //To be added

};

//Variables initialised

static char *command_ptr,
       *val_ptr;

static agriProtocol data;


agriData resolveString(char *s) //convert string to enum for processing
{

    if(strcmp(s,"UUID") == 0) {
        return UUID;
    }
    if(strcmp(s,"UTCS") == 0) {
        return UTCS;
    }
#ifdef BaseStation
    if(strcmp(s,"PWRS") == 0) {
        return PWRS;
    }
    if(strcmp(s,"LBAT") == 0) {
        return LBAT;
    }
    if(strcmp(s,"STMP") == 0) {
        return STMP;
    }
    if(strcmp(s,"ATMP") == 0) {
        return ATMP;
    }
    if(strcmp(s,"MOIS") == 0) {
        return MOIS;
    }
    if(strcmp(s,"LIGT") == 0) {
        return LIGT;
    }
    if(strcmp(s,"PHLV") == 0) {
        return PHLV;
    }
    if(strcmp(s,"COND") == 0) {
        return COND;
    }
    if(strcmp(s,"HUMD") == 0) {
        return HUMD;
    }
    if(strcmp(s,"GPSC") == 0) {
        return GPSC;
    }
#endif
#ifdef SensorNode
    if(strcmp(s,"PING") == 0) {
        return PING;
    }
    if(strcmp(s,"PDAT") == 0) {
        return PDAT;
    }
    if(strcmp(s,"BTRS") == 0) {
        return BTRS;
    }
    if(strcmp(s,"PALL") == 0) {
        return PALL;
    }
    if(strcmp(s,"LPMD") == 0) {
        return LPMD;
    }
    if(strcmp(s,"WAKE") == 0) {
        return WAKE;
    }
#endif
    //to be added

    return ERRR;
}

void reset(void) //reset pointers
{
    command_ptr = NULL;
    val_ptr = NULL;

#ifdef Display
    lcd.cls();
#endif

}

void parse(char *p ) //Parse data strcture
{
    char command[5];


#ifdef DEBUG
    usb.printf("Parsing\r\n");
#endif

    reset();

    for(int i=0; i<strlen(p); i++) {
        p[i] = toupper(p[i]); //Make all CAPS for consistency
    }

//First extraction

    command_ptr = strtok(p, " ");
#ifdef Display
    lcd.cls();
    int i = 0;
    int j = 5;
#endif
    while(command_ptr != NULL) {
#ifdef DEBUG
        int pcount = 1;
#endif
        strncpy(command, command_ptr, 4); //extract 4 digit command
        command[4] = '\0'; //append charcter
        switch (resolveString(command)) {

            case UUID:
                val_ptr = command_ptr + 4;
                data.UUID =  atol(val_ptr);
#ifdef DEBUG
                usb.printf("UUID: %u \r\n",data.UUID);
#endif

                break;

            case UTCS:
                val_ptr = command_ptr + 4;
                data.UTCS = atol(val_ptr);
#ifdef DEBUG
                usb.printf("UTCS: %u \r\n",data.UTCS);
#endif
                break;
                
#ifdef BaseStation
            case STMP:
                val_ptr = command_ptr + 4;
                data.STMP = atof(val_ptr);
#ifdef DEBUG
                usb.printf("SOILTEMP: %4.2f \r\n",data.STMP);
#endif
                break;

            case ATMP:
                val_ptr = command_ptr + 4;
                data.ATMP = atof(val_ptr);
#ifdef DEBUG
                usb.printf("AIRTEMP: %4.2f \r\n",data.ATMP);
#endif
                break;

            case MOIS:
                val_ptr = command_ptr + 4;
                data.MOIS = atof(val_ptr);
#ifdef DEBUG
                usb.printf("SOILMOIS: %4.2f \r\n",data.MOIS);
#endif
                break;

            case LIGT:
                val_ptr = command_ptr + 4;
                data.LIGT = atof(val_ptr);
#ifdef DEBUG
                usb.printf("LIGHT: %4.2f \r\n",data.LIGT);
#endif
                break;

            case COND:
                val_ptr = command_ptr + 4;
                data.COND = atof(val_ptr);
#ifdef DEBUG
                usb.printf("CONDUCTIVITY: %4.2f \r\n",data.COND);
#endif
                break;

            case HUMD:
                val_ptr = command_ptr + 4;
                data.HUMD = atof(val_ptr);
#ifdef DEBUG
                usb.printf("AIR HUMIDITY: %4.2f \r\n",data.HUMD);
#endif
                break;

            case PHLV:
                val_ptr = command_ptr + 4;
                data.PHLV = atof(val_ptr);
#ifdef DEBUG
                usb.printf("PH LEVEL: %4.2f \r\n",data.PHLV);
#endif
                break;

            case LBAT:
                val_ptr = command_ptr + 4;
                data.LBAT = atoi(val_ptr);
#ifdef DEBUG
                usb.printf("LOWBAT: %i \r\n",data.LBAT);
#endif
                break;

#endif
#ifdef SensorNode
            case PDAT:
                val_ptr = command_ptr + 4;
                data.PDAT = atoi(val_ptr);
                break;

            case PING:
                val_ptr = command_ptr + 4;
                data.PING = atoi(val_ptr);
                break;

            case WAKE:
                val_ptr = command_ptr + 4;
                data.WAKE = atoi(val_ptr);
                break;

            case PALL:
                val_ptr = command_ptr + 4;
                data.PALL = atoi(val_ptr);
                break;

            case BTRS:
                val_ptr = command_ptr + 4;
                data.BTRS = atoi(val_ptr);
                break;

            case LPMD:
                val_ptr = command_ptr + 4;
                data.LPMD = atoi(val_ptr);
                break;

            case BRST:
                val_ptr = command_ptr + 4;
                data.BRST = atoi(val_ptr);
                break;

#endif

            default: //ignore unknown data
                memset(command, 0, 5);
                val_ptr = NULL;
                break;
        }

#ifdef Display
        if (i >= 32) {
            i = 0;
            j +=20;
        }
        lcd.locate(j,i);
        lcd.printf("%s:  %s", command, val_ptr);
        i+=8;
#endif

#ifdef DEBUG
        usb.printf("Parsing Pass %i\r\n", pcount);
#endif
//Reset pointers and command bytes
        memset(command, 0, 5);
        val_ptr = NULL;
        command_ptr = strtok(NULL, " ");

    }


#ifdef DEBUG
    usb.printf("Success\r\n");
#endif


}

void receive(void) //recived data from UART
{
    static char input [max_msg]; //Set maximum message size
    static unsigned int input_pos = 0; //reset input positon coutner
#ifdef DEBUG
    char c = usb.getc();
#else
    char c = xbee.getc();
#endif
    switch(c) {
        case '\r':   // end of text
#ifdef DEBUG
            usb.printf("\r\n Recv: %s\r\n", input);
#endif
            parse(translate(input)); //Move to translate for API mode
            memset(input, 0, max_msg);
            input_pos = 0;

            break;

        case '\n':   // discard carriage return
            break;

        default:
            if (input_pos < (max_msg)) {
                input [input_pos++] = c;
#ifdef DEBUG
                usb.putc(c);
#endif
                break;
            }
    }

}

