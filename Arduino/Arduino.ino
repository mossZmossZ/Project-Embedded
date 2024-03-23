#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 5
#define RST_PIN 0

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
    lcd.print("Start Scaner");
    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }

    Serial.println("Start Scaner");
}

void loop() {
  lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
    lcd.print("Ready To Scan");
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;


  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    lcd.clear();
    Serial.print(F("NUID :"));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    lcd.setCursor(0, 0); // Set cursor to the beginning of first line
    lcd.print("NUID: ");
    printUID();
  }
  else Serial.println(F("Card read previously."));
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
