\# ESP32-S3 Bluetooth Mouse Jiggler Pro v6.0 🖱️⚡



A professional-grade, hardware-based HID (Human Interface Device) solution to prevent computer sleep/standby mode. Unlike software alternatives, this device is detected as a \*\*physical Bluetooth mouse\*\*, making it virtually undetectable by IT monitoring software.



\## 🌟 Features

\* \*\*Dual-Mode Functionality\*\*: Acts as a Bluetooth Low Energy (BLE) Mouse for the PC and a Wi-Fi Access Point (AP) for remote control.

\* \*\*Web Interface\*\*: A modern, mobile-responsive dashboard with a zero-refresh UI.

\* \*\*Dynamic Customization\*\*: Change the \*\*Bluetooth Device Name\*\*, \*\*Wi-Fi SSID\*\*, and \*\*Password\*\* directly from the web interface.

\* \*\*Countdown Timer\*\*: Set a specific duration (in minutes) for the jiggler to run before automatically stopping.

\* \*\*Real-time Adjustments\*\*: 

&nbsp;   \* \*\*Distance\*\*: Control how many pixels the cursor moves (1px to 50px).

&nbsp;   \* \*\*Interval\*\*: Set frequency of movement (1s to 60s).

&nbsp;   \* \*\*LED Brightness\*\*: Adjust the intensity of the onboard RGB status indicator.

\* \*\*Persistent Storage\*\*: All settings are saved to the ESP32's NVS memory and survive power cycles.

\* \*\*Visual Feedback\*\*: Onboard RGB LED indicates:

&nbsp;   \* 🔴 \*\*Blinking Red\*\*: Waiting for Bluetooth pairing.

&nbsp;   \* ⚪ \*\*Dim Gray\*\*: Connected but Idle (Standby).

&nbsp;   \* 🟢 \*\*Green Blink\*\*: Mouse movement executed.



---



\## 🛠️ Hardware Compatibility

Specifically tested and optimized for:

\* \*\*Board\*\*: ESP32-S3 DevKitC-1 (\*\*N16R16 variant\*\*)

\* \*\*MCU\*\*: ESP32-S3

\* \*\*Memory\*\*: 16MB Flash / 16MB PSRAM

\* \*\*Peripherals\*\*: Internal RGB LED (Default Pin 48)



\*Note: Compatible with most ESP32-S3 and C3 boards. For other models, verify the NeoPixel pin in the code.\*



---



\## 🚀 Installation Guide



\### 1. Prerequisites

\* Install \[Arduino IDE](https://www.arduino.cc/en/software).

\* Add ESP32 Board Support: `File` > `Preferences` > `Additional Boards Manager URLs`:

&nbsp; `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package\_esp32\_index.json`

\* Install these libraries via \*\*Library Manager\*\*:

&nbsp;   \* `NimBLE-Arduino`

&nbsp;   \* `ESP32-BLE-Mouse` (by T-vK)

&nbsp;   \* `Adafruit NeoPixel`



\### 2. Board Settings (Dev Kit N16R16)

Select the following under the \*\*Tools\*\* menu:

\* \*\*Board\*\*: "ESP32S3 Dev Module"

\* \*\*USB CDC On Boot\*\*: "Enabled"

\* \*\*Flash Mode\*\*: "QIO 80MHz"

\* \*\*Flash Size\*\*: "16MB"

\* \*\*PSRAM\*\*: "OPI PSRAM"

\* \*\*Partition Scheme\*\*: "16M Flash (3MB APP/9.9MB FATFS)"



\### 3. Uploading

1\. Download `MouseJiggler.ino` and `index.h`.

2\. Ensure both files are in the same folder named `MouseJiggler`.

3\. Connect your ESP32-S3 via USB.

4\. Select the correct Port and click \*\*Upload\*\*.



---



\## 📱 How to Use

1\. \*\*Power up\*\*: Connect the ESP32 to any USB power source.

2\. \*\*Connect Wi-Fi\*\*: On your phone, join the network \*\*"ESP32-Mouse-Config"\*\* (Pass: `12345678`).

3\. \*\*Open Dashboard\*\*: Navigate to `192.168.4.1` in your browser.

4\. \*\*Pair Bluetooth\*\*: On your PC, search for a new Bluetooth device named \*\*"ESP32-MouseJiggler"\*\*.

5\. \*\*Control\*\*: Use the web interface to toggle the jiggler or change device names.



---



\## 📁 Project Structure

\* `MouseJiggler.ino`: Core logic, Wi-Fi AP server, and dynamic BLE initialization using pointers.

\* `index.h`: Compressed HTML/CSS/JS string for the web controller.



\## ⚠️ Disclaimer

This tool is intended for educational purposes and personal productivity (e.g., preventing screen timeout during long renders). Please use responsibly and in accordance with your organization's IT policies.

