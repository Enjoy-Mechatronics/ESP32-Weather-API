#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

//LCD Object (0x3F or 0x27)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//Put your WiFi Credentials here
const char* ssid = "yourssid";
const char* password = "yourpassword";

//URL Endpoint for the API
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "PUT_YOUR_APIKey_Here";

// Replace with your location Credentials
String lat = "36.736813369199396";
String lon = "3.087043199273793";

void setup() {
  Serial.begin(115200);

  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL with Location and API key information
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve some information about the weather from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Display the Current Weather Info
      const char* description = obj["weather"][0]["description"].as<const char*>();
      const float temp = obj["main"]["temp"].as<float>();
      const float humidity = obj["main"]["humidity"].as<float>();

      lcd.clear();
      lcd.print(description);
      lcd.setCursor(0, 1);
      lcd.print(temp);
      lcd.print(" C, ");
      lcd.print(humidity);
      lcd.print(" %");

    } else {
      Serial.println("Error!");
      lcd.clear();
      lcd.print("Can't Get DATA!");
    }

    http.end();

  }
  
  //Wait for 30 seconds
  delay(30000);
}
