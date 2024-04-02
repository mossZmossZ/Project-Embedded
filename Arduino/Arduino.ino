#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h> 
#include <Keypad_I2C.h>
#include <Keypad.h>

const char* ssid = "NattaveeIphone"; // Change to your WiFi SSID
const char* password = "88888888"; // Change to your WiFi password
const char* host = "45.141.27.250"; // Change to your FastAPI host address
const byte ROWS = 4;  
const byte COLS = 4;  
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','-'},
  {'7','8','9','-'},
  {'-','0','-','-'}
};


#define SS_PIN 5
#define RST_PIN 0
#define BUZZER_PIN 15
#define I2CADDRKEYPAD 0x21 

bool isStudent(String role) {
    return role == "Student";
}
bool isItems(String role) {
    return role == "Items";
}
bool isunknown(String role){
    return role =="NotFound";
}
byte nuidPICC[4];
byte rowPins[ROWS] = {0, 1, 2, 3}; 
byte colPins[COLS] = {4, 5, 6, 7}; 

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 rfid(SS_PIN, RST_PIN); 

MFRC522::MIFARE_Key key; 

Keypad_I2C keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDRKEYPAD, PCF8574 );

String inputdate(); // Function initial

char pressedKeys[4]; // Array to hold pressed keys
int keyIndex = 0;    // Index to keep track of the position in the array

void setup() {
    Serial.begin(115200);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 
    lcd.begin();
    keypad.begin(makeKeymap(keys));  
    lcd.backlight();
    lcd.setCursor(0, 0);
    pinMode(BUZZER_PIN, OUTPUT);
    tone(BUZZER_PIN, 500);
    delay(200);
    noTone(BUZZER_PIN);
    lcd.print("Starting");
    Serial.println("Starting");
    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }
    connectWiFi();
}

void loop() {
  lcd.setCursor(0, 0); 
  lcd.print("Ready To Scan");
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;
  for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
  }
    lcd.clear();
    tone(BUZZER_PIN, 523);
    delay(300);
    noTone(BUZZER_PIN);
    Serial.print(F("NUID :"));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    lcd.setCursor(0, 0); 
    lcd.print("NUID: ");
    printUID();
    delay(500);
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
        delay(2000);
        String StudentRFID = rfidData;
        // Clear previous scan data
        memset(nuidPICC, 0, sizeof(nuidPICC));
        unsigned long startTime = millis(); // Record the start time
            while (true) {
              // Check if 20 seconds have passed
              if (millis() - startTime > 60000) {
                  // Timeout reached, exit the loop
                  lcd.clear();
                  lcd.print("Timeout reached");
                  delay(2000);
                  lcd.clear();
                  break;
              }
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
                  lcd.setCursor(0, 0);
                  lcd.print(name);
                  lcd.setCursor(0, 1);
                  lcd.print("SetDaytoBorrow");
                  delay(4000);
                  String ResultDate = inputdate();
                  String ResultDataSent = sendBorrowToServer(StudentRFID, ItemsRFID,ResultDate);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print(ResultDataSent);
                  lcd.setCursor(0, 1);
                  lcd.print("Thank You!");
                  delay(2000);
                  lcd.clear();
                }
                if(result_ItemsStatus =="unavailable"){
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Item Unavailable");
                  lcd.setCursor(0, 1);
                  lcd.print("Scan Again");
                  memset(nuidPICC, 0, sizeof(nuidPICC));
                  unsigned long startTime_ItemsStatus = millis(); // Record the start time
                  while (true) {
                      // Check if 20 seconds have passed
                      if (millis() - startTime_ItemsStatus > 60000) {
                          // Timeout reached, exit the loop
                          lcd.clear();
                          lcd.print("Timeout reached");
                          delay(2000);
                          lcd.clear();
                          break;
                      }
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
                      int commaIndex = result.indexOf(',');
                      String name = result.substring(0, commaIndex);
                      lcd.print(name);
                      String rfidData = prepareRFIDData();
                      String result_ItemsStatus = sendItemsCheck(rfidData);
                      Serial.print(result_ItemsStatus);
                      if (result_ItemsStatus =="Available") {
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print(name);
                          lcd.setCursor(0, 1);
                          lcd.print("SetDaytoBorrow");
                          delay(5000);
                          String ResultDataSent = sendBorrowToServer(StudentRFID, ItemsRFID,"5");
                          lcd.setCursor(0, 0);
                          lcd.print(ResultDataSent);
                          lcd.setCursor(0, 1);
                          lcd.print("Thank You!");
                          delay(2000);
                          lcd.clear();
                          break;
                      }
                      else{
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print("Items Not Available");
                          lcd.setCursor(0, 1);
                          lcd.print("Scan Again");
                          memset(nuidPICC, 0, sizeof(nuidPICC));
                      }
                  }
                }  
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
        String itemsRFID = rfidData;
        String result_ItemsStatus = sendItemsCheck(rfidData);
        Serial.print(result_ItemsStatus);
        // Clear previous scan data
        memset(nuidPICC, 0, sizeof(nuidPICC));
        if (result_ItemsStatus == "Available"){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(name);
          lcd.setCursor(0,1);
          lcd.print("Available");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Please Scan ");
          lcd.setCursor(0, 1);
          lcd.print("Student Card");
          delay(2000);
          lcd.clear();
        }
        if(result_ItemsStatus =="unavailable"){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Please Scan ");
          lcd.setCursor(0, 1);
          lcd.print("Student Card");
          memset(nuidPICC, 0, sizeof(nuidPICC));
          unsigned long startTime = millis(); // Record the start time
            while (true) {
              // Check if 20 seconds have passed
              if (millis() - startTime > 60000) {
                  // Timeout reached, exit the loop
                  lcd.clear();
                  lcd.print("Timeout reached");
                  delay(2000);
                  lcd.clear();
                  break;
              }
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
              int commaIndex = result.indexOf(',');
              String name = result.substring(0, commaIndex);
              lcd.print(name);
              String rfidData = prepareRFIDData();
              String result_ReturnStatus = sendReturnToServer(rfidData, itemsRFID);
              Serial.print(result_ReturnStatus);
              if (result_ReturnStatus =="successfully") {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print(result_ReturnStatus);
                  lcd.setCursor(0, 1);
                  lcd.print("Thank You!");
                  delay(2000);
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
                  break;
              }
          }
        }  
    } 
    if (isunknown(role)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Not Found");
      lcd.setCursor(0,1);
      lcd.print("Register");
      delay(2000);
    }
    else{
      lcd.clear();
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting wifi");
  Serial.print("Attempting to connect to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected");
  Serial.println("Connected to WiFi");
  delay(500);
  lcd.clear();
}
String prepareRFIDData() {
  String rfidData;
  for (int i = 0; i < rfid.uid.size; i++) {
    rfidData += rfid.uid.uidByte[i];
  }
  return rfidData;
}
String sendRFIDData(String rfidData) {
    String result = "";
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        if (client.connect(host, 8000)) {
            Serial.println("Connected to server");
            String payload = "{\"rfid_id\":\"" + rfidData + "\"}";
            Serial.println(payload);
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
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connection to");
            lcd.setCursor(0,1);
            lcd.print("Server failed");
            Serial.println("Connection to server failed");
            delay(1000);
            lcd.clear();
        }
    } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WiFi not");
        lcd.setCursor(0,1);
        lcd.print("Connected");
        Serial.println("WiFi not connected");
        delay(1000);
        lcd.clear();
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
            delay(500); // Wait for 5 seconds
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
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connection to");
            lcd.setCursor(0,1);
            lcd.print("Server failed");
            Serial.println("Connection to server failed");
            delay(1000);
            lcd.clear();
            return "Error";
        }
    } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WiFi not");
        lcd.setCursor(0,1);
        lcd.print("Connected");
        Serial.println("WiFi not connected");
        delay(1000);
        lcd.clear();
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
            delay(3000); // Wait for 5 seconds
            // Check if response is available
            if (client.available()) {
                // Read the response body
                result_Status = client.readString();
                // Parse JSON response
                if (result_Status.indexOf("successfully") != -1) {
                    return "successfully";
                } else {
                    return "No response";
                }
            } else {
                Serial.println("No response from server");
                return "Error";
            }
            // Close the connection
            client.stop();
        } else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connection to");
            lcd.setCursor(0,1);
            lcd.print("Server failed");
            Serial.println("Connection to server failed");
            delay(1000);;
            lcd.clear();
            return "Error";
        }
    } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WiFi not");
        lcd.setCursor(0,1);
        lcd.print("Connected");
        Serial.println("WiFi not connected");
        delay(1000);
        lcd.clear();
        return "Error";
    }
}
String sendReturnToServer(String student_rfid, String item_rfid) {
    String result_Status = ""; // Initialize result variable
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        if (client.connect(host, 8000)) {
            Serial.println("Connected to server");
            // Construct the JSON payload
            String payload = "{\"Student_Rfid_tag\": \"" + student_rfid + "\", \"Item_Rfid_tag\": \"" + item_rfid + "\"}";
            // Print the data before sending
            Serial.println(payload);
            // Construct the HTTP request
            String httpRequest = "POST /api/RETURN_Item HTTP/1.1\r\n";
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
            delay(1000); // Wait for 5 seconds
            // Check if response is available
            if (client.available()) {
                // Read the response body
                result_Status = client.readString();
                // Parse JSON response
                if (result_Status.indexOf("successfully") != -1) {
                    return "successfully";
                } 
                if (result_Status.indexOf("ERROR") != -1) {
                    return "Error";
                }else {
                    return "No response";
                }
            } else {
                Serial.println("No response from server");
                return "Error";
            }
            // Close the connection
            client.stop();
        } else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connection to");
            lcd.setCursor(0,1);
            lcd.print("Server failed");
            Serial.println("Connection to server failed");
            delay(1000);
            lcd.clear();
            return "Error";
        }
    } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WiFi not");
        lcd.setCursor(0,1);
        lcd.print("Connected");
        Serial.println("WiFi not connected");
        delay(1000);
        lcd.clear();
        return "Error";
    }
}
String inputdate() {
  lcd.clear();
  unsigned long startTimeinput = millis();
  String result;
  lcd.setCursor(0, 0);
  lcd.print("Borrow : ");
  lcd.setCursor(6, 1);
  lcd.print("Days");
  while (true) {
    // Check if 20 seconds have passed
    if (millis() - startTimeinput > 60000) {
      // Timeout reached, exit the loop
      lcd.clear();
      lcd.print("Timeout reached");
      delay(2000);
      lcd.clear();
      return ""; // Return an empty string due to timeout
    }
    char key_pressed = keypad.getKey();
    if (key_pressed && key_pressed != 'A' && key_pressed != '-' && keyIndex < 4) {
      Serial.println(key_pressed);
      lcd.setCursor(keyIndex, 1);
      lcd.print(key_pressed);
      pressedKeys[keyIndex] = key_pressed; // Store pressed key in the array
      keyIndex++;
    }
    if (keyIndex > 3 || (key_pressed == 'A' && keyIndex != 0)) {
      lcd.clear();
      delay(100);
      // Print final pressed keys
      Serial.print("Final pressed keys: ");
      for (int i = 0; i < keyIndex; i++) {
        Serial.print(pressedKeys[i]);
        result += pressedKeys[i]; // Append each character to the result string
      }
      Serial.println();
      // Reset variables
      keyIndex = 0;
      // Clear the pressed keys array
      memset(pressedKeys, 0, sizeof(pressedKeys));
      lcd.clear();
      return result; // Return the result string
    }
  }
}