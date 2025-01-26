#include <SPI.h>
#include <MFRC522.h>

#define LED_PIN
#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

#define RELAY A0 // Create a relay pin for controlling the door lock

bool locked = true;

// RFID tag UID to be used for unlocking
byte authorizedUID[] = { 0x63, 0xFB, 0x54, 0x31}; // Replace with your RFID tag's UID

void setup() {
  Serial.begin(9600);  // Start serial communication
  SPI.begin();         // Start SPI bus
  mfrc522.PCD_Init();  // Initialize the RFID reader
  pinMode(RELAY, OUTPUT); // Set relay pin as output'
  // pinMode(LED_BUILTIN, OUTPUT);
  
  // Initial lock position (locked)
  Serial.println("Scan your RFID card/tag to unlock...");
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("Card Present");
     
    if(mfrc522.PICC_ReadCardSerial()){
      Serial.print("UID tag: ");
      String readUID = "";
      
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        readUID += String(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      Serial.println(readUID);
    
      Serial.println("Checking auth.... ");
      //if (checkAuthorization(readUID)) {
        Serial.println("Correct RFID");
        unlockDoor();
      //} else {
        //Serial.println("Access denied.");
      //}
      
      delay(1000); 
    }
    mfrc522.PICC_HaltA(); // halt PICC
    mfrc522.PCD_StopCrypto1(); // stop encryption on PCD
  }
  delay(1000);
}

bool checkAuthorization(String scannedUID) {
  String authorizedUIDStr = "";
  for (byte i = 0; i < 4; i++) {
    authorizedUIDStr += String(authorizedUID[i], HEX);
  }
  return scannedUID.equalsIgnoreCase(authorizedUIDStr);
}

void unlockDoor() {
  if (locked == true){
    digitalWrite(RELAY, HIGH);  // Rotate servo to unlock position
    delay(52);
    digitalWrite(RELAY, LOW); 
    Serial.println("Door Unlocked!");
  }
  else{
    digitalWrite(RELAY, HIGH);  // Rotate servo to unlock position
    delay(52);
    digitalWrite(RELAY, LOW); 
    Serial.println("Door Locked!");
  }
  locked = !locked;
}
