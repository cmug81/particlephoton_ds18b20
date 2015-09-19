// This #include statement was automatically added by the Spark IDE.
#include "OneWire/OneWire.h"

OneWire one = OneWire(D2);
uint8_t rom[8];
uint8_t resp[9];

char szInfo[64];

void setup() {
    Spark.variable("tempr", &szInfo, STRING);
    Serial1.begin(9600);    
}

void loop() {

    // Get the ROM address
    one.reset();
    one.write(0x33);
    one.read_bytes(rom, 8);
    // Get the temp
    one.reset();
    one.write(0x55);
    one.write_bytes(rom,8);
    one.write(0x44);
    delay(10);
    one.reset();
    one.write(0x55);
    one.write_bytes(rom, 8);
    one.write(0xBE);
    one.read_bytes(resp, 9);

    byte MSB = resp[1];
    byte LSB = resp[0];

    int16_t intTemp = ((MSB << 8) | LSB); //using two's compliment 16-bit
    float celsius =   ((double)intTemp)/16.0;
    float fahrenheit = (( celsius*9.0)/5.0+32.0);

    sprintf(szInfo, "T: %2.2f C, %2.2f F", celsius, fahrenheit);


       Spark.publish("tmpinfo", szInfo);

    delay(25000);
}
