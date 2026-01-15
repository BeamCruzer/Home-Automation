# 🏠 IoT-Based Home Automation System using ESP8266 & Blynk

A robust **IoT-enabled home automation system** built using **ESP8266 NodeMCU** and **Blynk**, allowing users to remotely monitor and control household appliances from **anywhere in the world** via **mobile or web dashboard**.  
The system also supports **offline control**, **motion-based automation**, **timed operations**, and **real-time notifications** for enhanced safety and energy efficiency.

---

## ✨ Key Features

### 🌐 Remote Appliance Control
- Control **multiple electrical appliances** (Light, Fan, Charger, etc.) using a **3-channel relay module**
- Operate devices over the **internet** via **Blynk Android app** and **Blynk Web Dashboard**
- Real-time ON/OFF status feedback for each appliance

---

### 📡 Internet & Offline Operation
- Works over **WiFi** when internet is available
- **Offline control support** using **TSOP1738 IR receiver**
- Appliances can be controlled using a standard **IR remote** (TV remote)
- Requires **direct line-of-sight**, similar to conventional TV operation

---

### 🚶 Motion-Based Automation
- Integrated **HC-SR501 PIR motion sensor**
- Automatically turns ON room lights or appliances when motion is detected
- Automatically turns OFF devices after a **configurable idle time**
- Ideal for **energy saving** and convenience

---

### ⏱️ Time-Based Automations
- Create **timed schedules** to control appliances
- Example:
  - Turn ON charger for **1 hour 30 minutes**
  - Automatically turn OFF after completion
- Helps prevent **overcharging** of mobile phones and laptops
- Reduces unnecessary power consumption

---

### 🚨 Security & Emergency Notifications
- Instant **push notifications** sent to mobile device on motion detection
- Useful for **intrusion alerts** and theft prevention
- Enables real-time awareness even when away from home

---

## ⚙️ System Specifications

| Component | Description |
|---------|------------|
| Microcontroller | **ESP8266 NodeMCU** with built-in WiFi |
| Relay Module | 3-Channel Relay (AC load switching up to **10A**) |
| Motion Sensor | HC-SR501 PIR Sensor |
| IR Receiver | TSOP1738 |
| Control Platform | **Blynk (Mobile App & Web Dashboard)** |
| Communication | WiFi (Client–Server Model) |

---

## 🧠 Working Principle

- The ESP8266 acts as a **WiFi client** and connects to the Blynk cloud server
- User inputs from mobile or web dashboard are sent to the server
- The server processes the request and sends commands back to the ESP8266
- The microcontroller switches appliances using relay outputs based on logic
- Sensor data (motion status, relay state) is continuously synced with Blynk for real-time feedback

---

## 🧰 Technologies Used

- **ESP8266 NodeMCU**
- **Blynk IoT Platform**
- **Embedded C / Arduino Framework**
- **WiFi Communication**
- **Relay-Based AC Load Switching**
- **IR Remote Control**
- **PIR Motion Detection**

---

# 🤝 Contributing

Contributions welcome!
Open an issue or submit PR

---

# ⭐ Support

If you like this project, consider giving it a **Star** ⭐ on **GitHub!**

---

