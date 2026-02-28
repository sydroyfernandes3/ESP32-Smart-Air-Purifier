#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include "ThingSpeak.h" // Added ThingSpeak Library

// --- SENSOR & HARDWARE PINS ---
#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ135_PIN 34

#define GREEN_LED 14
#define YELLOW_LED 27
#define RED_LED 26
#define FAN_RELAY 25
#define BUZZER 33   // active buzzer pin

// --- WIFI CREDENTIALS ---
#define WIFI_SSID "Redmi 14C"
#define WIFI_PASSWORD "sydroy07"

// --- THINGSPEAK CREDENTIALS ---
unsigned long myChannelNumber = 3281288; 
const char * myWriteAPIKey = "SC7Y4Q4Q79DGAMRJ";

// --- OBJECT INITIALIZATIONS ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client; 

// Timer variables for ThingSpeak (updates every 20 seconds)
unsigned long lastTime = 0;
unsigned long timerDelay = 20000; 

void setup() {
  Serial.begin(115200);

  // Initialize hardware
  Wire.begin(21, 22);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(FAN_RELAY, HIGH); // fan OFF
  digitalWrite(BUZZER, LOW);     // buzzer OFF

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected with IP: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
  
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int air    = analogRead(MQ135_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  String status;

  // Logic control (happens every 2 seconds)
  if (air < 300) {
    status = "Good";
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(FAN_RELAY, HIGH);
    digitalWrite(BUZZER, LOW);   
  }
  else if (air < 600) {
    status = "Moderate";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(FAN_RELAY, HIGH);
    digitalWrite(BUZZER, LOW);   
  }
  else {
    status = "Poor";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(FAN_RELAY, LOW);
    digitalWrite(BUZZER, HIGH);  
  }

  // Serial Output
  Serial.print("Temp:"); Serial.print(temp);
  Serial.print("C  Hum:"); Serial.print(hum);
  Serial.print("%  Air:"); Serial.print(air);
  Serial.print("  "); Serial.println(status);

  // LCD Output
  lcd.setCursor(0,0);
  lcd.print("T:"); lcd.print(temp,1);
  lcd.print("C H:"); lcd.print(hum,0); lcd.print("%   ");

  lcd.setCursor(0,1);
  lcd.print("Air:"); lcd.print(status);
  lcd.print("      ");

  // Push data to ThingSpeak every 20 seconds
  if ((millis() - lastTime) > timerDelay) {
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);
    ThingSpeak.setField(3, air);
    ThingSpeak.setStatus(status);
    
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if (httpCode == 200) {
      Serial.println("ThingSpeak Update Successful!");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(httpCode));
    }
    lastTime = millis();
  }

  delay(2000); 
}
