# 🚨 Dam Monitoring and Alerting System using LoRaWAN and Integrated IoT Platforms

A real-time dam monitoring and flood alert system using LoRaWAN technology, designed for remote deployment. This project integrates platforms like The Things Network, ThingSpeak, DataCake, and IFTTT to monitor water levels and rainfall, send real-time alerts, and automatically control dam gates using IoT automation.

---

## 🧠 Why This Project? (Need)

Remote dams often lack automated systems to monitor critical parameters like water level and rainfall. This creates a risk of delayed alerts, inefficient manual control, and potential disaster during floods.

**This project solves these challenges by:**
- Providing **real-time monitoring** in remote locations
- Sending **automated alerts** via SMS or email
- Allowing **automatic dam gate control** based on danger levels
- Using **low-power long-range LoRaWAN** for uninterrupted data transmission

---

## ⚙️ How It Works (Working)

The system has two main LoRa nodes and multiple cloud platforms for data handling:

### 🔹 1. Sensor Node (LoRa Node 1)
- **Sensors**: Ultrasonic (water level) + Rain sensor
- **Hardware**: Arduino Uno + LoRa Shield
- **Transmission**: Sends data via LoRaWAN to **The Things Network (TTN)**

### 🔹 2. Cloud Integration
- **TTN**: Collects and routes sensor data
- **ThingSpeak**: Visualizes water/rain levels in graphs
- **IFTTT**: Sends **real-time SMS/email alerts** to officials
- **DataCake**: Sends **downlink commands** for actuator control

### 🔹 3. Actuator Node (LoRa Node 2)
- Receives downlink data from TTN → DataCake
- **Displays level** on LCD
- **LED indicators**:
  - 🟢 Green – Safe
  - 🟡 Yellow – Caution
  - 🔴 Red + Buzzer – Danger
- **Servo motor**:
  - Partially opens gate at caution
  - Fully opens gate at danger level

### 🔄 Full Loop Summary:
1. Sensors collect water and rainfall data
2. Data goes to TTN → ThingSpeak (uplink)
3. Threshold triggers → IFTTT alert & DataCake downlink
4. LoRa Node 2 receives downlink → Alerts & gate control

---

## 🚀 Features

- 📡 LoRaWAN-based long-range wireless communication
- 🌊 Real-time dam water and rainfall monitoring
- 🔔 Automated alerts via SMS/Email (IFTTT)
- 🧠 Reflex-based dam gate control with servo motor
- 📊 Live data visualization on ThingSpeak
- ⬇️ Downlink actuation using DataCake

---

## 🛠 Technologies Used

- **LoRaWAN (The Things Network)**
- **ThingSpeak (Data plotting)**
- **IFTTT (Notification alerts)**
- **DataCake (Downlink control)**
- **Arduino Uno** with LoRa Shield
- **Sensors**: Ultrasonic, Rain
- **Servo motor**, **LEDs**, **Buzzer**, **LCD Display**


---

## 📈 Results

- Working **prototype** tested with real-time sensors
- Alerts and actuation successfully triggered across three zones:
  - Safe → Green LED
  - Caution → Yellow LED + Gate Half Open
  - Danger → Red LED + Buzzer + Gate Fully Open
- Fully integrated **IoT cloud system** using LoRaWAN

---

## 👥 Authors

Division of Electronics and Communication Engineering,  
Karunya Institute of Technology and Sciences, Coimbatore, India  

- **Aldrin G**  
  📧 aldring@karunya.edu.in

- **S. Rubeena Grace Tamilarasi**  
  📧 srubeena22@karunya.edu.in
  
- **G. Josemin Bala**  
  📧 josemin@karunya.edu

---

## 📜 License

This project is developed for educational and research purposes. Please cite the original IEEE publication if used in academic or commercial work.

