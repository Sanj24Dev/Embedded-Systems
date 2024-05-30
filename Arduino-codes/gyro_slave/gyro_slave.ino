#include <Wire.h>

int x = 13067;
int y = -15564;
int z = 0;
int xl;
int xh;
int yl, yh;
int zl, zh;
uint8_t reg=0;

int toTwosComplement(int value) {
  if (value >= 0) {
    return value; 
  } else {
    return ~(abs(value)) + 1;
  }
}

void setup() {
  Serial.begin(9600);

  xh = (toTwosComplement(x))>>8;
  xl = (toTwosComplement(x)&0xFF);
  yh = (toTwosComplement(y))>>8;
  yl = (toTwosComplement(y)&0xFF);
  zh = (toTwosComplement(z))>>8;
  zl = (toTwosComplement(z)&0xFF);
  
  Wire.begin(104);                // join i2c bus with address 0x68
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void requestEvent() {
  if(reg == 0x43)
    Wire.write(xh);
  else if(reg == 0x44)
    Wire.write(xl);
  else if(reg == 0x45)
    Wire.write(yh);
  else if(reg == 0x46)
    Wire.write(yl);
  else if(reg == 0x47)
    Wire.write(zh);
  else if(reg == 0x48)
    Wire.write(zl);
  Serial.print("Sent the value for ");
  Serial.println(reg, HEX);
}

void receiveEvent(int numBytes){
  while (Wire.available()) {
    reg = Wire.read();
    Serial.print("Data requested from ");
    Serial.println(reg, HEX);
  }
  
}
