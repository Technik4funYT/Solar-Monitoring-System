#define BLYNK_PRINT Serial                                        // einbinden der Libraries
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_ADS1015.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>




char auth[] = "token";                                        // Blynk token
char ssid[] = "Ssid";                                         // Wlan Ssid
char pass[] = "Passwort";                                     // Wlan Passwort 


float Volt1 = 0.0;                                             // die Variabeln speichern
float Volt2 = 0.0;
float AmpereS  = 0.0;
float AmpereL  = 0.0;


SimpleTimer timer;                                                 // timer

Adafruit_ADS1115 ads(0x48);                                        // Adresse für den ADC




void sendUptime()                                                 // Daten an Blynk senden
{
  Blynk.virtualWrite(V0, Volt1);
  Blynk.virtualWrite(V1, Volt2);
  Blynk.virtualWrite(V2, AmpereS);
  Blynk.virtualWrite(V3, AmpereL);
}




void setup(void)
{
  timer.setInterval(1000L, sendUptime);       // Timer
  timer.setInterval(1000L, serialPrinttime);

   
                                                 
                                              // The ADC input range (or gain) can be changed via the following
                                              // functions, but be careful never to exceed VDD +0.3V max, or to
                                              // exceed the upper and lower limits if you adjust the input range!
                                              // Setting these values incorrectly may destroy your ADC!
                                              //                                                                ADS1115
                                              //                                                                -------
                                              // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
                                              // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV
                                              // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
                                              // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.03125mV
                                              // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.015625mV
                                              // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  
  ads.setGain(GAIN_ONE);                      // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();                                // starte den ADC
  
  Serial.begin(9600);                         // beginne Seriellenmonitor BAUD 9600
  
  Blynk.begin(auth, ssid, pass);              // starte Blynk
}




void loop(void)
{
  int16_t adc0;                                     // Berechnung für Volt2
  adc0 = ads.readADC_SingleEnded(0);                // lese den ADC0 aus
  Volt2 = (adc0 * 0.125) / 1000 * 7.67;             // Volt1=(adc0*0.125)/1000 entspricht V am ADC Pin,(*7.67)ist das Spannungsteiler Verhältnis 

  int16_t adc1;
  adc1 = ads.readADC_SingleEnded(1);                // Berechnung für Volt1
  Volt1 = (adc1 * 0.125) / 1000 * 7.67;

 
  int16_t adc2;
  adc2 = ads.readADC_SingleEnded(2);
  AmpereL = (((adc2 * 0.125) * 1.612) / 100) - 24.44;
if (AmpereL < 0.02)
{
  AmpereL = 0.00;
}
else
{
  AmpereL = AmpereL;
}
 


  int16_t adc3;
  adc3 = ads.readADC_SingleEnded(3);
  AmpereS = (((adc3 * 0.125) * 1.612) / 100) - 24.44;
if (AmpereS < 0.02)
{
  AmpereS = 0.00;
}
else
{
  AmpereS = AmpereS;






  
}
  Blynk.run();                                                      // Blynk läuft
  timer.run();                                                      // Timer läuft

  delay(100);                                                       // warte 100ms
}




void serialPrinttime()                                              // Zeit für den Seriellen Monitor, zum Debuggen
{
  Serial.print("Volt1: ");                                         // Sende die fertigen Werte an den Seriellenmonitor
  Serial.println(Volt1);
  Serial.println();

  Serial.print("Volt2: ");
  Serial.println(Volt2);
  Serial.println();

  Serial.print("AmpereS: ");
  Serial.println(AmpereS);
  Serial.println();

  Serial.print("AmpereL: ");
  Serial.println(AmpereL);
  Serial.println();

}
