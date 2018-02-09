#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi and UDP constants and variables
WiFiUDP Udp;
const char *APssid = "guderesearch";
const char *APpassword = "12345678";
const unsigned int udpPort = 3333;
char incomingPacket[255];
int packetSize;
boolean sendit=false;
char packetBuffer[255];          // buffer to hold incoming packet
unsigned long starttime = 0;
uint8_t ReplyBuffer[50];
int connected = 0;

void initiateUDPConnection(void) {
    WiFi.mode(WIFI_AP);    // initialize ESP module in AP mode

    // start access point
    WiFi.softAP(APssid,APpassword);
    Serial.println("Access point started");
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // if you get a connection, report back via serial:
    connected = Udp.begin(udpPort);
    Serial.print("Listening on port ");
    Serial.println(udpPort);
    Serial.print("Connected = ");
    Serial.println(connected);

}
void listenUdpCommand(void){
    packetSize = Udp.parsePacket();
    if (packetSize != 0) {
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remoteIp = Udp.remoteIP();
        Serial.print(remoteIp);
        Serial.print(", port ");
        Serial.println(Udp.remotePort());

        // read the packet into packetBufffer
        int len = Udp.read(packetBuffer, 255);
        if (len > 0) {
          packetBuffer[len] = 0;
        }
        Serial.println("Contents:");
        Serial.println(packetBuffer);
    } else {
        //Serial.print("no data received");
        packetBuffer[0] = '!';
    }
  }

  void sendUdp(void){
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer, 50);
    Udp.endPacket();
  }

void setup()
{
  Serial.begin(115200);
  initiateUDPConnection();

  for (uint8_t i=0;i<50;i++){
    ReplyBuffer[i]=i;
  }
}

void loop()
{
  // listen for command to start sending data
  listenUdpCommand();
  if (sendit == false){
      if (strcmp(packetBuffer, "start") == 0) {
            sendit = true;
            Serial.println("Logging started");
            starttime=millis();
            sendUdp();
          }
        }
  // if (connected == 1) {
  //
  // }
  delay(1000);
}
