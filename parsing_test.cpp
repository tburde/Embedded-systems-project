#include "mbed.h"
#include <string.h>
#include <ctype.h>
#include <string>

using namespace std;

#define DEBUG

#define BaseStation
//#define SensorNode

#ifdef SensorNode

    #define TEMPSENS
    #define MOISSENS
    
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
    //To be added
    
    
};

//Variables

static char *command_ptr,               
            *val_ptr;  
                          
static agriProtocol data;

const unsigned int max_msg = 128;

agriData resolveString(char *p)
{
#ifdef BaseStation
       if(strcmp(p,"UUID") == 0){} return UUID;
       if(strcmp(p,"UTCS") == 0){} return UTCS;
       if(strcmp(p,"PWRS") == 0){} return PWRS;
       if(strcmp(p,"LBAT") == 0){} return LBAT;
       if(strcmp(p,"STMP") == 0){} return STMP;
       if(strcmp(p,"ATMP") == 0){} return ATMP;
       if(strcmp(p,"MOIS") == 0){} return MOIS;
#endif
#ifdef SensorNode       
       if(strcmp(p,"PDAT") == 0){} return PDAT;
#endif
       //to be added
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
    
    for(int i=0;i<strlen(p);i++){
        p[i] = toupper(p[i]);
    }
    
//First extraction
    
    command_ptr = strtok(p, " ");
    
    while(command_ptr != NULL)
    {
        
        strncpy(command, command_ptr, 4);
        command[4] = '\0';
        switch (resolveString(command)){
#ifdef BaseStation
        case UUID:
            val_ptr = command_ptr + 4;
            data.UUID =  stoi(val_ptr)   
        break;
        
        case UTCS:
            val_ptr = command_ptr + 4;
            data.UTCS = stoi(val_ptr)
        break;
        
        case TEMP:
            val_ptr = command_ptr + 4;
            data.TEMP = stof(val_ptr)
        break;   
#endif                  
#ifdef SensorNode
        case PDAT:
            val_ptr = command_ptr + 4;
            data.PDAT = stoi(val_ptr)
        break;
#endif 

    
        default:
            
        break;
        }
        
    #ifdef DEBUG
                usb.printf("COMMAND: %s \r\n",command);
                usb.printf("Processed: %s \r\n", val_ptr);
    #endif
    
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


