
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  pinMode(33 , OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (SerialBT.available()) {
    // reads 3 bytes of data
    // first byte is 49 for ON and 48 for OFF
    // next 2 bytes are 13 and 10
    int n = SerialBT.read();
    int b = SerialBT.read();
    int c = SerialBT.read();
//    Serial.println(n);
    Serial.write(n);
    SerialBT.write(n);    
  }
  delay(20);
}
