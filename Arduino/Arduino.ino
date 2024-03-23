#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h> // Use WiFi.h library for ESP32 or ESP8266

const char* ssid = "NATTAVEE_2.4G"; // Change to your WiFi SSID
const char* password = "PBY02556"; // Change to your WiFi password
const char* host = "192.168.1.144"; // Change to your FastAPI host address

#define SS_PIN 5
#define RST_PIN 0
#define BUZZER_PIN 15

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

void setup() {
    Serial.begin(115200);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 
    lcd.begin();
    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
    pinMode(BUZZER_PIN, OUTPUT);
    tone(BUZZER_PIN, 500);
    delay(100);
    tone(BUZZER_PIN, 500);
    delay(100);
    noTone(BUZZER_PIN);
    lcd.print("Start Scaner");
    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }

    Serial.println("Start Scaner");

    // Attempt to connect to WiFi
    connectWiFi();
}

void loop() {
  lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
  lcd.print("Ready To Scan");
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
  }
  lcd.clear();
  tone(BUZZER_PIN, 523);
  delay(200);
  noTone(BUZZER_PIN);
  Serial.print(F("NUID :"));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  lcd.setCursor(0, 0); // Set cursor to the beginning of first line
  lcd.print("NUID: ");
  printUID();
  String rfidData = prepareRFIDData();
  sendRFIDData(rfidData);

  delay(2000);
  lcd.clear();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
void printUID() {
  // Assuming rfid is your RFID object
  // Print UID on LCD in decimal format
  for (int i = 0; i < rfid.uid.size; i++) {
    lcd.print(rfid.uid.uidByte[i], DEC);
  }
}
void connectWiFi() {
  // Connect to WiFi network
  Serial.print("Attempting to connect to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

String prepareRFIDData() {
  String rfidData;
  // Assuming rfid is your RFID object
  // Prepare RFID data
  for (int i = 0; i < rfid.uid.size; i++) {
    rfidData += rfid.uid.uidByte[i];
  }
  return rfidData;
}

void sendRFIDData(String rfidData) {
  // Send the RFID data to the FastAPI endpoint
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(host, 8000)) {
      Serial.println("Connected to server");

      // Construct the JSON payload
      String payload = "{\"rfid_id\":\"" + rfidData + "\"}";
      
      // Print the data before sending
      Serial.println("Data to be sent:");
      Serial.println(payload);

      // Construct the HTTP request
      String httpRequest = "POST /api/SentRFID HTTP/1.1\r\n";
      httpRequest += "Host: ";
      httpRequest += host;
      httpRequest += "\r\n";
      httpRequest += "Content-Type: application/json\r\n";
      httpRequest += "Content-Length: ";
      httpRequest += String(payload.length());
      httpRequest += "\r\n\r\n";
      httpRequest += payload;

      // Send the HTTP request
      client.print(httpRequest);

      Serial.println("Data sent to server");

      // Wait for response from the server
      bool bodyStarted = false;
      String responseBody = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          if (bodyStarted) {
            responseBody += c;
          } else if (c == '[') {
            bodyStarted = true;
          }
        }
      }

      client.stop(); // Close the connection

      // Print the extracted text from the response
      int startIndex = responseBody.indexOf('"') + 1;
      int endIndex = responseBody.lastIndexOf('"');
      String extractedText = responseBody.substring(startIndex, endIndex);
      Serial.println(extractedText);
      lcd.clear();
      lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
      lcd.print(extractedText);
      Serial.println("Response received");
    } else {
      Serial.println("Connection to server failed");
    }
  } else {
    Serial.println("WiFi not connected");
  }
}








