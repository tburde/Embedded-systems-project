#include "mbed.h"
#include "parser.h"

char msg[32] = "UUID1001 UTCS198723 ATMP23.4 \r\n";
//char msg2[32] = "UUID1002 UTCS348733 STMP18.3 \r\n";
char msg1[32] = "UUID10234 UTCS34873 PDAT1 \r\n";
char msg2[32] = "UUID45234 UTCS234323 PALL1 \r\n";

AnalogIn pot1(p19);

int main()
{
#ifdef DEBUG
    usb.baud(9600);
    usb.attach(&receive, Serial::RxIrq);
#else
    xbee.baud(9600);
    xbee.attach(&receive, Serial::RxIrq);
#endif
#ifdef Display
    lcd.cls();
    lcd.locate(0,3);
#ifdef BaseStation
    lcd.printf("AgriNet Base Station");
#endif
#ifdef SensorNode
    lcd.printf("AgriNet Sensor Node");
#endif
    wait(2);
    lcd.cls();
#endif

    while(1) {
#ifdef BaseStation
        transmit(msg1, 1);
        transmit(msg2, 2);
#endif
#ifdef SensorNode
        add_msg("UUID", uint16_t(12432), false);
        add_msg("UTCS", uint32_t(567372), false);
        add_msg("ATMP", float(pot1.read()), true);
        transmit(transmit_msg);
        clear_msg();
#endif
        wait(2);

    }
}

