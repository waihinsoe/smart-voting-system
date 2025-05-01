# Smart Voting System Using RFID and ESP32

A secure and efficient **IoT-based electronic voting system** utilizing **RFID authentication** and **ESP32 microcontroller**, designed for tamper-resistant, transparent voting processes. This project replaces traditional voting with an automated, verifiable, and user-friendly digital system.

## 🎯 Aim & Objectives

### Aim

To design and implement a **secure and effective voting machine** using **RFID card technology**.

### Objectives

-   Assign data to RFID cards for secure voter input.
-   Design and build an RFID reader system.
-   Implement an automated voting process with vote encryption and validation.

## 🧠 Application Areas

-   Civil elections
-   Organizational/committee polls
-   Secure voting in educational institutions
-   Membership-based voting systems

## 🧰 Hardware Components

-   **ESP32 Development Board** – Main controller with Wi-Fi support
-   **RFID Reader Scanner (MFRC-522)** – For reading RFID voter cards
-   **RFID Tags or Cards** – Each represents a unique voter ID
-   **OLED Display** – Shows voting status and options
-   **SD Card Adapter** – Temporary encrypted vote storage
-   **Push-Button Switches** – Used to select candidates or confirm votes

## 💻 Software & Cloud Requirements

### Software Layer

-   Manages: voting logic, encryption, SD card writing, OLED UI, RFID input

### Cloud Layer

-   Uses **Firebase** for secure, real-time vote syncing
-   Ensures vote auditability, authorized access, and storage reliability

## 🔁 System Flow

1. Voter scans RFID card using MFRC-522.
2. System verifies if the card is registered and not yet used.
3. Voter uses buttons to cast a vote, which is then encrypted.
4. Vote is stored on an SD card and synced to Firebase.
5. OLED display gives real-time feedback (e.g., "Vote Recorded", "Already Voted").

## 🔐 Voter Authentication

-   RFID tag contains a unique serial number.
-   System cross-references with a predefined database (name, NRC, position).
-   Prevents multiple votes from the same RFID card.

## 📦 Assigning Data to RFID Cards

-   Data like Voter ID or choices is mapped to RFID cards.
-   Mapped in software via a structured database or dictionary.
-   RFID reader pins are connected to ESP32’s SPI interface.

## 🧱 Block Diagram & Flowcharts

-   Block diagrams show all connections: RFID reader, SD card, OLED, buttons, and ESP32.
-   Flowcharts represent the entire logic from card detection to vote casting and syncing.

## ✅ Conclusion

This RFID-based smart voting system increases:

-   **Security** through authentication and encryption
-   **Transparency** using OLED feedback and Firebase storage
-   **Efficiency** with automation and real-time vote compilation

## 🔮 Future Work

-   Finalize database integration for voter management
-   Complete simulation and testing using Proteus and Firebase
-   Extend system for multilingual and multi-option voting

## 📚 References

-   S. Zubair, _Design and Construction of a Simple Microcontroller Based Conference Voting Machine_
-   Barbara Ondrisek, _E-Voting System Security Optimization_, HICSS 2009
-   Ronald L. Rivest, _Electronic Voting_, MIT Technical Report
-   [International IDEA], _Introducing Electronic Voting: Essential Considerations_

---

**Thank you for your attention!**
