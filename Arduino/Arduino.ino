#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h> // Use WiFi.h library for ESP32 or ESP8266
#include <ArduinoJson.h>

const char* ssid = "NATTAVEE_2.4G"; // Change to your WiFi SSID
const char* password = "PBY02556"; // Change to your WiFi password
const char* host = "192.168.1.144"; // Change to your FastAPI host address

#define SS_PIN 5
#define RST_PIN 0
#define BUZZER_PIN 15


bool isStudent(String role) {
    return role == "Student";
}
bool isItems(String role) {
    return role == "Items";
}

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
    String result = sendRFIDData(rfidData);
    int commaIndex = result.indexOf(',');
    int spaceIndex = result.indexOf(' ');
    String name = result.substring(0, spaceIndex);
    String role = result.substring(commaIndex + 1);
    if (isStudent(role)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(name);
        lcd.setCursor(0, 1);
        lcd.print("Scan Items");
        String StudentRFID = rfidData;
        // Clear previous scan data
        memset(nuidPICC, 0, sizeof(nuidPICC));
        while (true) {
            // Wait for RFID card to be scanned and read its serial
            while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
                delay(100); // Add a small delay to prevent busy-waiting
            }
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
            // Send RFID data to the server and get the role
            String rfidData = prepareRFIDData();
            String result = sendRFIDData(rfidData);
            String ItemsRFID = rfidData;
            int commaIndex = result.indexOf(',');
            String name = result.substring(0, commaIndex);
            String role = result.substring(commaIndex + 1);
            Serial.println(role);
            // Check if the role is "Items"
            if (role == "Items") {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(name);
                lcd.setCursor(0, 1);
                lcd.print("SetDaytoBorrow");
                delay(5000);
                String ResultDataSent = sendBorrowToServer(StudentRFID, ItemsRFID,"5");
                Serial.print(ResultDataSent);
                lcd.clear();
                break;
            }
            else{
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Invalid Card");
              lcd.setCursor(0, 1);
              lcd.print("Scan Again");
              memset(nuidPICC, 0, sizeof(nuidPICC));
            }
        }  
    } 
    if (isItems(role)) {
        int commaIndex = result.indexOf(',');
        String name = result.substring(0, commaIndex);
        lcd.print(name);
        String rfidData = prepareRFIDData();
        String result_ItemsStatus = sendItemsCheck(rfidData);
        Serial.print(result_ItemsStatus);
        // Clear previous scan data
        //memset(nuidPICC, 0, sizeof(nuidPICC));
        if (result_ItemsStatus == "Available"){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(name);
          lcd.setCursor(0,1);
          lcd.print("Available");
          delay(5000);
          Serial.println("Please Scan Student Card");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Please Scan ");
          lcd.setCursor(0, 1);
          lcd.print("Student Card");
          delay(3000);
          lcd.clear();
        }
        if(result_ItemsStatus =="unavailable"){
          while (true) {
            // Wait for RFID card to be scanned and read its serial
            while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
                delay(100); // Add a small delay to prevent busy-waiting
            }
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
            // Send RFID data to the server and get the role
            //String rfidData = prepareRFIDData();
            //String result_ItemsStatus = sendItemsCheck(rfidData);
            /*
            if (role == "Student") {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(name);
                lcd.setCursor(0, 1);
                lcd.print("Comfirm?");
                delay(5000);
                lcd.clear();
                break;
            }
            else{
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Invalid Card");
              lcd.setCursor(0, 1);
              lcd.print("Scan Again");
              memset(nuidPICC, 0, sizeof(nuidPICC));
            }
            */
          }
        }  
    } 
    else{
      return;
    }
    lcd.clear();
  }

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
void printUID() {
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

String sendRFIDData(String rfidData) {
    String result = ""; // Initialize result variable

    // Send the RFID data to the FastAPI endpoint
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        if (client.connect(host, 8000)) {
            Serial.println("Connected to server");

            // Construct the JSON payload
            String payload = "{\"rfid_id\":\"" + rfidData + "\"}";

            // Print the data before sending
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
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Waiting Data");
            lcd.setCursor(0,1);
            lcd.print("From Server");

            bool bodyStarted = false;
            String responseBody = "";
            while (client.connected()) {
                if (client.available()) {
                    char c = client.read();
                    if (bodyStarted) {
                        responseBody += c;
                    } else if (c == '"') {
                        bodyStarted = true;
                    }
                }
            }

            // Extract the fields from the response
            int commaIndex = responseBody.indexOf(',');
            String name = responseBody.substring(0, commaIndex-1); // Extract the first value between quotes
            String role = responseBody.substring(commaIndex + 2, responseBody.length() - 2); // Extract the second value between quotes

            lcd.clear();
            Serial.println("Response received");

            // Combine name and role into a single string, separated by a comma
            result = name + "," + role;

            client.stop(); // Close the connection
        } else {
            Serial.println("Connection to server failed");
        }
    } else {
        Serial.println("WiFi not connected");
    }

    // Return the combined result
    return result;
}
String sendItemsCheck(String rfidData) {
    String result_ItemsStatus = ""; // Initialize result variable

    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        if (client.connect(host, 8000)) {
            Serial.println("Connected to server");

            // Construct the JSON payload
            String payload = "{\"rfid_id\":\"" + rfidData + "\"}";

            // Print the data before sending
            Serial.println(payload);

            // Construct the HTTP request
            String httpRequest = "POST /api/CheckItems HTTP/1.1\r\n";
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

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Waiting Data");
            lcd.setCursor(0, 1);
            lcd.print("From Server");

            // Wait for response from the server
            delay(3000); // Wait for 5 seconds

            // Check if response is available
            if (client.available()) {
                // Read the response body
                result_ItemsStatus = client.readString();
                // Parse JSON response
                if (result_ItemsStatus.indexOf("Available") != -1) {
                    return "Available";
                } else if (result_ItemsStatus.indexOf("unavailable") != -1) {
                    return "unavailable";
                } else {
                    return "Unknown";
                }
            } else {
                Serial.println("No response from server");
                return "Error";
            }

            // Close the connection
            client.stop();
        } else {
            Serial.println("Connection to server failed");
            return "Error";
        }
    } else {
        Serial.println("WiFi not connected");
        return "Error";
    }
}
String sendBorrowToServer(String student_rfid, String item_rfid, String borrow_date) {
    String result_Status = ""; // Initialize result variable

    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        if (client.connect(host, 8000)) {
            Serial.println("Connected to server");

            // Construct the JSON payload
            String payload = "{\"Student_Rfid_tag\": \"" + student_rfid + "\", \"Item_Rfid_tag\": \"" + item_rfid + "\", \"borrow_Date\": \"" + borrow_date + "\"}";
            // Print the data before sending
            Serial.println(payload);

            // Construct the HTTP request
            String httpRequest = "POST /api/SEND_Borrow HTTP/1.1\r\n";
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

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Waiting Data");
            lcd.setCursor(0, 1);
            lcd.print("From Server");

            // Wait for response from the server
            delay(500); // Wait for 5 seconds

            // Check if response is available
            if (client.available()) {
                // Read the response body
                result_Status = client.readString();
                // Parse JSON response
                if (result_Status.indexOf("Available") != -1) {
                    return "Available";
                } else if (result_Status.indexOf("unavailable") != -1) {
                    return "unavailable";
                } else {
                    return "Unknown";
                }
            } else {
                Serial.println("No response from server");
                return "Error";
            }

            // Close the connection
            client.stop();
        } else {
            Serial.println("Connection to server failed");
            return "Error";
        }
    } else {
        Serial.println("WiFi not connected");
        return "Error";
    }
}







