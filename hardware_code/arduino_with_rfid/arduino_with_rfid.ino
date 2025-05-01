#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 8  // Define the pin the buzzer is connected to

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() {
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();            // Initiate  SPI bus
  mfrc522.PCD_Init();     // Initiate MFRC522
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output
  digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void beepOnce() {
  digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  delay(100); // Buzzer on for 100 milliseconds
  digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
}

void loop() {
  mfrc522.PCD_Reset();  // Reset the RFID reader
  mfrc522.PCD_Init();   // Reinitialize the reader
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    StaticJsonDocument<200> doc;
    String cardId = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardId += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
    }

    cardId.toUpperCase();
    doc["cardID"] = cardId.c_str();
    serializeJson(doc, Serial); // Serialize JSON and send over serial
    // Serial.println(); // Ensure to end with a newline
    
    beepOnce(); // Beep the buzzer once when a card is scanned

    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
  }
  delay(1000);  // Add a delay to prevent continuous reading
}
