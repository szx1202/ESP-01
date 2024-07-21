/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP8266 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

//******* CODE FOR ARDUINO ESP-01 ***************

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL4S2Rs-8ZG"
#define BLYNK_TEMPLATE_NAME "IOT Relay"
#define BLYNK_AUTH_TOKEN "Hd3GCLpfjRw_dbA369_wGDEz01mKYLmK"         

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LZ_24G";
char pass[] = "*andromedA01.";

const int rel_RailWay_Pin = 0; // on the shield with mounted relay pin 0 is default
int prevState = -1;
int currState = -1;
long lastChangeTime = 0;

void checkPin()
{
  // Invert state, since button is "Active LOW"
  int message = !digitalRead(rel_RailWay_Pin);
  
  if (message == 1) {
    digitalWrite(rel_RailWay_Pin, LOW);
    Serial.println("==1");
  } 
  else if (message == 0) 
  {
    digitalWrite(rel_RailWay_Pin, HIGH);
    Serial.println("==0");
  } 
  else {
    Serial.println("Invalid message");
  }

}

void setup()
{
  // Debug console
  Serial.begin(115200);

  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  checkPin();
  
}
