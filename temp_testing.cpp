#include "mbed.h"

I2C i2c(p28, p27);
 
const int addr = 0x44 << 1;
 
int main() {
    char cmd[2];
    char resp[6];

    while (1) {
        
        cmd[0] = 0x2C;
        cmd[1] = 0x06;
        i2c.write(addr, cmd, 2, false);
 
        wait(0.5);
 
        i2c.read(addr, resp, 6, false);
        
        uint16_t temp = ((resp[0]<<8)|resp[1]);
        
        float tmp = ((-45.0+175.0*(temp/(65535.0))));
        printf("Temp = %.2f\r\n", tmp);
        
        wait(2);
    }
}
