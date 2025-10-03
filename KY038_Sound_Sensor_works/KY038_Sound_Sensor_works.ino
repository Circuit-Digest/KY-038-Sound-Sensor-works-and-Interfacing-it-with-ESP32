#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define  SCREEN_WIDTH 128 // OLED display width, in pixels
#define  SCREEN_HEIGHT 64 // OLED display height, in pixels
#define  OLED_RESET   -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int sampleWindow = 50;
unsigned int sample;
#define SENSOR_PIN 35
#define PIN_QUIET 33
#define PIN_MODERATE 25
#define PIN_LOUD 26
void setup(){
  pinMode(SENSOR_PIN, INPUT);
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();
  digitalWrite(PIN_LOUD, LOW);
}
void loop()
{
  unsigned long startMillis = millis();                 
  float peakToPeak = 0;                                 
  unsigned int signalMax = 0;                           
  unsigned int signalMin = 1024;                         
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN);
    if (sample < 1024)                                 
    {
      if (sample > signalMax)
      {
        signalMax = sample;                         
      }
      else if (sample < signalMin)
      {
        signalMin = sample;                          
      }
    }
  }
  peakToPeak = signalMax - signalMin;                   
  int db = map(peakToPeak, 0, 900, 49, 90);        
  Serial.print("\t");
  Serial.println(db);
  display.setCursor(0, 0);
  display.print("Loudness: ");
  display.print(db);
  display.print("dB");
  digitalWrite(PIN_LOUD, LOW);
  if (db <= 55)
  {
    display.clearDisplay();
    display.setCursor(0, 1);
    display.print("Level:Quite");
    display.display();
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
    //  delay(3000);
  }
  else if (db > 60 && db < 85)
  {
    display.clearDisplay();
    display.setCursor(0, 1);
    display.print("Level:Moderate");
    display.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db >= 85 && db <= 90)
  {
    display.clearDisplay();
    display.setCursor(0, 1);
    display.print("Level:High");
    display.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
  }
  else
  {
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
  }
  delay(200);
}
