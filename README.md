# ESP32_AutoFirmwareUpgrade

## Automatic OTA Firmware Upgrade For ESP32
Automatically upgrade the firmware of your ESP32 device over-the-air (OTA) using a GitHub repository as the host server.

## How it works?

This program is designed to download and flash the latest binary firmware file available in your GitHub repository whenever the firmware version in your Arduino code detects a newer version number from the "version" file in your repository.

## How to use?

Follow these steps to set up and use ESP32_AutoFirmwareUpgrade:

1. **Export Your Code**: First, export your Arduino code as a binary (bin) file. This will be the firmware that your ESP32 will upgrade to.

2. **GitHub Repository Setup**:
    - Store the exported binary (bin) file in your GitHub repository.
    - Create a text file in your repository and name it "version." In this file, store your firmware version number.
    - Ensure that your GitHub repository is accessible to the ESP32 device.

3. **Configure URLs**:
    - Open the ESP32_AutoFirmwareUpgrade program and locate the "HOST_URL" and "VERS_URL" variables.
    - Set "HOST_URL" to the direct download link of the binary (bin) file in your GitHub repository.
    - Set "VERS_URL" to the direct download link of the "version" text file in your GitHub repository.

4. **DigiCert High Assurance EV Root CA**:
    - This program uses the DigiCert High Assurance EV Root CA for secure connections.
    - The certificate is valid until 2031, providing secure and encrypted communication.

5. **Upload and Run**:
    - Upload the ESP32_AutoFirmwareUpgrade program to your ESP32 device.
    - The program will periodically check for a new firmware version. If a newer version is detected in your GitHub repository, it will automatically download and flash the new firmware.

6. **Enjoy Automatic OTA Updates**:
    - With the ESP32_AutoFirmwareUpgrade program running on your ESP32, you can now enjoy the convenience of automatic firmware updates without the need for manual interventions.

## Author
This project was developed by agnath18. If you have questions or need support, feel free to reach out.

## License
This software is provided under an open-source license. Please refer to the license file for details.

---

Please note that setting up and using ESP32_AutoFirmwareUpgrade requires some knowledge of GitHub repositories, ESP32 programming, and secure connections. Make sure to follow the instructions carefully and use this tool responsibly. Enjoy seamless OTA firmware upgrades for your ESP32 devices!
