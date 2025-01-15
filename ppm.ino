
#define RXD1 16  // Change to your specific RX pin [Based on Adafruit Feather ESP32]
#define TXD1 17  // Change to your specific TX pin [Based on Adafruit Feather ESP32]
// Define constants for scaling
const float maxRawValue = 255.0;  // Maximum raw value (8-bit, 0xFF)
const float maxPPM = 1000.0;      // Maximum PPM range of the sensor

// Accuracy formula constants
const float baseError = 20.0;     // Base error (20 ppm)
const float percentageError = 0.03; // 3% error

void setup() {
  Serial.begin(9600);                           // For debugging
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);  // Initialize Serial1 with 9600 baud rate
  Serial.println("N2O Sensor Data Processing");

}

void loop() {
  // Example data with two columns: {Identifier, Hex Value}
  String data[][2] = {
    {"377", "7F"},
    {"378", "7E"},
    {"380", "85"}
  };
  int numRows = sizeof(data) / sizeof(data[0]);

  for (int i = 0; i < numRows; i++) {
    // Extract the identifier and hex value
    String identifier = data[i][0];
    String hexValue = data[i][1];

    // Convert hexadecimal string to integer
    int rawValue = strtol(hexValue.c_str(), NULL, 16);

    // Calculate PPM value
    float ppm = (rawValue * maxPPM) / maxRawValue;

    // Calculate the accuracy (error range)
    float error = baseError + (ppm * percentageError);
    float lowerBound = ppm - error; // Lower bound of the true value
    float upperBound = ppm + error; // Upper bound of the true value

    // Print results to the serial monitor
    Serial.print("Identifier: ");
    Serial.print(identifier);
    Serial.print(" -> Hex: ");
    Serial.print(hexValue);
    Serial.print(" -> Decimal: ");
    Serial.print(rawValue);
    Serial.print(" -> PPM: ");
    Serial.print(ppm, 2);  // Display with 2 decimal places
    Serial.print(" -> Accuracy Range: ");
    Serial.print(lowerBound, 2);
    Serial.print(" to ");
    Serial.println(upperBound, 2);  // Accuracy range with 2 decimal places
  }

  delay(2000);  // Wait for 2 seconds before repeating
}
