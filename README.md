# ESP32-S3 Bluetooth Mouse Jiggler Pro v6.0 🖱️⚡

This project turns an ESP32-S3 into a Bluetooth HID mouse that moves automatically to prevent your computer from going to sleep or showing as "Away". It includes a Wi-Fi web interface to control all settings from your smartphone.

<p align="center">
  <img src="images/web.png" alt="Web Interface Preview" width="400">
</p>

## 🌟 Features
* **Smart Wi-Fi Connectivity**: 
    * **Station Mode**: Connects to your home/office Wi-Fi for easy access.
    * **AP Mode (Fallback)**: If home Wi-Fi is unavailable, it creates its own hotspot (192.168.4.1) after 30 seconds.
* **Web Dashboard**: Web interface to control everything.
* **Full Customization**: Update **Bluetooth Name**, **Home Wi-Fi (STA)**, and **Local Hotspot (AP)** credentials via web.
* **Countdown Timer**: Set a duration in minutes; the device stops automatically when time is up.
* **Real-time Adjustments**: 
    * **Distance**: Pixel-perfect control of cursor movement (1px to 50px).
    * **Interval**: Set frequency of movement (1s to 60s).
    * **LED Brightness**: Adjust the intensity of the onboard RGB indicator.
* **Persistent Memory**: All settings (SSIDs, Passwords, and Timings) are saved to NVS and survive restarts.
* **Visual RGB Status**: 
    * 🔵 **Blinking Blue**: Attempting to connect to Home Wi-Fi.
    * 🔴 **Blinking Red**: Waiting for Bluetooth pairing.
    * ⚪ **Dim Gray**: Bluetooth connected & Standby.
    * 🟢 **Green Flash**: Mouse movement executed.

---

## 🛠️ Hardware Compatibility
Tested on:
---
* **Board**: ESP32-S3 DevKitC-1 (**N16R16 variant**)
* **Flash/PSRAM**: 16MB / 16MB
* **RGB LED**: Internal Pin 48
---

* **Board**: ESP32-S3 Supermini

---

## 🚀 Installation Guide

### 1. Libraries Setup (Important)
To handle Bluetooth HID, you need specific libraries:

1. **BLE Mouse Library**:
   * Download the ZIP from: [https://github.com/wakwak-koba/ESP32-NimBLE-Mouse](https://github.com/wakwak-koba/ESP32-NimBLE-Mouse)
   * In the Arduino IDE, go to **Sketch** -> **Include Library** -> **Add .ZIP Library...** and select the downloaded file.
2. **NimBLE-Arduino**:
   * In the Arduino IDE, go to **Tools** -> **Manage Libraries...**
   * Search for **"NimBLE-Arduino"** by h2zero and install it.
3. **NeoPixel**:
   * Search for **"Adafruit NeoPixel"** in the Library Manager and install it.

### 2. Board Settings
Configure your Arduino IDE under **Tools**:

**Dev Kit N16R16**
* **Board**: "ESP32S3 Dev Module"
* **USB CDC On Boot**: "Enabled"
* **Flash Mode**: "QIO 80MHz"
* **Flash Size**: "16MB"
* **PSRAM**: "OPI PSRAM"
* **Partition Scheme**: "16M Flash (3MB APP/9.9MB FATFS)"

### 3. Uploading
1. Place `MouseJiggler.ino` and `index.h` in a folder named `MouseJiggler`.
2. Open the `.ino` file, select your COM port, and click **Upload**.

---

## 📱 How to Use

### 1. Power & Connection
* **Power up**: Plug the ESP32 into any USB power source.
* **First Time Setup**: 
    1. On your phone, look for the Wi-Fi network **"ESP32-Mouse-Config"** (Pass: `12345678`).
    2. Open your browser and go to `192.168.4.1`.
    3. Enter your **Home Wi-Fi** credentials in the "Station Settings" section and click **Update**.
* **Daily Use**: Once configured, the device will automatically join your Home Wi-Fi. You can access the dashboard at:
    * **http://jiggler.local** (if working on your machine)
    * Or the local IP shown in the Arduino Serial Monitor.

### 2. Bluetooth Pairing
* On your PC, go to Bluetooth settings and "Add Device".
* Select **"ESP32-MouseJiggler"** (or the custom name you set).
* The RGB LED will turn **Dim Gray** once paired and connected.

### 3. Jiggling
* Open the Web Dashboard.
* Adjust the distance and interval as needed.
* Click **START MANUAL** or set a **Timer** for automatic shutdown.

---

## 📁 Project Structure
* `MouseJiggler.ino`: Main logic and server.
* `index.h`: Web interface code.

## ⚠️ Disclaimer
This tool is for educational use and personal productivity. Check your local IT policies before use.