#include "mbed.h"
#include <string.h>
#include <ctype.h>
//#include <string>

//using namespace std;

#define DEBUG

#define BaseStation
//#define SensorNode

#ifdef SensorNode

//#define TEMPSENS
//#define MOISSENS

#endif

Serial usb(USBTX, USBRX);

enum agriCmd {
    NONE,
    WAKE,
    SLEEP,
    RETRIEVE_ALL,
    RETRIEVE_VITAL,
    BATTERY_RESET,
};


enum agriData {
    ERRR,
    UUID,
    UTCS,
    PWRS,
    LBAT,
    STMP,
    ATMP,
    MOIS,
    LIGT,
    PHLV,
    COND,
    HUMD,
    GPSC,
};

struct agriProtocol {
    uint16_t UUID;
    uint32_t UTCS;
    uint8_t LBAT;
    uint8_t PWRS;
    uint8_t PDAT;
    double STMP;
    //To be added


};

//Variables

static char *command_ptr,
       *val_ptr;

static agriProtocol data;

const unsigned int max_msg = 128;

agriData resolveString(char *s)
{
#ifdef BaseStation
    if(strcmp(s,"UUID") == 0) {
        return UUID;
    }
    if(strcmp(s,"UTCS") == 0) {
        return UTCS;
    }
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
#endif
#ifdef SensorNode
    if(strcmp(s,"PDAT") == 0) {
        return PDAT;
    }
#endif
    //to be added
    
    return ERRR;
}

void reset(void)
{
    command_ptr = NULL;
    val_ptr = NULL;
    //command_type = NONE ;
}

void parse(char *p ) //Parse data strcture
{
    char command[5];


#ifdef DEBUG
    usb.printf("Parsing\r\n");
#endif

    reset();

    for(int i=0; i<strlen(p); i++) {
        p[i] = toupper(p[i]);
    }

//First extraction

    command_ptr = strtok(p, " ");

    while(command_ptr != NULL) {

        strncpy(command, command_ptr, 4);
        command[4] = '\0';
        switch (resolveString(command)) {
#ifdef BaseStation
            case UUID:
                val_ptr = command_ptr + 4;
                data.UUID =  atoi(val_ptr);
#ifdef DEBUG
                usb.printf("UUID: %d \r\n",data.UUID*3);
#endif
                break;

            case UTCS:
                val_ptr = command_ptr + 4;
                data.UTCS = atoi(val_ptr);
#ifdef DEBUG
                usb.printf("UTCS: %d \r\n",data.UTCS*2);
#endif
                break;

            case STMP:
                val_ptr = command_ptr + 4;
                data.STMP = atof(val_ptr);
#ifdef DEBUG
                usb.printf("SOILTEMP: %4.2f \r\n",data.STMP*2);
#endif
                break;
#endif
#ifdef SensorNode
            case PDAT:
                val_ptr = command_ptr + 4;
                data.PDAT = atoi(val_ptr);
                break;
#endif


            default:
                memset(command, 0, 5);
                val_ptr = NULL;
                break;
        }

#ifdef DEBUG
        // usb.printf("COMMAND: %s \r\n",command);
        // usb.printf("Processed: %s \r\n", val_ptr);
#endif

        memset(command, 0, 5);
        val_ptr = NULL;
        command_ptr = strtok(NULL, " ");

    }


#ifdef DEBUG

    usb.printf("Success\r\n");
#endif


}

void receive(void) //recived data from UART for testing
{
    static char input [max_msg];
    static unsigned int input_pos = 0;

    char c = usb.getc();

    switch(c) {
        case '\r':   // end of text
            //input [input_pos] = ;
#ifdef DEBUG
            usb.printf("\r\n Recv: %s", input);
            usb.printf("\r\n");
#endif
            parse(input);
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





int main()
{
    usb.baud(9600);
    usb.attach(&receive, Serial::RxIrq);
    //reset();

    while(1) {
        //usb.printf("Waiting\r\n");
        //wait(2);
        //parse(&command_type);
    }
}


