/*
# ESP32_AutoFirmwareUpgrade
Automatic OTA Firmware Upgrade For ESP32.
agnath18@gmail.com

Using Github as host Server.

-> How it works?

* This program download and flash the lastest bin file available in your GitHub Repository whenever the Firmware_Version of the arduino code detect a newer version number from the "version" file of your repo.

-> How to use?

* Export your code as bin file and store it in your repository.
* Create a text file in your repo with your Firmware version number.
* Copy the download link of bin file as well as the "version" text file and paste the same in the HOST_URL & VERS_URL.


* Using DigiCert High Assurance EV Root CA : Valid Until 2031

  agnath18 :) 

*/
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include "TrustRoot.h" 

const char* HOST_URL = "raw.githubusercontent.com";
const char* VERS_URL = "https://raw.githubusercontent.com/agnath18K/ESP32_AutoFirmwareUpgrade/main/bin/version";
const char* Firm_URL = "https://raw.githubusercontent.com/agnath18K/ESP32_AutoFirmwareUpgrade/main/bin/firmware.bin";
const char* WIFI_SSID = "agnath18";
const char* WIFI_PASSWORD = "123456789";

const int LED_BUILTIN_PIN = 2;
const int httpsPort = 443;
const unsigned long Update_Interval = 60000;

enum FirmwareUpdateResult {
  FIRMWARE_UPDATE_SUCCESS,
  FIRMWARE_UPDATE_FAILED,
  FIRMWARE_NO_UPDATES
};

double Firm_Ver = 1.00;
double Lat_Ver = 0.00;
int Error_Count = 0;
unsigned long lastUpdateMillis = 0;

void setup() {
  Serial.begin(115200);
  Connect_WiFi();
  setClock();
}

void loop() {
  if (millis() - lastUpdateMillis >= Update_Interval) {
    FirmwareUpdateResult result = Firmware_Update();
    lastUpdateMillis = millis();
    if (result == FIRMWARE_UPDATE_SUCCESS) {

    }
  }
}

void Connect_WiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("\nConnecting to Wi-Fi");
  int try_count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
    try_count++;
    if (try_count >= 100) {
      Serial.println("\nConnection taking too long.");
      Error_Con();
    }
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  setClock();
}

void setClock() {
  int ntp_count = 0;
  configTime(0, 0, "pool.ntp.org", "time.google.com");
  Serial.print(F("\nWaiting for NTP time sync: "));
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    yield();
    delay(500);
    Serial.print(F("."));
    now = time(nullptr);
    ntp_count++;
    if (ntp_count >= 100) {
      Serial.println("\nConnection taking too long.");
      Error_Con();
    }
  }
}

void Error_Con() {
  Serial.print("\nError Detected.\nError Count : ");
  Serial.print(Error_Count);
  Error_Count++;
  if (Error_Count > 3) {
    Serial.print("\nProceeding ESP Restart In 60 Seconds\n");
    Error_Count = 0;
    delay(60000);
    Serial.print("\n\nRestarting ESP\n\n");
    ESP.restart();
  } else {
    Serial.println("\nRestarting WiFi Connection");
    Connect_WiFi();
  }
}

FirmwareUpdateResult Firmware_Update() {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);

  Serial.print("\nConnection to host : ");
  if (!client.connect(HOST_URL, httpsPort)) {
    Serial.println("Failed");
    Error_Con();
    return FIRMWARE_UPDATE_FAILED;
  }

  Serial.print("Success\n");

  client.print(String("GET ") + VERS_URL + " HTTP/1.1\r\n" +
               "Host: " + HOST_URL + "\r\n" +
               "User-Agent: ESP32_Firmware_Updater\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String line = client.readStringUntil('\n');
  Lat_Ver = line.toDouble();

  if (Lat_Ver == 404.00) {
    Serial.println("\nFailed to retrieve Version file.\nSkipping Firmware Check\n");
    return FIRMWARE_UPDATE_FAILED;
  }

  if (Firm_Ver >= Lat_Ver) {
    Serial.println("\nDevice Running On Latest Firmware");
    return FIRMWARE_NO_UPDATES;
  } else {
    Serial.println("\nNew Firmware Found!");
    Serial.println("\nStarting Upgrade\n");
    httpUpdate.setLedPin(LED_BUILTIN_PIN, LOW);
    t_httpUpdate_return ret = httpUpdate.update(client, Firm_URL);

    if (ret == HTTP_UPDATE_OK) {
      Serial.println("FIRMWARE_UPDATE_OK");
      return FIRMWARE_UPDATE_SUCCESS;
    } else {
      Serial.printf("FIRMWARE_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      return FIRMWARE_UPDATE_FAILED;
    }
  }
}
