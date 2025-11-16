# KEYLESS-ESP32-C3-Super-Mini

This project implements a secure **keyless entry system** for motorcycles using two ESP32-C3 microcontrollers communicating through **ESP-NOW**. The system improves vehicle access security by combining ESP-NOW’s low-latency peer-to-peer communication with a lightweight **Transposition Cipher** for encrypted authentication.

The project consists of:
- **MCU1 (Remote / Transmitter)** — carried by the user
- **MCU2 (Receiver / Vehicle Unit)** — installed on the motorcycle

---

## 🚀 Features

- **ESP-NOW peer-to-peer wireless communication**
- **No WiFi, no pairing, no Bluetooth**
- **Encrypted authentication using Transposition Cipher**
- **Fast connection (<10ms) with low power consumption**
- **Automatic unlock when remote is in range**
- **Relay control for ignition (ON/OFF)**
- **LED indicator for status**
- **Buzzer notification on connect/disconnect**

---

## 🧩 System Overview

### **MCU1 – Remote Unit (Transmitter)**
- Sends encrypted “authentication token” periodically
- Battery-powered compact remote
- Designed for low power consumption (sleep-ready)

### **MCU2 – Vehicle Unit (Receiver)**
- Constantly listens to ESP-NOW packets
- Decrypts and validates token
- Controls:
  - Relay (IGNITION)
  - LED indicator (status)
  - Buzzer (connection alert)

---

## 🔐 Encryption Method: Transposition Cipher

A lightweight encryption method suitable for microcontrollers:

1. Authentication token placed into matrix  
2. Columns swapped using secret key  
3. MCU2 performs reverse-transposition  
4. Only matching decrypted token unlocks ignition  

This method requires minimal CPU but improves security significantly.

---

## 📡 ESP-NOW Communication Flow

1. MCU1 broadcasts encrypted token via ESP-NOW
2. MCU2 receives packet instantly (low-latency)
3. MCU2 decrypts token and verifies
4. If token is valid:
   - Relay turns **ON**
   - LED lights up
   - Buzzer beeps for confirmation
5. If token stops arriving (remote out of range):
   - Relay turns **OFF**
   - Buzzer beeps again
   - LED turns off

---

## 🛠 Hardware Requirements

### **MCU1 (Remote)**
- ESP32-C3 Super Mini
- Battery (Li-Ion / LiPo / DC 5V)
- Optional button or LED

### **MCU2 (Receiver)**
- ESP32-C3 Super Mini
- Relay module (ignition)
- LED indicator
- Active buzzer
- 5V–12V power supply (depends on relay)

---

---

## 🔧 Installation & Setup

 1. Upload the firmware
Flash each `.ino` file to its corresponding ESP32-C3 using Arduino IDE or PlatformIO.

 2. Set MAC addresses
In the code:
- MCU1 must send to **MCU2’s MAC**
- MCU2 must allow **MCU1’s MAC**

ESP32 MAC can be printed using:
Serial.println(WiFi.macAddress());

 3.Power on the system
-MCU2 (receiver) starts listening for packets
-MCU1 (remote) begins sending encrypted tokens every few milliseconds

 4. Bring remote close to motorcycle
-Receiver validates token
-Relay activates → ignition ON
-LED turns ON
-Buzzer beeps once
 5. Move remote away
-Packet loss detected
-Relay OFF
-LED OFF
-Buzzer beeps

⚠️ Security Notes
 -ESP-NOW is not easily sniffed because it does not use standard WiFi frames.
 -Encryption adds extra protection against packet replay or cloning.
 -Avoid sharing your encryption key publicly.

📜 License
This project is released under the MIT License.
See LICENSE for full details.

🤝 Credits
ESP-NOW examples from Espressif
Custom Transposition Cipher algorithm
System design & implementation by Moch Rizky F.

📞 Contact
For contribution or questions, please open an Issue in this repository.
