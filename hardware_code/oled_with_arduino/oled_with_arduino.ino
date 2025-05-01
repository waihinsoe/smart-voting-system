#include <U8g2lib.h>

// Assuming the same display/connection as before:
U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // sck=13, sda=11

//Variables to store incoming data
String incomingData = "";
bool newData = false;
int totalLines=0;
String lines[6];

//function declaration 
void displayLines();

void setup() {
  Serial.begin(115200);        // Start serial communication at 9600 baud rate
  u8g2.begin();              // Initialize the display
  u8g2.setFont(u8g2_font_5x7_tr);  // Set font size
}

void loop() {
 if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');  // Read the incoming data until newline
    Serial.println(receivedData);

    if(receivedData[0] == '@') {
      totalLines = 0;
      int lastComma = -1;

      for(int i=0; i< receivedData.length(); i++) {
        if(receivedData[i] == ',') {
          lines[totalLines] = receivedData.substring(lastComma+1,i);
          lastComma = i;
          totalLines++;
        }
      }

      // Capture any remaining data after the last comma
      if(lastComma < receivedData.length() -1) {
        lines[totalLines] = receivedData.substring(lastComma +1);
        totalLines++;
      }
      displayLines();

    }else{
      // Display the receivedData
      u8g2.clearBuffer();         // Clear the display buffer
      // u8g2.setFont(u8g2_font_5x7_tr);  // Set font size
      u8g2.drawStr(0, 8, receivedData.c_str()); // Draw the string at position (x=0, y=12)
      u8g2.sendBuffer();          // Send the buffer to the display
    }

  }
}

void displayLines() {

  u8g2.clearBuffer();
  for(int i=0; i < totalLines-1; i++) {
    
    if(i ==0 ) {
      lines[i].remove(0,1);
    }
    String displayData = String(i+1) +" "+ lines[i];
    int yPosition = (i + 1) * 8; // Adjust y position based on font size and display size
    u8g2.drawStr(0, yPosition, displayData.c_str());
  }
  u8g2.sendBuffer();
}
