#include <SoftwareSerial.h>
#include <Wire.h>
#include "Protocentral_MAX30205.h"

SoftwareSerial mySerial(9, 10); // use uart2// Demo program for Arduino MEGA2560 board and sensor with serial output
SoftwareSerial ble(7, 8); 
unsigned char read1, read2, read3;
char sbuffer[30], ch;
String transmitdata;
unsigned char pos;
int temperature;
float temp;
MAX30205 tempSensor;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  tempSensor.begin();
  ble.begin(9600);
}

char mygetchar(void)
{mySerial.listen();
  while (!mySerial.available())
  {
    temp = tempSensor.getTemperature(); // read temperature for every 100ms
    //Serial.println(temp);
    temperature = (1.8 * temp) + 32;
    Serial.println("temperature in fahrenheit");
    Serial.println(temperature);
    delay(1000);

       transmitdata="@";
    transmitdata+=read1;
   transmitdata+=","; 
    transmitdata+=read2;
   transmitdata+=","; 
    transmitdata+=read3;
   transmitdata+=","; 
    transmitdata+=temperature;
   transmitdata+="$"; 
 Serial.println("transmitted data is");
   Serial.println(transmitdata);
  // ble.listen();
   ble.println(transmitdata);
  }
  return mySerial.read();
}


void loop() {
  ch = mygetchar(); //loop till character received
  if (ch == 0x0A) // if received character is <LF>, 0x0A, 10 then process buffer
  {
    pos = 0; // buffer position reset for next reading

    // extract data from serial buffer to 8 bit integer value
    // convert data from ASCII to decimal
    read1 = ((sbuffer[1] - '0') * 100) + ((sbuffer[2] - '0') * 10) + (sbuffer[3] - '0');
    read2 = ((sbuffer[6] - '0') * 100) + ((sbuffer[7] - '0') * 10) + (sbuffer[8] - '0');
    read3 = ((sbuffer[11] - '0') * 100) + ((sbuffer[12] - '0') * 10) + (sbuffer[13] - '0');

    // Value of variables will be between 0-255
    Serial.println();
    Serial.print("systolic:");
    Serial.print(read1);
    Serial.print('\t');
    Serial.print("diastolic:");
    Serial.print(read2);
    Serial.print('\t');
    Serial.print("pulse:");
    Serial.print(read3);
    Serial.print('\t');
    Serial.println();
    transmitdata="@";
    transmitdata+=read1;
   transmitdata+=","; 
    transmitdata+=read2;
   transmitdata+=","; 
    transmitdata+=read3;
   transmitdata+=","; 
    transmitdata+=temperature;
   transmitdata+="$"; 
   Serial.println("transmitted data is");
   Serial.println(transmitdata);
   ble.listen();
   ble.println(transmitdata);
  }
  else
  {
    //store serial data to buffer
    sbuffer[pos] = ch;
    pos++;
  }

}
