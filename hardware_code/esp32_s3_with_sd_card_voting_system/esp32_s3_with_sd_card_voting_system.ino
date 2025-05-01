#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

/***************************** wifi setup *****************************/
#include <WiFi.h>
// WiFi network credentials
#define WIFI_SSID "6ecthesis"
#define WIFI_PASSWORD "6ecthesis"

/***************************** firebase setup *************************/

#include <Firebase_ESP_Client.h>
// Include helper functions for Firebase token and RTDB management
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define API_KEY "AIzaSyB_GmRi5EKE3YCSDfAR8eywn8bwhGTsZYA"
#define USER_EMAIL "votingsystem27@gmail.com"
#define USER_PASSWORD "123@voting"
#define DATABASE_URL "https://smart-voting-system-70e58-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase objects for managing data and authentication
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;

/*****************************firebase setup *************************/

HardwareSerial mySerial(1); // Use UART1 for serial communication

// Define the SPI pins
#define MOSI 8
#define MISO 13
#define CLK  14
#define CS   15

// Define GPIO pins for the switches
#define CANCEL_SWITCH 40
#define SWITCH_1 35
#define SWITCH_2 36
#define SWITCH_3 37
#define SWITCH_4 38
#define SWITCH_5 39
#define MAX_CANDIDATES 5

String candidates[MAX_CANDIDATES];
int numCandidates = 0;

File myFile;

/***************** declare function *****************/
void initWiFi();
void setupFirebase();
void setupSwitches();
void loadCandidates();
void syncVoteListDataToFirebase();
void syncVotersDataToFirebase();
void syncCandidatesDataToFirebase();
void checkCardType();
void processVoter(String cardID);
bool idExists(const String& id, const char* filePath);
bool hasVoted(const String& id, const char* filePath);
String getCandidateSelection();
void recordVote(const String& voterID, const String& candidate);




void setup() {
  Serial.begin(115200); // Start the main serial at 115200 baud rate for debugging
  Serial.println("Init setup .....");

  while (!Serial) {
    ; // Wait for the serial port to connect. Necessary only for certain platforms.
  }

  initWiFi();
  setupFirebase();

  SPI.begin(CLK, MISO, MOSI, CS);  // Initialize SPI
  // Initialize SD card
  if (!SD.begin(CS)) {
    Serial.println("Card Mount Failed");
    return;
  }


  Serial.println("Card Mount Successful");
  checkCardType();

  loadCandidates();
  setupSwitches();

  syncCandidatesDataToFirebase();
  syncVotersDataToFirebase();
  syncVoteListDataToFirebase();

   Serial.println("Setup completed !");

}

void loop() {
  StaticJsonDocument<200> doc; // Allocate a small JSON document
  
  // Check if data is available on the main serial
  if (Serial.available()) {
    DeserializationError error = deserializeJson(doc, Serial);
    if (!error) {
      String cardID = doc["cardID"].as<String>();
        
      processVoter(cardID);
    } else {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
  }
}


void setupSwitches() {
   // Initialize candidate switch GPIOs as inputs
  pinMode(CANCEL_SWITCH, INPUT_PULLUP);  // Initialize cancel switch GPIO as input
  pinMode(SWITCH_1, INPUT_PULLUP);
  pinMode(SWITCH_2, INPUT_PULLUP);
  pinMode(SWITCH_3, INPUT_PULLUP);
  pinMode(SWITCH_4, INPUT_PULLUP);
  pinMode(SWITCH_5, INPUT_PULLUP);

}

void loadCandidates() {
  File file = SD.open("/candidates.txt",FILE_READ);
  if(!file) {
    Serial.println('Failed to open candidates file');
    return;
  }

  while(file.available() && numCandidates < MAX_CANDIDATES ) {
    candidates[numCandidates] = file.readStringUntil('\n');
    candidates[numCandidates].trim(); // Remove any extra whitespace
    numCandidates++;
  }
  file.close();
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  // Serial.println(WiFi.localIP());
  // Serial.println();
}

void setupFirebase() {

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  // Initialize Firebase with the configured settings
  Firebase.begin(&config, &auth);

  // Wait for user UID as it's necessary for database path
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  uid = auth.token.uid.c_str();
  // Serial.print("User UID: ");
  // Serial.println(uid);
  Serial.println("OK ..");

}

void syncVoteListDataToFirebase() {
  File file = SD.open("/votelist.txt", FILE_READ);
  if(!file) {
    // Serial.println("Failed to open file for reading");
    Serial.println("Failed file open!");
    return;
  }
  FirebaseJson json;

  while(file.available()) {
    String line = file.readStringUntil('\n');
    line.trim(); //Trim any whitespace
    if(line.length() > 0) {
      int index = line.indexOf(',');
      if(index != -1) {
        String voterID = line.substring(0,index);
        String candidate = line.substring(index + 1);
        json.set(voterID , candidate);
      }
    }
  }
  file.close();

  String path = "/ElectionData/" + String(uid) + "/voteList";
  // old voteList delete
  if(!Firebase.RTDB.deleteNode(&fbdo, path)) {
    // Serial.println("Failed to clear data: "+ String(fbdo.errorReason()));
    Serial.println("Failed to clear data");
  }else {
    // Serial.println("Old voteList cleared as no new votes were found.");
    Serial.println("Old viteList cleared");
  }

  // new voteList add
  if(!Firebase.RTDB.updateNode(&fbdo, path, &json)) {
    // Serial.println("Failed to upload data: "+ String(fbdo.errorReason()));
    // Serial.println("Failed to upload data");
  }else {
    // Serial.println("VoteList uploaded successfully!");
    Serial.println("uploaded successfully!");
  }
}

void syncVotersDataToFirebase() {
  File file = SD.open("/voters.txt",FILE_READ);
  if(!file) {
    Serial.println("Failed to open for reading");
    return;
  }

  FirebaseJson json;
  while(file.available()) {
    String line = file.readStringUntil('\n');
    line.trim(); // Trim any whitespace

    if(line.length() > 0) {
      //Split the line at each comma
      int firstCommaIndex = line.indexOf(',');
      int secondCommaIndex = line.indexOf(',',firstCommaIndex + 1);
      int thirdCommaIndex = line.indexOf(',', secondCommaIndex + 1);

      if(thirdCommaIndex != -1) {
        String voterID = line.substring(0,firstCommaIndex);
        String name = line.substring(firstCommaIndex+1,secondCommaIndex);
        String qualification = line.substring(secondCommaIndex+1, thirdCommaIndex);
        String position = line.substring(thirdCommaIndex+1);
        
        FirebaseJson voterInfo;
        voterInfo.set("name",name);
        voterInfo.set("qualification",qualification);
        voterInfo.set("position",position);

        // Use the RFID  as the key for each voter
        json.set(voterID,voterInfo);
      }
    }
  }
  file.close();

   String path = "/ElectionData/" + String(uid)+"/voters";
  
  // delete old data
  if(!Firebase.RTDB.deleteNode(&fbdo, path)) {
    // Serial.println("Failed to clear data: "+ String(fbdo.errorReason()));
    
  }else {
    Serial.println("Old voters data cleared.");
  }

  // update new data
  if(!Firebase.RTDB.updateNode(&fbdo, path, &json)) {
    // Serial.println("Failed to upload data: "+ String(fbdo.errorReason()));
    Serial.println("Failed to clear data!");
  }else{
    Serial.println("Voters data upload successfully!");
  }
}

void syncCandidatesDataToFirebase() {
  File file = SD.open("/candidates.txt", FILE_READ);
  if(!file) {
    // Serial.println("Failed to open file for reading");
    Serial.println("Failed file open!");
    return;
  }

  FirebaseJson json;
  int candidateIndex = 0; // Simple index to create unique keys for each candidate
  while(file.available()) {
    String candidate = file.readStringUntil('\n');
    candidate.trim(); // Trim any whitespace
    if(candidate.length()> 0) {
      candidateIndex++;
      String key = "candidate"+String(candidateIndex);
      json.set(key, candidate);
    }
  }
  file.close();

  String path = "/ElectionData/" + String(uid) + "/candidates";
  // delete old data
  if(!Firebase.RTDB.deleteNode(&fbdo, path)) {
    // Serial.println("Failed to clear data: "+ String(fbdo.errorReason()));
    Serial.println("Failed to clear data");
  }else {
    Serial.println("Old candidates data cleared.");
  }
  // update new data
  if(!Firebase.RTDB.updateNode(&fbdo, path,&json)) {
    // Serial.println("Failed to upload data: "+ String(fbdo.errorReason()));
    Serial.println("Failed to upload data");
  }else{
    // Serial.println("Candidates data uploaded successfully!");
    Serial.println("data uploaded successfully!");
  }
  
}

void checkCardType() {
  uint8_t cardType = SD.cardType();
  Serial.print("SD Card Type: ");
  switch(cardType) {
    case CARD_NONE: Serial.println("No SD card attached"); return;
    case CARD_MMC: Serial.println("MMC"); break;
    case CARD_SD: Serial.println("SDSC"); break;
    case CARD_SDHC: Serial.println("SDHC"); break;
    default: Serial.println("UNKNOWN"); break;
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void processVoter(String cardID) {
  if (!idExists(cardID, "/voters.txt")) {
    Serial.println("Invalid voter!");
    return;
  }
  Serial.println("Voter validated,");
  if (hasVoted(cardID, "/votelist.txt")) {
    Serial.println("Already voted!");
    return;
  }
  String candidate = getCandidateSelection();

  if(candidate == "CANCEL") {
    Serial.println("Voting cancelled.");
    return;
  } else if (!candidate.isEmpty()) {
    if (!idExists(cardID, "/voters.txt")) {
      Serial.println("Invalid voter!");
      return;
    }
    if (hasVoted(cardID, "/votelist.txt")) {
      Serial.println("Already voted!");
      return;
    }
    recordVote(cardID, candidate);
  }
}

bool idExists(const String& id, const char* filePath) {
  myFile = SD.open(filePath, FILE_READ);
  if (!myFile) {
    Serial.println("Failed to open file");
    return false;
  }

  String currentLine;
  while (myFile.available()) {
    currentLine = myFile.readStringUntil('\n');
    if (currentLine.indexOf(id) != -1) {
      myFile.close();
      return true;
    }
  }

  myFile.close();
  return false;
}

bool hasVoted(const String& id, const char* filePath) {
  return idExists(id, filePath);
}

String getCandidateSelection() {
  Serial.print('@');
  for(int i=0; i < numCandidates; i++) {
    // Serial.print("Press ");
    // Serial.print(i+1);
    // Serial.print(" for ");
    Serial.print(candidates[i]);
    Serial.print(",");                  
    
  }
  Serial.print("cancel");
  Serial.print(",");
  Serial.println();

  while (true) { // Infinite loop to keep checking the switches
    if (digitalRead(CANCEL_SWITCH) == LOW) return "CANCEL";
    if (digitalRead(SWITCH_1) == LOW && numCandidates >= 1) return candidates[0];
    if (digitalRead(SWITCH_2) == LOW && numCandidates >= 2) return candidates[1];
    if (digitalRead(SWITCH_3) == LOW && numCandidates >= 3) return candidates[2];
    if (digitalRead(SWITCH_4) == LOW && numCandidates >= 4) return candidates[3];
    if (digitalRead(SWITCH_5) == LOW && numCandidates >= 5) return candidates[4];

    delay(10); // Short delay to prevent excessive CPU usage
  }
  // No need for a return here as the loop should not exit unless a switch is pressed
}

void recordVote(const String& voterID, const String& candidate) {
  myFile = SD.open("/votelist.txt", FILE_APPEND);
  if (myFile) {
    myFile.println(voterID + "," + candidate);
    myFile.close();
    Serial.println("Vote recorded");

    if(Firebase.ready()) {
      syncVoteListDataToFirebase();
    }

  } else {
    Serial.println("Error recording vote");
  }
}


