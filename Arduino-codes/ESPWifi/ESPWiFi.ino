#include <ESP8266WiFi.h>
#include <MPU6050.h>

#ifndef STASSID
#define STASSID "Galaxy M118515"  //name of the network
#define STAPSK "password"   //password of the network
#endif
#define MPU_ADDR 0x68
constexpr int port = 12370;  // should match server port

WiFiClient client;

void setup() {

  Serial.begin(115200);
  Serial.println(ESP.getFullVersion());

  // setup gyro
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  int s = Wire.endTransmission();
  Serial.println(s);

  // WiFi setup
  WiFi.begin(STASSID, STAPSK);
  Serial.print("\nConnecting to ");
  Serial.println(STASSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.println("connected");
  // server IP and port
  if(client.connect("192.168.162.62", port)){
    Serial.println("Connected to server");
  }

}


void loop() {
  unsigned char x;
  int stat;
  
  Wire.beginTransmission(MPU_ADDR);//begin transmission with gyro in Write mode
  Serial.println(MPU_ADDR,HEX);
  
  Serial.println("Sending reg addr");
  Wire.write(0x75);
  Serial.println("Reading");
  Wire.requestFrom(0x68, 1);//request 1 Bytes of information from gyro
  x = Wire.read();//read bytes
  stat = Wire.endTransmission();
  // Serial.print("stat :");
  // Serial.println(stat);
  Serial.print("x : ");
  Serial.println(x,HEX);

  client.println(x,HEX);
  delay(1000);
}
