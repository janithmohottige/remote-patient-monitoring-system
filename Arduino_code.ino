#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                      
#define FIREBASE_HOST "Firebase Host"
#define FIREBASE_AUTH "Firebase Auth Key"
#define WIFI_SSID "WiFI SSID"
#define WIFI_PASSWORD "WiFi Password"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "Wire.h"
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
float BPM, SpO2 ,avgBPM,avgSpO2;
float bpm=0;
float spo2=0;
int i=0;
uint32_t tsLastReport = 0;
const unsigned char bitmap [] PROGMEM=
{
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x0f, 0xe0, 0x7f, 0x00, 0x3f, 0xf9, 0xff, 0xc0,
0x7f, 0xf9, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0,
0xff, 0xf7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0x7f, 0xdb, 0xff, 0xe0,
0x7f, 0x9b, 0xff, 0xe0, 0x00, 0x3b, 0xc0, 0x00, 0x3f, 0xf9, 0x9f, 0xc0, 0x3f, 0xfd, 0xbf, 0xc0,
0x1f, 0xfd, 0xbf, 0x80, 0x0f, 0xfd, 0x7f, 0x00, 0x07, 0xfe, 0x7e, 0x00, 0x03, 0xfe, 0xfc, 0x00,
0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x3f, 0xc0, 0x00,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int val=0;



void setup() {

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor
  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  if (!pox.begin())
  {
       Serial.println("FAILED");
       for(;;);
  }
  else
  {
       Serial.println("SUCCESS");
       pox.setOnBeatDetectedCallback(onBeatDetected);
  }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
  pinMode(16, OUTPUT);
  Serial.print("Initializing Pulse Oximeter..");
  
}

void loop() { 

  pox.update();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {   Firebase.setFloat(firebaseData, "/BPM", BPM);
      Firebase.setFloat(firebaseData, "/SpO2", SpO2);
//      Serial.print("Heart rate:");
//      Serial.print(BPM);
//      Serial.print(" SpO2:");
//      Serial.print(SpO2);
//      Serial.println(" %");
      tsLastReport = millis();
      

   }

}
//  Firebase.setFloat(firebaseData, "/BPM", BPM);
//  Firebase.setFloat(firebaseData, "/SpO2", SpO2);
//if (Firebase.setInt(firebaseData, "/data", BPM)) {    // On successful Write operation, function returns 1  
//               Serial.println("Value Uploaded Successfully");
//               Serial.print("Val = ");
//               Serial.println(BPM);
//               Serial.println("\n");
//               
////               val++;
//               delay(1000);}
//
//else {        
//    Serial.println(firebaseData.errorReason());}




/* NOTE:
 *  To upload value, command is ===> Firebase.setInt(firebaseData, "path", variable);
 *  Example                     ===>  Firebase.setInt(firebaseData, "/data", val);
 */
    
