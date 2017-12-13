#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_BUILTIN 1  //or D2 but display an error
// WiFi and UDP constants and variables
WiFiUDP Udp;
const char *APssid = "guderesearch";
const char *APpassword = "12345678";
const unsigned int udpPort = 3333;
char incomingPacket[255];
int packetSize;
boolean sendit=false;
char packetBuffer[255];          // buffer to hold incoming packet

void initiateUDPConnection(void) {
    WiFi.mode(WIFI_AP);    // initialize ESP module in AP mode

    // start access point
    WiFi.softAP(APssid,APpassword);
    Serial.println("Access point started");
    IPAddress ip = WiFi.localIP();
    Serial.print(F("IP Address: "));
    Serial.println(ip);

    // if you get a connection, report back via serial:
    Udp.begin(udpPort);
    Serial.print("Listening on port ");
    Serial.println(udpPort);
}
void listenUdpCommand(void){
    packetSize = Udp.parsePacket();
    if (packetSize) {
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
        packetBuffer[0] = '!';
    }
  }

void setup()
{
 // initialize LED digital pin as an output.
 Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);
 initiateUDPConnection();
}

void loop()
{
  // listen for command to start sending data
  listenUdpCommand();
 // turn the LED on (HIGH is the voltage level)
 digitalWrite(LED_BUILTIN, HIGH);
 // wait for a second
 delay(1000);
 // turn the LED off by making the voltage LOW
 digitalWrite(LED_BUILTIN, LOW);
 Serial.println("Blink");
  // wait for a second
 delay(1000);
}
