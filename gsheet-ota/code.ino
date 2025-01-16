/* 
  Author: Sayed Tanimun Hasan
  Project: N2O Sensor Data Read and Send to GSheet Real Time
  Company: B4T
  Date: 04/12/2024
  Github: https://github.com/tanimunhasan

*/

#define MDASH_APP_NAME "MinimalApp"
#include <mDash.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define RXD1 16  // Change to your specific RX pin [Based on Adafruit Feather ESP32]
#define TXD1 17 // Change to your specific TX pin [Based on Adafruit Feather ESP32]
#define DEVICE_PASSWORD "Mf2ucqEyzkRxhe4IYxK12w"
const char* ssid = "Tanimun";         // Replace with your WiFi SSID
const char* password = "Sayed@15999";  // Replace with your WiFi Password

String scriptURL = "https://script.google.com/macros/s/AKfycbxEY8-Uc6Yj2VA03bW9QT40WWGDxkfwrNXs2Seyurmlp7Xav3O_q_aDYqsHcR0HuHo_/exec";  // Replace with your Web App URL


void setup() {
  Serial.begin(9600);  // For debugging
  Serial2.begin(9600);  // Initialize Serial1 with 9600 baud rate
//Serial1.begin(9600, SERIAL_8N1, RXD0, TXD0);  // Initialize Serial1 with 9600 baud rate  Sayed@15999


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
    Serial.println("\nWiFi connected, IP address:");
  Serial.println(WiFi.localIP());
  mDashBegin(DEVICE_PASSWORD);
}

void loop() {
  // Check if data is available on Serial1
  if (Serial2.available()) {
    String sensorData = Serial2.readStringUntil('\n');  // Read sensor data until a newline
    sensorData.trim();                                  // Remove any extra spaces or newline characters
    Serial.println(sensorData);

    // Parse data assuming format: "value1<TAB>value2"
    int separatorIndex = sensorData.indexOf('\t');  // Locate the tab character

    if (separatorIndex != -1) {
      String value1 = sensorData.substring(0, separatorIndex);   // Extract first value
      String value2 = sensorData.substring(separatorIndex + 1);  // Extract second value

      // Send data to Google Sheets
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(scriptURL);
        http.addHeader("Content-Type", "application/json");
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);  // Handle redirects

        // Create JSON payload
        String postData = "{\"value1\":" + value1 + ",\"value2\":" + value2 + "}";

        int httpResponseCode = http.POST(postData);
        String response = http.getString();  // Get response content
        Serial.println("Response Code: " + String(httpResponseCode));
        //Serial.println("Response Body: " + response);
        if (httpResponseCode > 0) {
          Serial.println("POST Sent. Response code: " + String(httpResponseCode));
        } else {
          Serial.println("Error sending POST: " + http.errorToString(httpResponseCode));
        }
        http.end();
      } else {
        Serial.println("WiFi Disconnected");
      }
    }

    else {
      Serial.println("Invalid data format: " + sensorData);
    }
  }

  delay(5000);  // Adjust the delay as needed
}
