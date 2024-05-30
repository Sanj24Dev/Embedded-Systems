// use serial bluetooth terminal to send messages
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define LED_PIN 33

BluetoothSerial SerialBT;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    // reads 3 bytes of data
    // first byte is 49 for ON and 48 for OFF
    // next 2 bytes are 13 and 10
    int n = SerialBT.read();
    int b = SerialBT.read();
    int c = SerialBT.read();
    if(n%2==0){
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED OFF");
    }
    else{
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED ONx");
    }
    
  }
  delay(20);
}
