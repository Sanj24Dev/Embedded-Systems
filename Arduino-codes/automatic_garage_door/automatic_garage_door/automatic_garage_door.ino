/***
 * Automatic garage system
 * There are 2 limit switches - 1 at the door close position, 1 at door open position
 * If the door-close sensor senses the door, then the garage is closed
 * If the door-open sensor does not sense the door, then the garage si open
 */


#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Galaxy M118515"  //name of the network
#define STAPSK "password"   //password of the network
#endif
#define down  2
#define up    3
constexpr int port = 12370;  // should match server port

WiFiClient client;

void setup() {

  Serial.begin(115200);
  Serial.println(ESP.getFullVersion());

  // limit switch setup
  pinMode(down,INPUT);
  pinMode(up,INPUT);
  
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
  if(client.connect("192.168.190.62", port)){
    Serial.println("Connected to server");
  }

}


void loop() {
  unsigned char x;
  int stat;
  if(client.available()){
    char dir = client.read();
    switch(dir)
    {
      case 'o':
        openGarage();
        break;
      case 'c':
        closeGarage();
        break;
    }
  }
  delay(1000);
}

int isUp()
{
  // int flag=digitalRead(up);     // if door not there, then sends 0  => check
  int flag;
  while(Serial.available() == 0);
  flag = Serial.parseInt();
  Serial.print("Door: ");
  Serial.println(flag);
  return flag;  // 1 if up
}

int isDown()
{
  // int flag=digitalRead(down);     // if door there, then sends 1  => check
  int flag = Serial.read();
  Serial.print("Door: ");
  Serial.println(flag);
  return flag;  // 1 if down
}

void openGarage()
{
  Serial.print("enter a num: ");
  while(Serial.available() == 0);
  int r = Serial.parseInt();
  Serial.print("read: ");
  Serial.println(r);
//  if(isDown() || !isUp())
//  {
//    // pull the door up => see its mech
//    client.println("Opening");
//    while(!isUp());
//    client.println("Opened");
//  }
}

void closeGarage()
{
  if(isUp() || !isDown())
  {
    // pull the door up => see its mech
    client.println("Closing");
    while(!isDown());
    client.println("Closed");
  }  
}
